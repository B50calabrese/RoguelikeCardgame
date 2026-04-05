# AGENTS.md

## General Project Overview
This is a Roguelike Card Game project built on top of a custom C++ game engine (see `GameEngine/` submodule).

## Core Directives for Agents
1.  **Testing Requirement**: For every change made to the core logic (especially in `src/core` and `include/core`), you **MUST** run the relevant unit tests.
2.  **Enabling Tests**: Ensure you configure the build with `-DBUILD_GAME_TESTS=ON`.
3.  **Running Tests**: After building, execute the test binaries located in `build/tests/`. You can also run all tests using `ctest` from the `build` directory.
4.  **Creating New Tests**: When adding new features or fixing bugs in the game logic, add a corresponding test file in `src/tests/` named `[feature]_test.cpp`. The build system will automatically discover and include it.
5.  **Namespace Usage**: Avoid `using namespace` in headers. In source files and tests, use specific `using` declarations or explicit qualifiers. Tests should ideally reside in an anonymous namespace within the appropriate project namespace (e.g., `core::effects::actions`).

## Build and Test Commands
```bash
# Configure with tests enabled
cmake -B build -DBUILD_GAME_TESTS=ON

# Build all targets including tests
cmake --build build -j$(nproc)

# Run a specific test
./build/tests/deal_damage_action_test

# Run all tests
cd build && ctest
```
