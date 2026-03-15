cmake_minimum_required(VERSION 3.14)

# ── 辅助宏：创建测试可执行文件并注册 ─────────────────────────────────────────
function(mygl_add_test name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE mygl_core GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(${name})
endfunction()

mygl_add_test(test_math           test_math.cpp)
mygl_add_test(test_vector         test_vector.cpp)
mygl_add_test(test_matrix         test_matrix.cpp)
mygl_add_test(test_transforms     test_transforms.cpp)
mygl_add_test(test_matrix_wrapper test_matrix_wrapper.cpp)
