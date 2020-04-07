# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

find_package(OpenGL REQUIRED)

find_package(glm CONFIG REQUIRED)

# GLEW's CMake support is terrible so until that's fixed, just locate the include path and library path manually
set(GLEW_DIR "" CACHE PATH "Path to GLEW directory")

add_library(GLEW::glew STATIC IMPORTED)

find_path(GLEW_INCLUDE_DIR GL/glew.h
	PATHS
		${GLEW_DIR}
	PATH_SUFFIXES
		/include)

# The GLEW install file places the library in a Release subdirectory, so account for it
find_library(GLEW_LIBRARY glew32s
	PATHS
		${GLEW_DIR}/lib/Release
	PATH_SUFFIXES
		/Win32)

set_target_properties(GLEW::glew PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${GLEW_INCLUDE_DIR})
set_target_properties(GLEW::glew PROPERTIES IMPORTED_LOCATION ${GLEW_LIBRARY})

target_compile_definitions(GLEW::glew
	INTERFACE
		GLEW_STATIC)

unset(GLEW_LIBRARY CACHE)

find_package(ENTT REQUIRED)

function(copy_qt_dependencies target_name plugins)
	foreach(plugin ${${plugins}})
		# Copy plugins to the directory relative to the plugins directory in the Qt install directory
		get_target_property(TARGET_LOCATION ${plugin} LOCATION)
		get_filename_component(PARENT_DIR ${TARGET_LOCATION} DIRECTORY)
		get_filename_component(TARGET_DIR_NAME ${PARENT_DIR} NAME)
		
		add_custom_command(TARGET ${target_name} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${target_name}>/${TARGET_DIR_NAME})
	  
		add_custom_command(TARGET ${target_name} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				$<TARGET_FILE:${plugin}>
				$<TARGET_FILE_DIR:${target_name}>/${TARGET_DIR_NAME})
	endforeach()
endfunction()

add_subdirectory(hlmv)
