target_sources(HLMV
	PRIVATE
		EditorAsset.hpp
		HLMVMainWindow.cpp
		HLMVMainWindow.hpp
		HLMVMainWindow.ui
		ProgramState.hpp
		ViewerWindow.cpp
		ViewerWindow.hpp)

add_subdirectory(dialogs)
add_subdirectory(dockpanels)
add_subdirectory(options)
