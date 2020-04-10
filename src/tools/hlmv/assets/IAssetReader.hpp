#pragma once

#include <cstdio>
#include <filesystem>
#include <memory>

#include "assets/Asset.hpp"

struct IAssetReader
{
	virtual ~IAssetReader() = 0 {}

	virtual bool CanRead(FILE* file) const = 0;

	virtual std::unique_ptr<Asset> Read(const std::filesystem::path& fileName, FILE* file) = 0;
};
