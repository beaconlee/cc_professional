cmake_minimum_required(VERSION 3.20)
project(CCProfessional LANGUAGES CXX)

# 确保使用 Clang
# set(CMAKE_CXX_COMPILER "clang++-20")

# 添加 -stdlib=libc++ 标志
# add_compile_options(-stdlib=libc++)
# include_directories(/usr/lib/llvm-20/include/c++/v1)
# 确保链接到 libc++ 和 libc++abi
# add_link_options(-lc++ -lc++abi)

find_package (glog 0.7.1 REQUIRED)

add_subdirectory(src/common)
add_subdirectory(src/string_stringview)
add_subdirectory(src/io)
add_subdirectory(src/utils)
add_subdirectory(src/function)
add_subdirectory(src/advanced_templates)
add_subdirectory(tests)