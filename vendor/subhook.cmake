set(SUBHOOK_STATIC ON)
set(SUBHOOK_TESTS OFF)

cpmaddpackage("gh:Zeex/subhook#e935959d2f9cc642bcbb5e7759b2b1e7196b0947")

target_link_libraries(${PROJECT_NAME} PRIVATE subhook::subhook)