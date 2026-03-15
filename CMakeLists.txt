cmake_minimum_required(VERSION 3.14)
project(MyGL CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ── Google Test ────────────────────────────────────────────────────────────────
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ── SFML 2.6.1 ────────────────────────────────────────────────────────────────
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.1
)
set(SFML_BUILD_AUDIO    OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK  OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC      OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

# ── MyGL Core math library (no SFML / window dependency) ──────────────────────
add_library(mygl_core STATIC
    MyGL/Core/Math.cpp
    MyGL/Core/Structures/Vector.cpp
    MyGL/Core/Structures/Matrix.cpp
    MyGL/Core/Transforms.cpp
)
target_include_directories(mygl_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/MyGL
)

# ── MyGL full library (Core + Context + Extension, with SFML) ─────────────────
add_library(mygl_full STATIC
    MyGL/Core/Math.cpp
    MyGL/Core/Structures/Vector.cpp
    MyGL/Core/Structures/Matrix.cpp
    MyGL/Core/Transforms.cpp
    MyGL/Core/GL.cpp
    MyGL/Core/Rasterizer.cpp
    MyGL/Core/Light.cpp
    MyGL/Core/Shader.cpp
    MyGL/Core/Clipping.cpp
    MyGL/Context/RenderingContext.cpp
    MyGL/Extension/Mesh.cpp
)
target_include_directories(mygl_full PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/MyGL
)
target_link_libraries(mygl_full PUBLIC sfml-graphics sfml-window sfml-system)

# ── Main executable ───────────────────────────────────────────────────────────
add_executable(MyGL_App main.cpp)
target_link_libraries(MyGL_App PRIVATE mygl_full)
# Working directory needs to be the project root to find Images/
set_target_properties(MyGL_App PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# ── Tests ─────────────────────────────────────────────────────────────────────
enable_testing()
add_subdirectory(tests)
