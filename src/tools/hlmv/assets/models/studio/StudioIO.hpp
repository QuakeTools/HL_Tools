#pragma once

#include <cstdio>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "assets/AssetExceptions.hpp"

#include "assets/models/studio/StudioFileFormat.hpp"

namespace models::studio
{
struct StudioDataDeleter
{
	void operator()(studiohdr_t* pointer) const
	{
		delete[] pointer;
	}

	void operator()(studioseqhdr_t* pointer) const
	{
		delete[] pointer;
	}
};

template<typename T>
using studio_ptr = std::unique_ptr<T, StudioDataDeleter>;

struct StudioData final
{
	studio_ptr<studiohdr_t> MainData;
	studio_ptr<studiohdr_t> TextureData;

	std::vector<studio_ptr<studioseqhdr_t>> SequenceGroupData;
};

bool IsStudioHeader(FILE* file);

studio_ptr<studiohdr_t> LoadStudioMainHeader(const std::filesystem::path& fileName);

studio_ptr<studioseqhdr_t> LoadStudioSequenceHeader(const std::filesystem::path& fileName);

StudioData LoadStudioFiles(const std::filesystem::path& fileName, FILE* file);

void SaveStudioFiles(const std::filesystem::path& fileName, const StudioData& data);
}
