//
// Created by X-ray on 12/09/2026.
//

#include "as_value.hpp"

#include "../script/as_script_manager.hpp"
#include "../util/as_bind.hpp"

#include <angelscript/scriptarray/scriptarray.h>
#include <angelscript/scriptdictionary/scriptdictionary.h>

#include <base-common/fs/vfs.hpp>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <new>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// A script's `serialization::Value` is a name for one node of a document stored in a GenericTree, and
// this file is the whole of the layer between the two: the tree knows nothing about scripts, and the
// class above knows nothing about AngelScript. The namespace is set in RegisterValue, at the end of
// the file, so everything below reads the way a script writes it.
//
// Three things about that layer are worth knowing before reading it. Value is a value type, so
// AngelScript makes and destroys its copies itself - the constructs and the destruct below are what
// that rests on, and there is no reference count anywhere in this file. Every method is registered by
// hand with its declaration spelled out, because the declarations are the script's API and several of
// them - a returned value, a reference to a whole array - are shapes the RegisterMethod helper refuses.
// And the writes are one method taking `const ?&in`, which is the same choice the standard dictionary
// makes: it lets a script assign anything at all to a node, and the type that arrives is the type
// that decides what the node becomes, so there is no overload set for the compiler to pick from.
namespace base::menu::as::bindings::value {
  namespace {
    using menu::util::GenericTree;
    using menu::util::GenericView;
    using menu::util::ValueFormat;

    /// Every Value that exists, counted by the two ends of its life. See LiveInstances.
    std::int64_t live_instances = 0;

    /// The document a new Value names, in the running script's own directory - the one file name a
    /// script has unqualified access to. The directory is made with it, so a script that only reads
    /// is left with an empty one rather than with nowhere to look, which is also what makes the
    /// containment test meaningful: it is resolved against a directory that exists.
    ///
    /// A Value is a value type, so there is no way to fail its construction and hand back nothing -
    /// so a Value made where no script is running gets a tree with no directory at all, which every
    /// read and write in memory still works on and every file operation refuses.
    std::shared_ptr<GenericTree> NewDocument() {
      const std::string name = script::ScriptManager::RunningScriptName();
      if (name.empty()) {
        LOG_ERROR("[AS] A Value was made outside of a loaded script, which has no directory of its own for a document "
                  "to be saved in: this one cannot be saved or loaded");
        return std::make_shared<GenericTree>(std::filesystem::path{});
      }

      return std::make_shared<GenericTree>(common::fs::vfs::GetScriptDataDir(name));
    }

    /// `Value root;` - a new, empty document.
    void DefaultConstruct(ScriptValue* self) {
      new (self) ScriptValue(GenericView(NewDocument(), {}));
    }

    /// `Value other = root["a"];` - another name for a node of the same document, which is the whole
    /// of what copying a Value means. Registered as the type's copy constructor, and reading the
    /// other Value through View() rather than copying it, since ScriptValue has no C++ copy.
    void CopyConstruct(const ScriptValue& other, ScriptValue* self) {
      new (self) ScriptValue(other.View());
    }

    /// Where the count comes down. AngelScript calls this for every Value it holds - a local at the
    /// end of its scope, a temporary at the end of its statement, and a copy it made for a call.
    void Destruct(ScriptValue* self) {
      self->~ScriptValue();
    }

    /// `root["a"]`, and `root["list"][0]`. The result is a Value of its own rather than a reference
    /// into this one, which is what makes a subscript chain writable from end to end.
    ScriptValue SubscriptKey(const ScriptValue* self, const std::string& key) {
      return self->Subscript(key);
    }

    ScriptValue SubscriptIndex(const ScriptValue* self, AngelScript::asUINT index) {
      return self->Subscript(index);
    }

    /// The engine of the script being run. Every conversion to one of the add-on's types needs it to
    /// make the object, and it is only reachable while a script is running - which a native called
    /// from a script always is.
    AngelScript::asIScriptEngine* CurrentEngine() {
      AngelScript::asIScriptContext* context = AngelScript::asGetActiveContext();
      if (!context) {
        LOG_ERROR("[AS] A Value conversion was called outside of a script, where there is no engine to build a value with");
        return nullptr;
      }

      return context->GetEngine();
    }

    /// Whether `type_id` is the string type, which is an object type and so has no id of its own in
    /// the fixed part of the range.
    bool IsString(const AngelScript::asIScriptEngine* engine, const int type_id) {
      return type_id == engine->GetTypeIdByDecl("std::string");
    }

    /// The value of a primitive of any width, read as the tree's one number type.
    ///
    /// The width comes from the engine rather than from a switch over the twelve primitive type ids,
    /// because an integer written in a script is an int32 and nothing else, and a switch that only
    /// listed the ids this code happens to expect would read an int as eight bytes and hand back a
    /// number made of the int and whatever followed it. The engine answers for enumerations too,
    /// which are stored as the 32-bit integers they are declared as.
    ///
    /// Unsignedness is the one thing the size does not carry, so it is the type id that says.
    ///
    /// Returns false for a type the engine does not call primitive - an object, a handle, or the
    /// any-typed parameter itself.
    bool ReadPrimitive(const void* value, const int type_id, const AngelScript::asIScriptEngine* engine, double& out) {
      const int size = engine->GetSizeOfPrimitiveType(type_id);
      if (!size) {
        return false;
      }

      const bool is_unsigned = type_id >= AngelScript::asTYPEID_UINT8 && type_id <= AngelScript::asTYPEID_UINT64;
      switch (size) {
        case 1:
          out = is_unsigned ? static_cast<double>(*static_cast<const std::uint8_t*>(value))
                            : static_cast<double>(*static_cast<const std::int8_t*>(value));
          return true;
        case 2:
          out = is_unsigned ? static_cast<double>(*static_cast<const std::uint16_t*>(value))
                            : static_cast<double>(*static_cast<const std::int16_t*>(value));
          return true;
        case 4:
          out = is_unsigned ? static_cast<double>(*static_cast<const std::uint32_t*>(value))
                            : static_cast<double>(*static_cast<const std::int32_t*>(value));
          return true;
        case 8:
          out = is_unsigned ? static_cast<double>(*static_cast<const std::uint64_t*>(value))
                            : static_cast<double>(*static_cast<const std::int64_t*>(value));
          return true;
        default:
          return false;
      }
    }

    /// `root["a"] = anything` - the single assignment a script has, and the one place a node's shape
    /// is decided by what a script wrote rather than by which method it called.
    ///
    /// The value comes first and the object last, the shape of every constructor here: a parameter of
    /// the any type arrives as a pair - a pointer to the value and its type id - and this is the shape
    /// the standard dictionary registers its own opAssign with, so it is the shape a `?&in` parameter
    /// is known to work in.
    void Assign(void* value, const int type_id, ScriptValue* self) {
      auto& view = self->View();

      // Nothing at all, which is what a script that assigned null means by it.
      if (type_id == 0 || !value) {
        view.SetNull();
        return;
      }

      if (type_id == AngelScript::asTYPEID_BOOL) {
        view.WriteBool(*static_cast<const bool*>(value));
        return;
      }

      const auto* engine = CurrentEngine();
      if (!engine) {
        return;
      }

      if (IsString(engine, type_id)) {
        view.WriteString(*static_cast<const std::string*>(value));
        return;
      }

      // Every number the tree holds is a double, so a float and a double arrive the same way and only
      // differ in how many of the bits they brought with them.
      if (type_id == AngelScript::asTYPEID_FLOAT) {
        view.WriteFloat(*static_cast<const float*>(value));
        return;
      }

      if (type_id == AngelScript::asTYPEID_DOUBLE) {
        view.WriteDouble(*static_cast<const double*>(value));
        return;
      }

      if (double number{}; ReadPrimitive(value, type_id, engine, number)) {
        view.WriteDouble(number);
        return;
      }

      LOG_ERROR("[AS] Cannot assign a value of type '{}' to this node: assign its members instead, or use one of the "
                "Set...Array or SetDictionary methods",
                engine->GetTypeDeclaration(type_id));
    }

    bool GetBool(ScriptValue* self) {
      return self->View().ReadBool();
    }

    std::int32_t GetInt(ScriptValue* self) {
      return self->View().ReadInt();
    }

    std::int64_t GetInt64(ScriptValue* self) {
      return self->View().ReadInt64();
    }

    float GetFloat(ScriptValue* self) {
      return self->View().ReadFloat();
    }

    double GetDouble(ScriptValue* self) {
      return self->View().ReadDouble();
    }

    std::string GetString(ScriptValue* self) {
      return self->View().ReadString();
    }

    bool IsNull(ScriptValue* self) {
      return self->View().IsNull();
    }

    bool IsObject(ScriptValue* self) {
      return self->View().IsObject();
    }

    bool IsArray(ScriptValue* self) {
      return self->View().IsArray();
    }

    bool IsNumber(ScriptValue* self) {
      return self->View().IsNumber();
    }

    bool IsBool(ScriptValue* self) {
      return self->View().IsBool();
    }

    bool IsStringValue(ScriptValue* self) {
      return self->View().IsString();
    }

    bool Contains(ScriptValue* self, const std::string& key) {
      return self->View().Contains(key);
    }

    AngelScript::asUINT GetSize(ScriptValue* self) {
      return static_cast<AngelScript::asUINT>(self->View().Size());
    }

    /// Removing a member, and removing an element, are the two names a script has for the same thing:
    /// the node goes, and whatever was after an element moves up.
    bool RemoveKey(ScriptValue* self, const std::string& key) {
      return self->View().Key(key).Erase();
    }

    bool RemoveIndex(ScriptValue* self, AngelScript::asUINT index) {
      return self->View().Index(index).Erase();
    }

    /// The node an array element becomes. The tree holds three kinds of leaf - a number, a boolean and
    /// a string - so an element's type decides which one it is, and an integer of any width becomes the
    /// one number type there is.
    template<typename T>
    glz::generic ElementNode(const T& value) {
      glz::generic node;
      if constexpr (std::is_same_v<T, bool>) {
        node = value;
      } else if constexpr (std::is_same_v<T, std::string>) {
        node = value;
      } else {
        node = static_cast<double>(value);
      }

      return node;
    }

    /// The reverse, for one element: a node of another type reads as the element type's zero, which is
    /// the same answer a missing read gives.
    template<typename T>
    void ReadElement(const glz::generic& node, T& out) {
      if constexpr (std::is_same_v<T, bool>) {
        out = node.is_boolean() && node.get_boolean();
      } else if constexpr (std::is_same_v<T, std::string>) {
        out = node.is_string() ? node.get_string() : std::string{};
      } else {
        out = static_cast<T>(node.is_number() ? node.get_number() : 0.0);
      }
    }

    /// This node's elements as an array of `element_type`, which the declaration is what names.
    ///
    /// A node that is not an array gives an empty array rather than a failure: a script reading one
    /// before it has written anything should not have to test the shape first, and the six Is...
    /// methods are there for the script that wants to tell the difference.
    template<typename T>
    AngelScript::CScriptArray* GetArray(ScriptValue* self, const char* element_type) {
      auto* engine = CurrentEngine();
      if (!engine) {
        return nullptr;
      }

      auto* array = AngelScript::CScriptArray::Create(engine->GetTypeInfoByDecl(element_type));
      if (!array) {
        LOG_ERROR("[AS] Cannot make a '{}': the engine has no such array type", element_type);
        return nullptr;
      }

      const auto* node = self->View().GetArray();
      if (!node) {
        return array;
      }

      array->Resize(static_cast<AngelScript::asUINT>(node->size()));
      for (AngelScript::asUINT i = 0; i < array->GetSize(); ++i) {
        ReadElement<T>((*node)[i], *static_cast<T*>(array->At(i)));
      }

      return array;
    }

    /// The reverse: this node becomes an array of the elements, replacing whatever it was.
    template<typename T>
    void SetArray(ScriptValue* self, const AngelScript::CScriptArray* array) {
      auto& node = self->View().MakeArray();
      node.clear();

      if (!array) {
        return;
      }

      node.reserve(array->GetSize());
      for (AngelScript::asUINT i = 0; i < array->GetSize(); ++i) {
        node.push_back(ElementNode(*static_cast<const T*>(array->At(i))));
      }
    }

    void SetIntArray(ScriptValue* self, const AngelScript::CScriptArray* array) {
      SetArray<std::int32_t>(self, array);
    }

    void SetInt64Array(ScriptValue* self, const AngelScript::CScriptArray* array) {
      SetArray<std::int64_t>(self, array);
    }

    void SetFloatArray(ScriptValue* self, const AngelScript::CScriptArray* array) {
      SetArray<float>(self, array);
    }

    void SetDoubleArray(ScriptValue* self, const AngelScript::CScriptArray* array) {
      SetArray<double>(self, array);
    }

    void SetBoolArray(ScriptValue* self, const AngelScript::CScriptArray* array) {
      SetArray<bool>(self, array);
    }

    void SetStringArray(ScriptValue* self, const AngelScript::CScriptArray* array) {
      SetArray<std::string>(self, array);
    }

    AngelScript::CScriptArray* GetIntArray(ScriptValue* self) {
      return GetArray<std::int32_t>(self, "std::array<int>");
    }

    AngelScript::CScriptArray* GetInt64Array(ScriptValue* self) {
      return GetArray<std::int64_t>(self, "std::array<int64>");
    }

    AngelScript::CScriptArray* GetFloatArray(ScriptValue* self) {
      return GetArray<float>(self, "std::array<float>");
    }

    AngelScript::CScriptArray* GetDoubleArray(ScriptValue* self) {
      return GetArray<double>(self, "std::array<double>");
    }

    AngelScript::CScriptArray* GetBoolArray(ScriptValue* self) {
      return GetArray<bool>(self, "std::array<bool>");
    }

    AngelScript::CScriptArray* GetStringArray(ScriptValue* self) {
      return GetArray<std::string>(self, "std::array<std::string>");
    }

    /// The names of this node's members, in the order they were added - which is the order they will
    /// be written in, so a script can read a file's layout back out of a document.
    AngelScript::CScriptArray* GetKeys(ScriptValue* self) {
      auto* engine = CurrentEngine();
      if (!engine) {
        return nullptr;
      }

      auto* array = AngelScript::CScriptArray::Create(engine->GetTypeInfoByDecl("std::array<std::string>"));
      if (!array) {
        LOG_ERROR("[AS] Cannot make an array of keys: the engine has no std::array<std::string>");
        return nullptr;
      }

      const auto keys = self->View().Keys();
      array->Resize(static_cast<AngelScript::asUINT>(keys.size()));
      for (AngelScript::asUINT i = 0; i < array->GetSize(); ++i) {
        *static_cast<std::string*>(array->At(i)) = keys[i];
      }

      return array;
    }

    /// A script dictionary's contents, as this node's members.
    ///
    /// Only the leaf kinds a tree has are converted. A dictionary holding another dictionary or an
    /// array is not converted whole - AngelScript's dictionary stores those by handle and by element
    /// type, and a value that went through the conversion would come back as something else - so the
    /// key is left out and named in the log rather than written as a value that cannot be read back.
    void SetDictionary(ScriptValue* self, const AngelScript::CScriptDictionary* dictionary) {
      auto& node = self->View().MakeObject();
      node.clear();

      if (!dictionary) {
        return;
      }

      const auto* engine = CurrentEngine();
      if (!engine) {
        return;
      }

      for (auto it = dictionary->begin(); it != dictionary->end(); ++it) {
        const int type_id = it.GetTypeId();
        const void* value = it.GetAddressOfValue();
        auto& member = node[it.GetKey()];

        if (type_id == 0 || !value) {
          member = nullptr;
        } else if (type_id == AngelScript::asTYPEID_BOOL) {
          member = *static_cast<const bool*>(value);
        } else if (type_id == AngelScript::asTYPEID_FLOAT) {
          member = static_cast<double>(*static_cast<const float*>(value));
        } else if (type_id == AngelScript::asTYPEID_DOUBLE) {
          member = *static_cast<const double*>(value);
        } else if (IsString(engine, type_id)) {
          member = *static_cast<const std::string*>(value);
        } else if (double number{}; ReadPrimitive(value, type_id, engine, number)) {
          member = number;
        } else {
          LOG_ERROR("[AS] A dictionary member '{}' is of type '{}', which is not one a value can hold; it is left out",
                    it.GetKey(), engine->GetTypeDeclaration(type_id));
        }
      }
    }

    /// The reverse. A member that is itself an object or an array is left out for the same reason, and
    /// named in the log.
    AngelScript::CScriptDictionary* GetDictionary(ScriptValue* self) {
      auto* engine = CurrentEngine();
      if (!engine) {
        return nullptr;
      }

      auto* dictionary = AngelScript::CScriptDictionary::Create(engine);
      if (!dictionary) {
        return nullptr;
      }

      const auto* node = self->View().GetObject();
      if (!node) {
        return dictionary;
      }

      const int string_type = engine->GetTypeIdByDecl("std::string");
      for (const auto& member : *node) {
        if (member.second.is_string()) {
          auto text = member.second.get_string();
          dictionary->Set(member.first, &text, string_type);
        } else if (member.second.is_boolean()) {
          auto flag = member.second.get_boolean();
          dictionary->Set(member.first, &flag, AngelScript::asTYPEID_BOOL);
        } else if (member.second.is_number()) {
          dictionary->Set(member.first, member.second.get_number());
        } else {
          LOG_ERROR("[AS] '{}' is not a number, a string or a boolean, so a dictionary cannot hold it; it is left out",
                    member.first);
        }
      }

      return dictionary;
    }

    bool Save(ScriptValue* self, const std::string& file_name) {
      return self->View().Save(file_name, ValueFormat::kJSON);
    }

    bool SaveAs(ScriptValue* self, const std::string& file_name, const ValueFormat format) {
      return self->View().Save(file_name, format);
    }

    bool Load(ScriptValue* self, const std::string& file_name) {
      return self->View().Load(file_name, ValueFormat::kJSON);
    }

    bool LoadAs(ScriptValue* self, const std::string& file_name, const ValueFormat format) {
      return self->View().Load(file_name, format);
    }

    std::string Dump(ScriptValue* self) {
      return self->View().Dump(ValueFormat::kJSON);
    }

    std::string DumpAs(ScriptValue* self, const ValueFormat format) {
      return self->View().Dump(format);
    }

    bool Parse(ScriptValue* self, const std::string& text) {
      return self->View().Parse(text, ValueFormat::kJSON);
    }

    bool ParseAs(ScriptValue* self, const std::string& text, const ValueFormat format) {
      return self->View().Parse(text, format);
    }
  }

  ScriptValue::ScriptValue(GenericView view) : view_(std::move(view)) {
    ++live_instances;
  }

  ScriptValue::~ScriptValue() {
    --live_instances;
  }

  ScriptValue ScriptValue::Subscript(std::string key) const {
    return ScriptValue(view_.Key(std::move(key)));
  }

  ScriptValue ScriptValue::Subscript(const std::size_t index) const {
    return ScriptValue(view_.Index(index));
  }

  std::int64_t ScriptValue::LiveInstances() {
    return live_instances;
  }

  void RegisterValue(AngelScript::asIScriptEngine* engine) {
    // The whole of the API lives in a namespace of its own: `serialization::Value` says what it is and
    // where it came from, and the bare names `Value` and `ValueFormat` are left to a script that wants
    // them for itself. Every declaration below is written the way it is read from a script, so the
    // namespace is set once here rather than spelled into each one.
    engine->SetDefaultNamespace("serialization");

    // The format is an enum rather than a pair of SaveJson/SaveToml methods so that a script picks the
    // format at the point of the call, and so that an editor's completion offers the two of them.
    util::RegisterEnum(engine, "ValueFormat")
      .Desc("The format a value is written and read as. JSON holds any document; TOML holds one whose "
            "root is an object, which is what a document a script builds by subscripting is.");
    util::RegisterEnumValue(engine, "ValueFormat", "JSON", static_cast<int>(ValueFormat::kJSON));
    util::RegisterEnumValue(engine, "ValueFormat", "TOML", static_cast<int>(ValueFormat::kTOML));

    util::RegisterObjectType(engine, "Value", static_cast<int>(sizeof(ScriptValue)),
                             AngelScript::asOBJ_VALUE | AngelScript::asOBJ_APP_CLASS_CDK)
      .Desc("A document a script can hold structured data in and write to a file. A value is what "
            "subscripting one gives, and every value names a node of the same document, so writing "
            "through one is writing through all of them.");

    // Registered as the type's constructors and destructor rather than as a factory and a pair of
    // reference-counting behaviours, which is what makes `Value root;` an empty document and
    // `Value other = root[\"a\"];` a second name for one of its nodes. The copy constructor is the
    // reason a Value can be held in a script variable at all, and the destructor is what takes the
    // count down - there is nothing else to release a Value.
    util::RegisterObjectBehaviour(engine, "Value", AngelScript::asBEHAVE_CONSTRUCT, "void f()", AngelScript::asFUNCTION(DefaultConstruct),
                                  AngelScript::asCALL_CDECL_OBJLAST)
      .Desc("An empty document, to be filled in by subscripting and saved. Nothing is read from a file "
            "until Load is called, and a second Value is a second document that shares nothing with "
            "the first.");
    util::RegisterObjectBehaviour(engine, "Value", AngelScript::asBEHAVE_CONSTRUCT, "void f(const Value &in)", AngelScript::asFUNCTION(CopyConstruct),
                                  AngelScript::asCALL_CDECL_OBJLAST)
      .Desc("Another name for the node the other value names, in the same document - so writing "
            "through either is writing through both. A copy is a name, not a copy of the document.")
      .Param("other", "The value to name the same node as.");
    util::RegisterObjectBehaviour(engine, "Value", AngelScript::asBEHAVE_DESTRUCT, "void f()", AngelScript::asFUNCTION(Destruct),
                                  AngelScript::asCALL_CDECL_OBJLAST);

    util::RegisterObjectMethod(engine, "Value", "Value opIndex(const std::string &in) const", AngelScript::asFUNCTION(SubscriptKey),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The member `name` of this value, as a value of its own, which may be written to or "
            "subscripted further - `root[\"a\"][\"b\"] = 1` makes both as it goes. Reading one that is "
            "not there gives a value that answers every read with its default and every write by "
            "making the member.")
      .Param("name", "The member to name. A name with a '.' in it is an ordinary name, not a path: "
                     "nesting is written as a second subscript.")
      .Returns("A value naming that member. Writing to it writes to this one.");
    util::RegisterObjectMethod(engine, "Value", "Value opIndex(uint) const", AngelScript::asFUNCTION(SubscriptIndex),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The element at `index` of the array this value is, as a value of its own. A value that is "
            "not an array becomes one when the element is written, and an index past the end grows the "
            "array with as many nulls as it takes to reach it.")
      .Param("index", "The element to name, counted from zero.")
      .Returns("A value naming that element. Writing to it writes to this one.");

    util::RegisterObjectMethod(engine, "Value", "void opAssign(const ?&in)", AngelScript::asFUNCTION(Assign),
                               AngelScript::asCALL_CDECL_OBJLAST)
      .Desc("Sets this value to what was assigned. A number, a boolean and a string are stored as "
            "themselves and make this value that kind of node; assigning null empties it. Assigning "
            "anything else - a handle, or an array or dictionary whole - is refused and logged: write "
            "the members instead, or use the SetDictionary and Set...Array methods.")
      .Param("value", "What to store. A number is stored as the tree's one number type, so a value past "
                      "2^53 does not come back exact; everything below it does.")
      .Returns("Nothing.");

    util::RegisterObjectMethod(engine, "Value", "bool GetBool()", AngelScript::asFUNCTION(GetBool), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("This value as a boolean. False for a value that is of another kind.")
      .Returns("The stored boolean, or false.");
    util::RegisterObjectMethod(engine, "Value", "int GetInt()", AngelScript::asFUNCTION(GetInt), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("This value as an integer, rounded to the nearest one rather than towards zero. Zero for a "
            "value that is not a number.")
      .Returns("The stored number as an int, or zero.");
    util::RegisterObjectMethod(engine, "Value", "int64 GetInt64()", AngelScript::asFUNCTION(GetInt64), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("This value as a 64-bit integer, rounded to the nearest one. Zero for a value that is not a "
            "number.")
      .Returns("The stored number as an int64, or zero.");
    util::RegisterObjectMethod(engine, "Value", "float GetFloat()", AngelScript::asFUNCTION(GetFloat), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("This value as a float. Zero for a value that is not a number.")
      .Returns("The stored number as a float, or zero.");
    util::RegisterObjectMethod(engine, "Value", "double GetDouble()", AngelScript::asFUNCTION(GetDouble), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("This value as a double, which is the type every number is stored as.")
      .Returns("The stored number, or zero.");
    util::RegisterObjectMethod(engine, "Value", "std::string GetString()", AngelScript::asFUNCTION(GetString), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("This value as a string. Empty for a value that is not a string.")
      .Returns("The stored string, or an empty one.");

    util::RegisterObjectMethod(engine, "Value", "bool IsNull()", AngelScript::asFUNCTION(IsNull), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Whether there is nothing here: the member is not there at all, or holds null. Exactly one "
            "of the six Is... methods is true of any value.")
      .Returns("True when this value holds nothing.");
    util::RegisterObjectMethod(engine, "Value", "bool IsObject()", AngelScript::asFUNCTION(IsObject), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Whether this value has members rather than being a number, a string, a boolean or an array.")
      .Returns("True when this value is an object.");
    util::RegisterObjectMethod(engine, "Value", "bool IsArray()", AngelScript::asFUNCTION(IsArray), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Whether this value is an array, which is what its elements being written to it one index at "
            "a time makes it.")
      .Returns("True when this value is an array.");
    util::RegisterObjectMethod(engine, "Value", "bool IsNumber()", AngelScript::asFUNCTION(IsNumber), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Whether this value is a number.")
      .Returns("True when this value is a number.");
    util::RegisterObjectMethod(engine, "Value", "bool IsBool()", AngelScript::asFUNCTION(IsBool), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Whether this value is a boolean.")
      .Returns("True when this value is a boolean.");
    util::RegisterObjectMethod(engine, "Value", "bool IsString()", AngelScript::asFUNCTION(IsStringValue), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Whether this value is a string.")
      .Returns("True when this value is a string.");

    util::RegisterObjectMethod(engine, "Value", "bool Contains(const std::string &in)", AngelScript::asFUNCTION(Contains),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Whether this value has a member of that name. False for a value that is not an object, so "
            "a script can ask before subscripting.")
      .Param("name", "The member to look for.")
      .Returns("True when the member is there.");
    util::RegisterObjectMethod(engine, "Value", "uint GetSize()", AngelScript::asFUNCTION(GetSize), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("How many members or elements this value has, or how many characters it holds when it is a "
            "string. Zero for a number, a boolean or null.")
      .Returns("The number of members, elements or characters.");
    util::RegisterObjectMethod(engine, "Value", "bool Remove(const std::string &in)", AngelScript::asFUNCTION(RemoveKey),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Removes a member of this value.")
      .Param("name", "The member to remove.")
      .Returns("True when there was one to remove.");
    util::RegisterObjectMethod(engine, "Value", "bool Remove(uint)", AngelScript::asFUNCTION(RemoveIndex),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Removes an element of this value, moving the elements after it up one.")
      .Param("index", "The element to remove, counted from zero.")
      .Returns("True when there was one to remove.");

    util::RegisterObjectMethod(engine, "Value", "void SetDictionary(const std::dictionary &in)", AngelScript::asFUNCTION(SetDictionary),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Makes this value an object holding the dictionary's members, replacing what was there. "
            "Only numbers, booleans and strings are converted; a member that is itself a dictionary or "
            "an array is left out and logged.")
      .Param("dictionary", "The dictionary to copy.")
      .Returns("Nothing.");
    util::RegisterObjectMethod(engine, "Value", "std::dictionary@ GetDictionary()", AngelScript::asFUNCTION(GetDictionary),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("This value's members as a dictionary, in the order they were added. An empty dictionary "
            "for a value that is not an object, and members that are objects or arrays are left out.")
      .Returns("A new dictionary holding this value's members.");

    util::RegisterObjectMethod(engine, "Value", "bool Save(const std::string &in)", AngelScript::asFUNCTION(Save),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Writes this value to a file of the script's own data directory, as JSON.")
      .Param("file", "The file to write, relative to this script's data directory. A name that "
                     "resolves outside it - by starting with '..', or by being absolute - is refused "
                     "and nothing is written. Subdirectories are made as they are needed.")
      .Returns("True when the file was written.");
    util::RegisterObjectMethod(engine, "Value", "bool Save(const std::string &in, ValueFormat)", AngelScript::asFUNCTION(SaveAs),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, in the format named. Saving a value that is not an object as TOML is refused: a "
            "TOML file is a table, so a document that is a bare number has no TOML to be written as.")
      .Param("format", "JSON or TOML.")
      .Returns("True when the file was written.");
    util::RegisterObjectMethod(engine, "Value", "bool Load(const std::string &in)", AngelScript::asFUNCTION(Load),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Reads a JSON file of the script's own data directory into this value, replacing it. A file "
            "that cannot be read or does not parse leaves this value as it was.")
      .Param("file", "The file to read, relative to this script's data directory.")
      .Returns("True when the file was read.");
    util::RegisterObjectMethod(engine, "Value", "bool Load(const std::string &in, ValueFormat)", AngelScript::asFUNCTION(LoadAs),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, in the format named.")
      .Param("format", "JSON or TOML.")
      .Returns("True when the file was read.");
    util::RegisterObjectMethod(engine, "Value", "std::string Dump()", AngelScript::asFUNCTION(Dump), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("This value as JSON text, for handing to something that takes a string rather than a file.")
      .Returns("The text, or an empty string when this value has none.");
    util::RegisterObjectMethod(engine, "Value", "std::string Dump(ValueFormat)", AngelScript::asFUNCTION(DumpAs),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, in the format named.")
      .Param("format", "JSON or TOML.")
      .Returns("The text, or an empty string when this value has none.");
    util::RegisterObjectMethod(engine, "Value", "bool Parse(const std::string &in)", AngelScript::asFUNCTION(Parse),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Reads JSON text into this value, replacing it - the reverse of Dump. Text that does not "
            "parse leaves this value as it was.")
      .Param("text", "The document to read.")
      .Returns("True when the text was read.");
    util::RegisterObjectMethod(engine, "Value", "bool Parse(const std::string &in, ValueFormat)", AngelScript::asFUNCTION(ParseAs),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, in the format named.")
      .Param("format", "JSON or TOML.")
      .Returns("True when the text was read.");

    // The array conversions are one pair of methods per element type rather than one pair taking a
    // `T`, because AngelScript has no way to overload on an array's element type - and a script that
    // would rather not convert at all can reach the same elements through subscripts, array<int> or
    // otherwise.
    util::RegisterObjectMethod(engine, "Value", "void SetIntArray(const std::array<int> &in)", AngelScript::asFUNCTION(SetIntArray),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Makes this value an array of the array's elements, replacing what was there.")
      .Param("array", "The elements to copy, in order.")
      .Returns("Nothing.");
    util::RegisterObjectMethod(engine, "Value", "void SetInt64Array(const std::array<int64> &in)", AngelScript::asFUNCTION(SetInt64Array),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, for an array of int64. The elements are stored as the tree's one number type.")
      .Param("array", "The elements to copy, in order.")
      .Returns("Nothing.");
    util::RegisterObjectMethod(engine, "Value", "void SetFloatArray(const std::array<float> &in)", AngelScript::asFUNCTION(SetFloatArray),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, for an array of float.")
      .Param("array", "The elements to copy, in order.")
      .Returns("Nothing.");
    util::RegisterObjectMethod(engine, "Value", "void SetDoubleArray(const std::array<double> &in)", AngelScript::asFUNCTION(SetDoubleArray),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, for an array of double.")
      .Param("array", "The elements to copy, in order.")
      .Returns("Nothing.");
    util::RegisterObjectMethod(engine, "Value", "void SetBoolArray(const std::array<bool> &in)", AngelScript::asFUNCTION(SetBoolArray),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, for an array of bool.")
      .Param("array", "The elements to copy, in order.")
      .Returns("Nothing.");
    util::RegisterObjectMethod(engine, "Value", "void SetStringArray(const std::array<std::string> &in)", AngelScript::asFUNCTION(SetStringArray),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, for an array of strings.")
      .Param("array", "The elements to copy, in order.")
      .Returns("Nothing.");

    util::RegisterObjectMethod(engine, "Value", "std::array<int>@ GetIntArray()", AngelScript::asFUNCTION(GetIntArray),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("This value's elements as an array of int, rounded to the nearest whole number. An empty "
            "array for a value that is not an array, and an element of another kind reads as zero.")
      .Returns("A new array of the elements.");
    util::RegisterObjectMethod(engine, "Value", "std::array<int64>@ GetInt64Array()", AngelScript::asFUNCTION(GetInt64Array),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, as an array of int64.")
      .Returns("A new array of the elements.");
    util::RegisterObjectMethod(engine, "Value", "std::array<float>@ GetFloatArray()", AngelScript::asFUNCTION(GetFloatArray),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, as an array of float.")
      .Returns("A new array of the elements.");
    util::RegisterObjectMethod(engine, "Value", "std::array<double>@ GetDoubleArray()", AngelScript::asFUNCTION(GetDoubleArray),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, as an array of double, which is the type the elements are stored as.")
      .Returns("A new array of the elements.");
    util::RegisterObjectMethod(engine, "Value", "std::array<bool>@ GetBoolArray()", AngelScript::asFUNCTION(GetBoolArray),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, as an array of bool. An element of another kind reads as false.")
      .Returns("A new array of the elements.");
    util::RegisterObjectMethod(engine, "Value", "std::array<std::string>@ GetStringArray()", AngelScript::asFUNCTION(GetStringArray),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The same, as an array of strings. An element of another kind reads as an empty string.")
      .Returns("A new array of the elements.");
    util::RegisterObjectMethod(engine, "Value", "std::array<std::string>@ GetKeys()", AngelScript::asFUNCTION(GetKeys),
                               AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("The names of this value's members, in the order they were added - which is the order they "
            "are written in, so reading a file back gives its layout back. Empty for a value that is "
            "not an object.")
      .Returns("A new array of the member names.");

    engine->SetDefaultNamespace("");
  }
}
