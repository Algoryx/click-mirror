find_package(Protobuf REQUIRED)

set(LIBSOURCES
    include/click/MessageFactory.h
    src/MessageFactory.cpp
)

add_library(clicklib ${LIBSOURCES})
target_link_libraries(clicklib
    PUBLIC
        protobuf::libprotobuf
        clickprotobuf
)
target_include_directories(clicklib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(tests)
