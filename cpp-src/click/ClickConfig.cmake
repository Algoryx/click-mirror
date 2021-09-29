include(CMakeFindDependencyMacro)
find_dependency(Protobuf 3.17.1)
find_dependency(zmqpp 4.2.0)
include("${MAKE_CURRENT_LIST_DIR}/ClickTargets.cmake")
