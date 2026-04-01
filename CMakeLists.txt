cmake_minimum_required(VERSION 3.15)
project(DeckBuilderGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. ADD THE ENGINE SUBDIRECTORY (Crucial)
# This tells CMake to go into the MyEngine folder and process its CMakeLists.txt.
# This makes the "GameEngine" target available to your game.
add_subdirectory("GameEngine")

add_subdirectory("demos")

# 7. GOOGLE TEST INTEGRATION
cmake_policy(SET CMP0135 NEW)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)

# 2. SOURCE AND HEADER SETUP
set(SOURCE_FILES
    "src/core/card_registry.cpp"
    "src/core/card_instance.cpp"
    "src/core/game_config.cpp"
    "src/core/graphics/card_renderer.cpp"
    "src/core/graphics/hand_renderer.cpp"
    "src/core/graphics/ui_button.cpp"
    "src/core/effects/effect_registry.cpp"
    "src/core/effects/actions/deal_damage_action.cpp"
    "src/core/effects/actions/modify_stats_action.cpp"
    "src/scenes/main_menu_scene.cpp"
    "src/scenes/combat_scene.cpp"
    "src/scenes/card_viewer_scene.cpp"
    "src/scenes/effect_demo_scene.cpp"
    "third_party/pugixml/pugixml.cpp")
file(GLOB_RECURSE INCLUDE_FILES "src/*.h" "include/*.h" "include/core/*.h" "include/core/graphics/*.h" "include/scenes/*.h" "src/scenes/*.h" "third_party/pugixml/*.hpp")

# 3. ASSET INFORMATION
set(ASSET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSET_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")

# Copy assets to build directory for relative path loading
file(COPY "${ASSET_SOURCE_DIR}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# 4. CREATE A LIBRARY FOR REUSABLE GAME LOGIC
# We move the logic here so demos can actually "link" to it.
add_library(RoguelikeCardgameLibrary STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

# Inject the asset path as a macro for C++ code
target_compile_definitions(RoguelikeCardgameLibrary PUBLIC 
    ENGINE_ASSETS_PATH=\"${ASSET_BUILD_DIR}/\"
)

# Link the Engine to the Library
target_link_libraries(RoguelikeCardgameLibrary PUBLIC GameEngine)

# Ensure the game's include folder is PUBLIC so anyone linking to it sees it
target_include_directories(RoguelikeCardgameLibrary PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/third_party/pugixml")

# 5. CREATE THE MAIN EXECUTABLE
add_executable(RoguelikeCardgame "src/main.cpp") # main.cpp is usually the only thing left here
target_link_libraries(RoguelikeCardgame PRIVATE RoguelikeCardgameLibrary)

# 6. VISUAL STUDIO SETTINGS
# Set the working directory so assets load correctly when pressing F5
set_target_properties(RoguelikeCardgame PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
