cmake_minimum_required(VERSION 3.11)

project(everythingblackexcept)

add_executable(ebe src/main.c)

set_target_properties(ebe PROPERTIES C_STANDARD 99 CXX_STANDARD_REQUIRED ON)
