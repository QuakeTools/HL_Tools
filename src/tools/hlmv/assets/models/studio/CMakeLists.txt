target_sources(HLMV
	PRIVATE
		EditableStudioModel.cpp
		EditableStudioModel.hpp
		StudioFileFormat.hpp
		StudioIO.cpp
		StudioIO.hpp
		StudioModelAssetReader.hpp
		StudioModelAssetWriter.hpp)

add_subdirectory(rendering)
