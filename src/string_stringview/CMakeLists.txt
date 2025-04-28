# src/string_stringview/CMakeLists.txt

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# 调试和编译命令
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_BUILD_TYPE Debug)

# 定义可执行目标
add_executable(c_str src/c_str.cc)
add_executable(string_literial src/string_literial.cc)
add_executable(string src/string.cc)
add_executable(convert src/convert_string_number.cc)
add_executable(sv src/string_view.cc)
add_executable(sformat src/string_format.cc)

set(EXECUTABLES
  c_str
  string_literial
  string
  convert
  sv
  sformat
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
target_include_directories(c_str PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接到 common 库以获取 color.hh 的头文件路径
target_link_libraries(c_str PUBLIC
    common
)