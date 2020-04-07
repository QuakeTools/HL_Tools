target_sources(HLMV
	PRIVATE
		ControlPanels.cpp
		ControlPanels.hpp
		ControlPanels.ui
		HLMVMainWindow.cpp
		HLMVMainWindow.hpp
		HLMVMainWindow.ui
		MainPanel.cpp
		MainPanel.hpp
		MainPanel.ui
		ViewerWindow.cpp
		ViewerWindow.hpp)

add_subdirectory(controlpanels)
add_subdirectory(options)
