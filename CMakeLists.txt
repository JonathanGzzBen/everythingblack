cmake_minimum_required(VERSION 3.11)

project(everythingblackexcept)

add_executable(everythingblackexcept src/main.c)

set_target_properties(everythingblackexcept PROPERTIES C_STANDARD 99 CXX_STANDARD_REQUIRED ON)
