cmake_minimum_required(VERSION 3.20)
project(CCProfessional LANGUAGES CXX)

find_package (glog 0.7.1 REQUIRED)

add_subdirectory(src/common)
add_subdirectory(src/string_stringview)