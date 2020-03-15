cmake_minimum_required(VERSION 3.16)

project(HL_Tools
	VERSION 3.0.0
	DESCRIPTION "Half-Life Modding Tools"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
