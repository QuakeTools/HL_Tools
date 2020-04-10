#pragma once

#include <cstdio>
#include <filesystem>
#include <memory>

#include "assets/Asset.hpp"

struct IAssetWriter
{
	virtual ~IAssetWriter() = 0 {}

	virtual void Write(const std::filesystem::path& fileName, FILE* file, const Asset& asset) = 0;
};
