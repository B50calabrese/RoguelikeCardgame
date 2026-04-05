# Roguelike Card Game

A roguelike card game project built on a custom C++ game engine.

## Prerequisites

- C++20 compatible compiler
- CMake 3.15 or higher
- GLFW 3.3
- OpenGL
- FreeType 2.x
- pugixml (included in `third_party/`)

## Building the Game

```bash
# Initialize and update submodules
git submodule update --init --recursive

# Configure the build
cmake -B build

# Build the main executable
cmake --build build --target RoguelikeCardgame -j$(nproc)
```

The game executable will be located in `build/RoguelikeCardgame`.

## Testing

The project uses GoogleTest for unit testing. To build and run the tests:

### Enabling and Building Tests

```bash
# Configure with testing enabled
cmake -B build -DBUILD_GAME_TESTS=ON

# Build all tests
cmake --build build -j$(nproc)
```

### Running Tests

Individual test binaries are located in `build/tests/`. You can run them manually:

```bash
./build/tests/deal_damage_action_test
./build/tests/start_turn_action_test
```

Alternatively, you can run all tests using `ctest` from the `build` directory:

```bash
cd build && ctest
```

## Adding New Tests

Following Google's C++ standards, new unit tests should be placed in the same directory as the source files they test and follow the `*_test.cpp` naming convention. The CMake build system will automatically discover and add them to the test suite.
