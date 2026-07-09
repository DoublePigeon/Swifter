cmake_minimum_required(VERSION 3.15)
project(Swifter LANGUAGES CXX)

# SFML 3.x 强制要求 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==========================================
# 使用 FetchContent 自动下载并编译源码
# ==========================================
include(FetchContent)

# 告诉 CMake 我们要强制使用静态编译 (生成 .a 而不是 .dll)
# 好处是：最终只生成一个独立的 .exe，不需要到处拷贝 dll！
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

message(STATUS "获取 SFML 3.1.0 源码...")
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.1.0  # 锁定 3.1.0 版本
)
FetchContent_MakeAvailable(SFML)

# ==========================================
# 你的游戏项目配置
# ==========================================
# 收集 src 目录下的所有 .cpp 文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.h" "include/*.hpp")



# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 包含你的头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接 SFML 3 的现代目标 (注意：带有 SFML:: 前缀)
target_link_libraries(${PROJECT_NAME} PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::Audio
    SFML::System
)

# 设置调试时的工作目录为项目根目录 (以便正确读取 assets/ 下的图片和音频)
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)