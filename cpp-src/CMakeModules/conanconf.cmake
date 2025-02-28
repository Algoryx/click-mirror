# Optionally use CMAKE_SOURCE_DIR as conan.cmake location and version control the file to get
# - feedback when it changes
# - build not depending on file to be online.
set(CONAN_CMAKE_LOCAL_FILE ${CMAKE_BINARY_DIR}/conan.cmake)
set(CONAN_CMAKE_MASTER_FILE ${CMAKE_BINARY_DIR}/.github.conan.cmake)

if(NOT EXISTS ${CONAN_CMAKE_MASTER_FILE})
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                "${CONAN_CMAKE_MASTER_FILE}" STATUS CONAN_CMAKE_DOWNLOAD_STATUS
                TLS_VERIFY ON)

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

if (APPLE)
  # Hardcode libsodium version used by click->zmq as workaround for clang16 not compiling libsodium as of 2024-09-17
  set(LIBSODIUM_DEPENDENCY libsodium/1.0.18)
endif()

conan_cmake_configure(
  REQUIRES
    protobuf/3.21.12@algoryx/stable
    cppzmq/4.10.0
    ${LIBSODIUM_DEPENDENCY}
    argparse/2.9
  BUILD_REQUIRES
    protobuf/3.21.12@algoryx/stable
    catch2/3.5.3
  GENERATORS
    cmake
    cmake_find_package
  OPTIONS
    ${ZMQOPTIONS}
)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                      BUILD missing
                      SETTINGS ${settings})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_MODULE_PATH}")
