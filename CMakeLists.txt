cmake_minimum_required(VERSION 3.15)
project(DeckBuilderGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. ADD THE ENGINE SUBDIRECTORY (Crucial)
# This tells CMake to go into the MyEngine folder and process its CMakeLists.txt.
# This makes the "GameEngine" target available to your game.
add_subdirectory("GameEngine")

# 2. SOURCE AND HEADER SETUP
set(SOURCE_FILES "src/main.cpp")
file(GLOB_RECURSE INCLUDE_FILES "src/*.h" "include/*.h")

# 3. ASSET INFORMATION
set(ASSET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSET_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")

# Copy assets to build directory for relative path loading
file(COPY "${ASSET_SOURCE_DIR}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# Inject the asset path as a macro for C++ code
add_definitions(-DENGINE_ASSETS_PATH="${ASSET_BUILD_DIR}/")

# 4. CREATE THE EXECUTABLE
# We include INCLUDE_FILES here so they show up in the Visual Studio Solution Explorer
add_executable(RoguelikeCardgame ${SOURCE_FILES} ${INCLUDE_FILES})

# 5. LINKING & INCLUDES
# By linking 'GameEngine', your project automatically inherits the 'PUBLIC' 
# include directories we defined in the engine's CMake file.
target_link_libraries(RoguelikeCardgame PRIVATE GameEngine)

# Ensure the game's own local include folder is searchable
target_include_directories(RoguelikeCardgame PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# 6. VISUAL STUDIO SETTINGS
# Set the working directory so assets load correctly when pressing F5
set_target_properties(RoguelikeCardgame PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")