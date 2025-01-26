file(GLOB_RECURSE GAME_SOURCE_FILES *.cpp)

add_executable(game ${GAME_SOURCE_FILES})
target_link_libraries(game PRIVATE engine)
