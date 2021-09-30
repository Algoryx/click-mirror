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

# TODO: Discuss and possibly remove below!
# We want to update packages in the Conan cache that have been changed on the
# server. However, for performance reasons we only want to do that on the
# first CMake run. We detect first run by checking for CMakeCache.txt.
# if (EXISTS "${CMAKE_BINARY_DIR}/CMakeCache.txt")
#   set(GENERIC_UPDATE_CONAN_PACKAGES "")
# else()
#   set(GENERIC_UPDATE_CONAN_PACKAGES "UPDATE")
# endif()

# set(GENERIC_CONAN_BUILD_RULE "never")

# Using recommendation at https://github.com/conan-io/cmake-conan to not use conan_cmake_run which is deprecated.

conan_cmake_configure(
  REQUIRES 
    protobuf/3.17.1
    zmqpp/4.2.0
  BUILD_REQUIRES
    protobuf/3.17.1
    catch2/2.11.1
  GENERATORS
    cmake
    cmake_find_package
  OPTIONS
    # Needed for 32 bit builds
    zeromq:encryption=None
  )

  conan_cmake_autodetect(settings)

  conan_cmake_install(PATH_OR_REFERENCE .
                      BUILD missing
#                      REMOTE conancenter
                      SETTINGS ${settings})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake) 
conan_basic_setup(TARGETS) 
