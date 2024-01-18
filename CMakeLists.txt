cmake_minimum_required(VERSION 3.11)

project(everythingblackexcept)

if(WIN32)
	add_executable(everythingblackexcept src/main.c src/implementations/implementation_windows.c)
else()
	add_executable(everythingblackexcept src/main.c src/implementations/implementation_linux_x11.c)
	target_link_libraries(everythingblackexcept X11 Xinerama)
endif()

set_target_properties(everythingblackexcept PROPERTIES C_STANDARD 99 CXX_STANDARD_REQUIRED ON)
target_include_directories(everythingblackexcept PRIVATE src include)
