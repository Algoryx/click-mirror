set(CPACK_PACKAGE_VENDOR "Algoryx Simulation")
set(CPACK_PACKAGE_CONTACT "contact@algoryx.se")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Connect Controller to OpenPLX-enabled Simulation")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/../README.md")
include(CPack)
