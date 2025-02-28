include(CMakeFindDependencyMacro)
find_dependency(Protobuf 3 REQUIRED)
find_dependency(cppzmq 4 REQUIRED)
include("${CMAKE_CURRENT_LIST_DIR}/ClickTargets.cmake")
