# src/string_stringview/CMakeLists.txt

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# 调试和编译命令
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_BUILD_TYPE Debug)


add_executable(fv format_variant.cc)

set(EXECUTABLES
  fv
)

foreach(execu IN ITEMS ${EXECUTABLES})
  target_include_directories(${execu} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
  target_link_libraries(${execu} PUBLIC
    common
  )
endforeach()


# 指定本地头文件目录
target_include_directories(fv PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接到 common 库以获取 color.hh 的头文件路径
target_link_libraries(c_str PUBLIC
    common
)

