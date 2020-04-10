target_sources(HLMV
	PRIVATE
		Asset.hpp
		AssetExceptions.hpp
		AssetIO.hpp
		AssetType.hpp
		AssetTypes.hpp
		IAssetReader.hpp
		IAssetWriter.hpp)

add_subdirectory(models)
