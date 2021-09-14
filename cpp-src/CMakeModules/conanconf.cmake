# Optionally use CMAKE_SOURCE_DIR as conan.cmake location and version control the file to get better 
set(CONAN_CMAKE_LOCAL_FILE ${CMAKE_BINARY_DIR}/conan.cmake)
set(CONAN_CMAKE_MASTER_FILE ${CMAKE_BINARY_DIR}/.github.conan.cmake)

if(NOT EXISTS ${CONAN_CMAKE_MASTER_FILE})
  message(STATUS "Downloading conan.cmake from https://github.com/memsharded/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                "${CONAN_CMAKE_MASTER_FILE}" STATUS CONAN_CMAKE_DOWNLOAD_STATUS)

  list(GET CONAN_CMAKE_DOWNLOAD_STATUS 0 CONAN_CMAKE_DOWNLOAD_STATUS)

  if(EXISTS ${CONAN_CMAKE_LOCAL_FILE})
    file(READ ${CONAN_CMAKE_LOCAL_FILE} CONAN_CMAKE_LOCAL)
  endif()
  file(READ ${CONAN_CMAKE_MASTER_FILE} CONAN_CMAKE_MASTER)

  if(CONAN_CMAKE_DOWNLOAD_STATUS EQUAL 0 AND (NOT CONAN_CMAKE_MASTER STREQUAL CONAN_CMAKE_LOCAL))
    message(STATUS "Updating local conan.cmake file, found updated version on github")
    file(WRITE ${CONAN_CMAKE_LOCAL_FILE} "${CONAN_CMAKE_MASTER}")
  endif()

endif()

include(${CONAN_CMAKE_LOCAL_FILE})


# We want to update packages in the Conan cache that have been changed on the
# server. However, for performance reasons we only want to do that on the
# first CMake run. We detect first run by checking for CMakeCache.txt.
if (EXISTS "${CMAKE_BINARY_DIR}/CMakeCache.txt")
  set(RCP_UPDATE_CONAN_PACKAGES "")
else()
  set(RCP_UPDATE_CONAN_PACKAGES "UPDATE")
endif()

set(RCP_CONAN_BUILD_RULE "never")

conan_cmake_run(CONANFILE conanfile.txt
  ${RCP_UPDATE_CONAN_PACKAGES}
  BASIC_SETUP
  KEEP_RPATHS
  NO_OUTPUT_DIRS
  CMAKE_TARGETS
  BUILD ${RCP_CONAN_BUILD_RULE}  # Set to 'all' to force rebuild of dependencies.
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
