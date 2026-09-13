# AngelScript is vendored here rather than in menu/ because this component is what owns the engine
# build: the JIT is driven by the engine's own compile hook, so asjit needs the language headers, the
# add-ons and the engine library in its own right. It re-exports all three, which is how menu_core
# goes on seeing them after menu/vendor.cmake stops including this.
#
# Must stay the only place in the project that includes angelscript.cmake: the file adds a
# subdirectory, and a second include of it cannot work.
include(../vendor/angelscript.cmake)

# AsmJit stays PRIVATE to this target. menu_core already carries PolyHook's AsmJit 1.17 on its
# include path as `asmjit/...`, so a second AsmJit include root reaching menu_core would make those
# includes ambiguous - and this component's public header therefore does not mention AsmJit at all.
include(../vendor/asmjit.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE asjit::asmjit)
