include(CMakeFindDependencyMacro)
find_dependency(Protobuf)
find_dependency(zmqpp)
include("${CMAKE_CURRENT_LIST_DIR}/ClickTargets.cmake")
