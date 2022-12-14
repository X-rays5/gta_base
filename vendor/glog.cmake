CPMAddPackage("gh:google/glog#v0.6.0")

target_link_libraries(${PROJECT_NAME} PRIVATE glog::glog)