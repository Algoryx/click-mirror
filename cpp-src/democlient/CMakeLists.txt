
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_executable(democlient)
target_sources(democlient
    PRIVATE
        src/democlient.cpp
    )
target_compile_features(democlient PRIVATE cxx_std_17)
target_link_libraries(democlient
    PRIVATE
    click
    CONAN_PKG::argparse
)

add_executable(demoserver)
target_sources(demoserver
    PRIVATE
        src/demoserver.cpp
    )
target_compile_features(demoserver PRIVATE cxx_std_17)
target_link_libraries(demoserver
    PRIVATE
    click
    CONAN_PKG::argparse
)
