# AGENTS.md

## General Project Overview
This is a Roguelike Card Game project built on top of a custom C++ game engine (see `GameEngine/` submodule).

## Core Directives for Agents
1.  **Testing Requirement**: For every change made to the core logic (especially in `src/core` and `include/core`), you **MUST** run the relevant unit tests.
2.  **Enabling Tests**: Ensure you configure the build with `-DBUILD_GAME_TESTS=ON`.
3.  **Running Tests**: After building, execute the test binaries located in `build/tests/`. You can also run all tests using `ctest` from the `build` directory.
4.  **Creating New Tests**: Following Google's C++ standards, new unit tests should be placed in the same directory as the source files they test and follow the `*_test.cpp` naming convention. The CMake build system will automatically discover and include it.
5.  **Namespace Usage**: Avoid `using namespace` in headers. In source files and tests, use specific `using` declarations or explicit qualifiers. Tests should ideally reside in an anonymous namespace within the appropriate project namespace (e.g., `core::effects::actions`).

## Project Tracking and Maintenance
1.  **Status Updates**: After completing any task and before every commit, you **MUST** update the `STATUS.md` file located in the root directory.
    -   Update the **Most Recent Changes** section with a concise summary of your work.
    -   Review and update the **Technical Status Report** if your changes affected any major scenes or infrastructure.
    -   Add or refine **Recommendations & Next Steps** based on your findings during the task.
2.  **Reference Status**: Always refer to [STATUS.md](STATUS.md) to understand the current progress and state of the game before starting a new task.

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
