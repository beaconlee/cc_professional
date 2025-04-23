# src/common/CMakeLists.txt

# 定义静态库（或根据需要改为 SHARED）
add_library(common SHARED
    src/log.cc
)
# 指定本地头文件目录
target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(common PUBLIC
    glog::glog
)