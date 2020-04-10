#pragma once

#include <cstdio>
#include <filesystem>

#include "assets/Asset.hpp"
#include "assets/AssetExceptions.hpp"
#include "assets/AssetType.hpp"
#include "assets/AssetTypes.hpp"

#include "assets/IAssetReader.hpp"
#include "assets/IAssetWriter.hpp"

#include "utility/FileUtilities.hpp"

inline std::unique_ptr<Asset> ReadAsset(const AssetTypes& types, const std::filesystem::path& fileName)
{
	if (const file_ptr file{fopen(fileName.u8string().c_str(), "rb")}; file)
	{
		const auto position = ftell(file.get());

		for (auto type : types.GetAssetTypes())
		{
			auto reader = type->GetReader();

			const auto canRead = reader->CanRead(file.get());

			fseek(file.get(), position, SEEK_SET);

			if (canRead)
			{
				return reader->Read(fileName, file.get());
			}
		}

		throw UnknownAssetType("Unknown asset type while trying to read asset from file");
	}
	else
	{
		throw CouldNotOpenFileForWriting("Could not open file \"" + fileName.u8string() + "\" for reading asset");
	}
}

inline void WriteAsset(const AssetTypes& types, const std::filesystem::path& fileName, const Asset& asset)
{
	if (auto type = types.FindById(asset.GetTypeId()); type)
	{
		if (const file_ptr file{fopen(fileName.u8string().c_str(), "wb")}; file)
		{
			type->GetWriter()->Write(fileName, file.get(), asset);
		}
		else
		{
			throw CouldNotOpenFileForWriting("Could not open file \"" + fileName.u8string() + "\" for writing asset");
		}
	}
	else
	{
		throw UnknownAssetType("Unknown asset type while trying to write asset to file");
	}
}
