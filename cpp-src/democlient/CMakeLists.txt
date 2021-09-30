
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_executable(democlient)
target_sources(democlient
    PRIVATE
        src/democlient.cpp
    )
target_link_libraries(democlient
    PRIVATE
    clicklib
)
