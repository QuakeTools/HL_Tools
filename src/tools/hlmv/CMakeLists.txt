add_executable(HLMV)

target_link_libraries(HLMV
	PRIVATE
		Qt5::Widgets
		Qt5::Network
		glm
		GLEW::glew
		OpenGL::GL
		EnTT::EnTT)

target_compile_options(HLMV
	PRIVATE
		# Use strict floating point behavior to ensure sensitive operations like PaletteHueReplace work properly
		$<$<CXX_COMPILER_ID:MSVC>:/fp:strict>)

target_sources(HLMV
	PRIVATE
		Main.cpp)

add_subdirectory(application)
add_subdirectory(assets)
add_subdirectory(game)
add_subdirectory(graphics)
add_subdirectory(ui)
add_subdirectory(utility)

#Create filters
get_target_property(SOURCE_FILES HLMV SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

# Copy the Qt libraries over automatically
add_custom_command(TARGET HLMV POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Core>
        $<TARGET_FILE_DIR:HLMV>)

add_custom_command(TARGET HLMV POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Gui>
        $<TARGET_FILE_DIR:HLMV>)

add_custom_command(TARGET HLMV POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Widgets>
        $<TARGET_FILE_DIR:HLMV>)

add_custom_command(TARGET HLMV POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Network>
        $<TARGET_FILE_DIR:HLMV>)

# Copy plugins so the program can run
copy_qt_dependencies(HLMV Qt5Core_PLUGINS)
copy_qt_dependencies(HLMV Qt5Gui_PLUGINS)
copy_qt_dependencies(HLMV Qt5Widgets_PLUGINS)
copy_qt_dependencies(HLMV Qt5Network_PLUGINS)

# Copy resources
add_custom_command(TARGET HLMV POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/resources
		${CMAKE_CURRENT_BINARY_DIR}/resources
)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT HLMV)
