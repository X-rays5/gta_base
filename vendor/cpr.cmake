# cpr fetches and builds curl itself. Force the Schannel backend so OpenSSL is never
# configured or built; Schannel also uses the Windows certificate store, so no CA bundle
# needs to be shipped. These are set before CPMAddPackage so cpr's own option() calls see
# the cache entries and keep working regardless of which module includes this recipe.
set(CPR_FORCE_WINSSL_BACKEND ON CACHE BOOL "" FORCE)
set(CPR_CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(CPR_BUILD_TESTS OFF CACHE BOOL "" FORCE)

CPMAddPackage("gh:libcpr/cpr#1.14.2")

if (TARGET ${PROJECT_NAME}_core)
    target_link_libraries(${PROJECT_NAME}_core PRIVATE cpr::cpr)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)
endif ()
