target_sources(HLMV
	PRIVATE
		HLMVMainWindow.cpp
		HLMVMainWindow.h
		HLMVMainWindow.ui
		MainPanel.cpp
		MainPanel.h
		MainPanel.ui
		ViewerDocument.cpp
		ViewerDocument.h
		ViewerDocument.ui
		ViewerWindow.cpp
		ViewerWindow.h)

add_subdirectory(controlpanels)
add_subdirectory(options)
