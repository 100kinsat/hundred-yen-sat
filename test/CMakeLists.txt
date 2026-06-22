cmake_minimum_required(VERSION 3.14)
project(hundred-yen-sat-tests CXX)

set(CMAKE_CXX_STANDARD 14)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL      https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    URL_HASH SHA256=1f357c27ca988c3f7c6b4bf68a9395005ac6761f034046e9dde0896e3aba00e4
)
FetchContent_MakeAvailable(googletest)

include(CTest)
include(GoogleTest)

# Fsmは動的確保・Arduino依存なしの純粋なC++ロジックなのでホストでテストできる。
# デバイス系（Imu/Gnss/Camera等）はWire/GNSS/Cameraライブラリへ依存するため、
# 実機テストのみとする（ルートCLAUDE.mdのテスト方針に準拠）。
add_executable(test_fsm
    fsm/test_fsm.cpp
    ${SRC_DIR}/hundred_yen_sat/fsm/Fsm.cpp
)
target_include_directories(test_fsm PRIVATE ${SRC_DIR})
target_link_libraries(test_fsm GTest::gtest_main)
gtest_discover_tests(test_fsm)
