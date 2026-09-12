//
// Created by X-ray on 11/09/2026.
//

#pragma once

#include <angelscript.h>

#include <enchantum/enchantum.hpp>

#include <array>
#include <cstddef>
#include <format>
#include <initializer_list>
#include <new>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace base::menu::as::util {
  struct ParamDoc {
    std::string name;
    std::string description;
  };

  /**
   * Documentation for a single script binding, authored at the registration site.
   */
  struct Doc {
    std::string description;
    std::vector<ParamDoc> params;
    std::string returns;
  };

  /**
   * Whether anything was written for a binding. Every registration creates a registry entry for the
   * fluent calls after it to write into, so an entry that is still empty is a binding nobody documented
   * rather than documentation that lost its binding - which is the difference the generator's report of
   * unmatched docs turns on.
   */
  [[nodiscard]] bool IsDocumented(const Doc& doc);

  /**
   * Fluent view over the Doc stored for a binding. The underlying reference stays valid for the
   * lifetime of the registry entry, so it may be held past the registration call that produced it.
   */
  class DocBuilder {
  public:
    explicit DocBuilder(Doc& doc) : doc_(&doc) {}

    DocBuilder& Desc(const std::string_view text) {
      doc_->description.assign(text);
      return *this;
    }

    DocBuilder& Param(const std::string_view name, const std::string_view description) {
      doc_->params.push_back(ParamDoc{std::string{name}, std::string{description}});
      return *this;
    }

    DocBuilder& Returns(const std::string_view text) {
      doc_->returns.assign(text);
      return *this;
    }

  private:
    Doc* doc_;
  };

  /**
   * Docs are keyed by symbol name rather than by signature, so a single doc covers every overload of
   * a name (log::info has both a fixed and a variadic overload that read the same).
   *   global function/property  ns::name
   *   object type               ns::Type
   *   method/object property    ns::Type::member
   *   enum/enum value           ns::Enum, ns::Enum::value
   *   typedef/funcdef           ns::Name
   */
  std::unordered_map<std::string, Doc>& GetDocRegistry();

  /**
   * Look up the docs registered for a key, or nullptr when nothing was registered.
   */
  const Doc* FindDoc(std::string_view key);

  /**
   * Every registered doc, used to report entries that never matched anything in the engine.
   */
  const std::unordered_map<std::string, Doc>& GetAllDocs();

  /**
   * Drop every registered doc. The registry outlives a single engine, so a second registration pass
   * in the same process would otherwise append a duplicate of every parameter to the doc it wrote
   * the first time. Call this before re-registering, while no DocBuilder is still alive.
   */
  void ClearDocs();

  /**
   * Render a javadoc-style comment block, every line prefixed with `indent`. Returns an empty string
   * when there is nothing to document.
   */
  std::string FormatDocComment(const Doc& doc, std::string_view indent);

  /**
   * The trailing symbol name of a declaration: "void log::info(const string &in)" -> "log::info",
   * "int m_value" -> "m_value".
   */
  std::string SymbolPathFromDecl(std::string_view decl);

  /**
   * Prefix `name` with `defaultNamespace` unless it already carries a namespace of its own.
   */
  std::string QualifyNamespace(std::string_view name, std::string_view defaultNamespace);

  // Registration helpers mirroring asIScriptEngine, which additionally capture docs for the binding.
  // The generator keys docs off symbol names, so only one overload of a name needs documenting.

  DocBuilder RegisterGlobalFunction(AngelScript::asIScriptEngine* engine, const char* declaration,
                                    const AngelScript::asSFuncPtr& funcPointer, AngelScript::asDWORD callConv,
                                    void* auxiliary = nullptr);

  DocBuilder RegisterGlobalProperty(AngelScript::asIScriptEngine* engine, const char* declaration, void* pointer);

  DocBuilder RegisterObjectType(AngelScript::asIScriptEngine* engine, const char* obj, int byteSize,
                               AngelScript::asQWORD flags);

  DocBuilder RegisterObjectMethod(AngelScript::asIScriptEngine* engine, const char* obj, const char* declaration,
                                 const AngelScript::asSFuncPtr& funcPointer, AngelScript::asDWORD callConv,
                                 void* auxiliary = nullptr, int compositeOffset = 0, bool isCompositeIndirect = false);

  DocBuilder RegisterObjectProperty(AngelScript::asIScriptEngine* engine, const char* obj, const char* declaration,
                                   int byteOffset, int compositeOffset = 0, bool isCompositeIndirect = false);

  DocBuilder RegisterObjectBehaviour(AngelScript::asIScriptEngine* engine, const char* obj,
                                    AngelScript::asEBehaviours behaviour, const char* declaration,
                                    const AngelScript::asSFuncPtr& funcPointer, AngelScript::asDWORD callConv,
                                    void* auxiliary = nullptr, int compositeOffset = 0, bool isCompositeIndirect = false);

  DocBuilder RegisterEnum(AngelScript::asIScriptEngine* engine, const char* type);

  DocBuilder RegisterEnumValue(AngelScript::asIScriptEngine* engine, const char* type, const char* name, int value);

  template<typename T> requires std::is_enum_v<T>
  DocBuilder RegisterEnum(AngelScript::asIScriptEngine* engine, const std::string& type_name) {
    const DocBuilder builder = RegisterEnum(engine, type_name.c_str());
    for (T value : enchantum::values_generator<T>) {
      RegisterEnumValue(engine, type_name.c_str(), enchantum::to_string(value).data(), static_cast<int>(value));
    }

    return builder;
  }

  /**
   * Register `T` under its own leaf name. enchantum::type_name keeps only what follows the last "::",
   * so a nested enum derives the same name as every other enum with that leaf - pass an explicit name
   * to the overload above whenever two would collide (ui::notification::Type and script::ScriptBase::Type
   * are both "Type").
   */
  template<typename T> requires std::is_enum_v<T>
  DocBuilder RegisterEnum(AngelScript::asIScriptEngine* engine) {
    return RegisterEnum<T>(engine, std::string{enchantum::type_name<T>});
  }

  DocBuilder RegisterFuncdef(AngelScript::asIScriptEngine* engine, const char* declaration);

  DocBuilder RegisterTypedef(AngelScript::asIScriptEngine* engine, const char* type, const char* declaration);

  /**
   * The AngelScript spelling of a C++ type, with cv and ref stripped. The default comes from enchantum,
   * so rage::Vector3 spells "Vector3" and needs no entry here; the specialisations below are only the
   * cases where the AngelScript name genuinely differs from the C++ leaf name.
   */
  namespace detail {
    template<typename T>
    struct AsTypeName {
      static constexpr std::string_view value = enchantum::type_name<T>;
    };

    template<> struct AsTypeName<void>               { static constexpr std::string_view value = "void"; };
    template<> struct AsTypeName<bool>               { static constexpr std::string_view value = "bool"; };
    template<> struct AsTypeName<char>               { static constexpr std::string_view value = "int8"; };
    template<> struct AsTypeName<signed char>        { static constexpr std::string_view value = "int8"; };
    template<> struct AsTypeName<unsigned char>      { static constexpr std::string_view value = "uint8"; };
    template<> struct AsTypeName<short>              { static constexpr std::string_view value = "int16"; };
    template<> struct AsTypeName<unsigned short>     { static constexpr std::string_view value = "uint16"; };
    template<> struct AsTypeName<int>                { static constexpr std::string_view value = "int"; };
    template<> struct AsTypeName<unsigned int>       { static constexpr std::string_view value = "uint"; };
    template<> struct AsTypeName<long>               { static constexpr std::string_view value = "int"; };
    template<> struct AsTypeName<unsigned long>      { static constexpr std::string_view value = "uint"; };
    template<> struct AsTypeName<long long>          { static constexpr std::string_view value = "int64"; };
    template<> struct AsTypeName<unsigned long long> { static constexpr std::string_view value = "uint64"; };
    template<> struct AsTypeName<float>              { static constexpr std::string_view value = "float"; };
    template<> struct AsTypeName<double>             { static constexpr std::string_view value = "double"; };

    // std::basic_string<> is a templated type, which enchantum::type_name refuses to name. The
    // add-ons register it inside namespace std, so the qualified spelling is the one that resolves.
    template<> struct AsTypeName<std::string> { static constexpr std::string_view value = "std::string"; };
  }

  template<typename T>
  inline constexpr std::string_view kAsTypeName = detail::AsTypeName<std::remove_cvref_t<T>>::value;

  /**
   * The one scalar shape that cannot be spelled as a value: a non-const reference is how a member
   * writes back to its caller, and a by-value declaration would drop the write silently. Everything
   * else that rides in a register is passed as a value, const references included - AngelScript has
   * no spelling for a const scalar reference, and none is needed, since the callee cannot write
   * through one.
   */
  template<typename A>
  inline constexpr bool kWritableScalarReference =
    std::is_lvalue_reference_v<A> && !std::is_const_v<std::remove_reference_t<A>> &&
    (std::is_arithmetic_v<std::remove_cvref_t<A>> || std::is_enum_v<std::remove_cvref_t<A>>);

  /**
   * The type a declaration spelling makes AngelScript pass, which is what the thunk has to receive.
   * A scalar rides in a register, so a member taking it by const reference is called with the value,
   * and a handle is a plain pointer; a class arrives as a pointer to the object, so it stays a
   * reference.
   */
  template<typename A>
  using ThunkParam = std::conditional_t<
    !kWritableScalarReference<A> &&
      (std::is_arithmetic_v<std::remove_cvref_t<A>> || std::is_enum_v<std::remove_cvref_t<A>> ||
       std::is_pointer_v<std::remove_cvref_t<A>>),
    std::remove_cvref_t<A>, A>;

  namespace detail {
    /**
     * The type and mode half of one parameter declaration: "float" for a scalar, "int &out" for a
     * reference a member writes through, "const Vector3 &in" for a const reference, "NativeHandle@"
     * for a pointer.
     */
    template<typename A>
    std::string ParamType() {
      using Bare = std::remove_cvref_t<A>;
      if constexpr (kWritableScalarReference<A>) {
        return std::format("{} &out", kAsTypeName<Bare>);
      } else if constexpr (std::is_arithmetic_v<Bare> || std::is_enum_v<Bare>) {
        return std::string{kAsTypeName<Bare>};
      } else if constexpr (std::is_pointer_v<Bare>) {
        static_assert(!std::is_same_v<std::remove_cv_t<std::remove_pointer_t<Bare>>, char>,
                      "A char pointer is a string in AngelScript; register that member by hand.");
        return std::format("{}@", kAsTypeName<std::remove_pointer_t<Bare>>);
      } else {
        static_assert(std::is_reference_v<A>,
                      "A by-value class parameter has no AngelScript spelling; declare it as a reference.");
        constexpr bool kConstRef = std::is_const_v<std::remove_reference_t<A>>;
        return std::format("{} {}{}", kConstRef ? "const " : "", kAsTypeName<Bare>, kConstRef ? " &in" : " &inout");
      }
    }

    /// The parameter list of a declaration. `names` is optional and positional; a parameter left out
    /// of it is simply unnamed.
    template<typename... A>
    std::string ParamList(const std::initializer_list<std::string_view> names) {
      const std::array<std::string, sizeof...(A)> types{ParamType<A>()...};

      std::string out;
      for (std::size_t i = 0; i < types.size(); ++i) {
        if (i > 0) out += ", ";
        out += types[i];
        if (i < names.size()) {
          out += ' ';
          out += names.begin()[i];
        }
      }
      return out;
    }

    /// Placement new with the object last, the shape asBEHAVE_CONSTRUCT with asCALL_CDECL_OBJLAST wants.
    template<typename T, typename... Args>
    void PlacementNew(Args... args, T* self) {
      static_assert(std::is_constructible_v<T, Args...>,
                    "RegisterConstructor cannot build a T from these arguments; pass a factory instead.");
      new (self) T(std::forward<Args>(args)...);
    }

    /// Decomposed by type rather than by value: a pack followed by another template parameter is not
    /// something a class template may declare, so the pointer is matched through its signature.
    ///
    /// Registering a member this cannot decompose is a compile error rather than a bad declaration,
    /// which is the whole point - but say so, because the default diagnostic is "incomplete type".
    template<typename M> struct MethodTraits {
      static_assert(!std::is_same_v<M, M>,
                    "RegisterMethod needs a plain non-static member function. A noexcept, ref-qualified or "
                    "C-varargs member, a member template, or a data member has no declaration to deduce; "
                    "register it by hand.");
    };

    /// A declared return type has to be a value: kAsTypeName strips cv and ref, so a member returning
    /// const T& or T* would be declared as returning T by value while the thunk hands AngelScript a
    /// pointer. An enum is 4 bytes in AngelScript whatever its underlying type, and AS stores the whole
    /// register a narrow enum comes back in, upper bits and all.
    template<typename R>
    constexpr void CheckReturnType() {
      static_assert(!std::is_reference_v<R> && !std::is_pointer_v<R> && !std::is_array_v<R>,
                    "RegisterMethod cannot deduce a reference, pointer or array return; the declaration would "
                    "have to spell it and the thunk would hand AngelScript a pointer. Register that member by hand.");
      if constexpr (std::is_enum_v<R>) static_assert(sizeof(R) == 4, "AngelScript enums are 4 bytes.");
    }

    template<typename R, typename T, typename... A>
    struct MethodTraits<R (T::*)(A...)> {
      template<R (T::*Method)(A...)>
      static R Call(T& self, ThunkParam<A>... args) { return (self.*Method)(args...); }

      static std::string Declare(const std::string_view name, const std::initializer_list<std::string_view> names) {
        CheckReturnType<R>();
        return std::format("{} {}({})", kAsTypeName<R>, name, ParamList<A...>(names));
      }
    };

    template<typename R, typename T, typename... A>
    struct MethodTraits<R (T::*)(A...) const> {
      template<R (T::*Method)(A...) const>
      static R Call(const T& self, ThunkParam<A>... args) { return (self.*Method)(args...); }

      static std::string Declare(const std::string_view name, const std::initializer_list<std::string_view> names) {
        CheckReturnType<R>();
        return std::format("{} {}({}) const", kAsTypeName<R>, name, ParamList<A...>(names));
      }
    };

    template<typename M> struct MemberOwner;

    template<typename V, typename O>
    struct MemberOwner<V O::*> {
      using Type = O;
    };

    /// A member pointer is only a plain byte offset for a type whose layout the compiler expresses that
    /// way, so the object is synthesised in a local buffer rather than dereferenced through a null one.
    template<auto Member>
    int MemberOffset() {
      using Owner = typename MemberOwner<decltype(Member)>::Type;
      static_assert(std::is_standard_layout_v<Owner>,
                    "RegisterProperty needs a standard layout type: a member pointer is not an offset otherwise.");
      alignas(Owner) std::byte storage[sizeof(Owner)];
      const auto* const owner = reinterpret_cast<const Owner*>(storage);
      return static_cast<int>(reinterpret_cast<const std::byte*>(&(owner->*Member)) - storage);
    }
  }

  /**
   * Register a member function without spelling its signature. The declaration and the thunk both come
   * from `Method`, so they cannot disagree about how an argument is passed.
   *
   * `asName` is the AngelScript name, which is only the C++ name when the member is not an operator.
   * `paramNames` is optional and positional; naming a parameter is what makes @param docs land on it.
   *
   * Always registered asCALL_CDECL_OBJFIRST. The thiscall path on x64 MSVC hands a method returning a
   * value type wider than a register the object pointer and the hidden return pointer in the opposite
   * order to MSVC's own convention, so the thunk would read `this` out of the return buffer.
   *
   * An overloaded member needs an explicit cast at the call site, which is also what makes the two
   * overloads agree: RegisterMethod<static_cast<V (T::*)(const float&) const>(&T::operator*)>(...).
   */
  template<auto Method>
  DocBuilder RegisterMethod(AngelScript::asIScriptEngine* engine, const char* object, const std::string_view asName,
                            const std::initializer_list<std::string_view> paramNames = {}) {
    using Traits = detail::MethodTraits<std::remove_cvref_t<decltype(Method)>>;
    const std::string declaration = Traits::Declare(asName, paramNames);
    return RegisterObjectMethod(engine, object, declaration.c_str(),
                                AngelScript::asFunctionPtr(&Traits::template Call<Method>),
                                AngelScript::asCALL_CDECL_OBJFIRST);
  }

  /**
   * Register a value type, deriving the byte size and the type traits. `asGetTypeTraits` already tells
   * AngelScript whether the ABI is a primitive or a class and which of its members the type provides,
   * so the flags are OR-ed onto the fixed pair rather than repeated; pass extras only to add to them.
   */
  template<typename T>
  DocBuilder RegisterValueType(AngelScript::asIScriptEngine* engine, const char* name,
                               const AngelScript::asQWORD extraFlags = 0) {
    // asGetTypeTraits is not constexpr, so a type it has no answer for - a union, say - registers
    // without any asOBJ_APP_ flag rather than failing here. Register those by hand.
    return RegisterObjectType(engine, name, static_cast<int>(sizeof(T)),
                              AngelScript::asOBJ_VALUE | AngelScript::asOBJ_POD |
                                AngelScript::asGetTypeTraits<T>() | extraFlags);
  }

  /**
   * Register a constructor from the argument types alone, generating the placement-new thunk that
   * built-by-hand versions used to spell out. Missing when T is not constructible from Args.
   */
  template<typename T, typename... Args>
  DocBuilder RegisterConstructor(AngelScript::asIScriptEngine* engine, const char* typeName,
                                 const std::initializer_list<std::string_view> paramNames = {}) {
    const std::string declaration = std::format("void f({})", detail::ParamList<Args...>(paramNames));
    return RegisterObjectBehaviour(engine, typeName, AngelScript::asBEHAVE_CONSTRUCT, declaration.c_str(),
                                   AngelScript::asFunctionPtr(&detail::PlacementNew<T, Args...>),
                                   AngelScript::asCALL_CDECL_OBJLAST);
  }

  /**
   * Register a constructor whose thunk is a hand-written factory, for the cases where the arguments do
   * not map onto a matching C++ constructor - an argument conversion, say. The declaration still comes
   * from `Args`, so only the body is written out.
   */
  template<typename T, typename... Args>
  DocBuilder RegisterConstructor(AngelScript::asIScriptEngine* engine, const char* typeName, void (*factory)(Args..., T*),
                                 const std::initializer_list<std::string_view> paramNames = {}) {
    const std::string declaration = std::format("void f({})", detail::ParamList<Args...>(paramNames));
    return RegisterObjectBehaviour(engine, typeName, AngelScript::asBEHAVE_CONSTRUCT, declaration.c_str(),
                                   AngelScript::asFunctionPtr(factory), AngelScript::asCALL_CDECL_OBJLAST);
  }

  /**
   * Register a data member, deriving the declared type and the byte offset from the member pointer.
   */
  template<auto Member>
  DocBuilder RegisterProperty(AngelScript::asIScriptEngine* engine, const char* object, const std::string_view memberName) {
    using Value = std::remove_cvref_t<decltype(std::declval<typename detail::MemberOwner<decltype(Member)>::Type>().*Member)>;
    static_assert(!std::is_array_v<Value>,
                  "RegisterProperty cannot spell an array member; register it by hand.");
    const std::string declaration = std::format("{} {}", kAsTypeName<Value>, memberName);
    return RegisterObjectProperty(engine, object, declaration.c_str(), detail::MemberOffset<Member>());
  }
}
