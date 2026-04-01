add_executable(
  game_tests
  test_card_data.cpp
)

target_link_libraries(
  game_tests
  GTest::gtest_main
  RoguelikeCardgameLibrary
)

include(GoogleTest)
gtest_discover_tests(game_tests)
