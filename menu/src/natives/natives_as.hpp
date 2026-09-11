#pragma once

#pragma warning(push)
#pragma warning(disable: 4100)

// Auto-generated file - DO NOT EDIT
// AngelScript native registration

#include <angelscript.h>

namespace natives {

	// The namespace the native bindings are registered under: one lowercased sub-namespace
	// per NativeDB group, with PascalCase native names.
	inline constexpr const char* kNamespace = "natives";

	// Registers every native that is present in the crossmap. The custom types
	// (Vector2/3/4, Hash, NativeHandle) must already be registered by the caller.
	void RegisterAngelScript(AngelScript::asIScriptEngine* engine);

} // namespace natives

#pragma warning(pop)
