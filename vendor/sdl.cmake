# disable all but renderer
set(SDL_AUDIO OFF CACHE BOOL "Disable Audio subsystem in SDL3" FORCE)
set(SDL_CAMERA OFF CACHE BOOL "Disable Camera subsystem in SDL3" FORCE)
set(SDL_JOYSTICK OFF CACHE BOOL "Disable Joystick subsystem in SDL3" FORCE)
set(SDL_HAPTIC OFF CACHE BOOL "Disable Haptic subsystem in SDL3" FORCE)
set(SDL_HIDAPI OFF CACHE BOOL "Disable HIDAPI subsystem in SDL3" FORCE)
set(SDL_POWER OFF CACHE BOOL "Disable Power subsystem in SDL3" FORCE)
set(SDL_SENSOR OFF CACHE BOOL "Disable Sensor subsystem in SDL3" FORCE)
set(SDL_DIALOG OFF CACHE BOOL "Disable Dialog subsystem in SDL3" FORCE)

CPMAddPackage("gh:libsdl-org/SDL#release-3.2.4")

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)