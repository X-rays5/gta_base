cmake_minimum_required(VERSION 3.16)
project(imfont LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS src/*.hpp)

# Copy headers to the include directory
foreach(header ${HEADERS})
    file(RELATIVE_PATH rel_path ${CMAKE_CURRENT_SOURCE_DIR}/src ${header})
    set(dest ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/${rel_path})
    configure_file(${header} ${dest} COPYONLY)
endforeach()

add_library(${PROJECT_NAME} STATIC
    src/IconsFontAwesome6.hpp
    src/imfont.hpp
    src/imfont.cpp
)
add_library(base::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE base::common)
include(vendor.cmake)

# We make sure the headers are updated every build
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME})
foreach(header ${HEADERS})
  file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}/src" ${header})
  get_filename_component(dir_path ${rel_path} DIRECTORY)

  # Create directory if needed
  if(dir_path)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/${dir_path})
  endif()

  # Single copy command for all cases
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${header}
      ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/${rel_path}
    COMMENT "Copying ${rel_path} to include directory"
  )
endforeach()

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)

b_embed(${PROJECT_NAME} assets/fonts/fa-solid-900.ttf)
b_embed(${PROJECT_NAME} assets/fonts/RobotoMono-Regular.ttf)