#include <cassert>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <sstream>

#include "assets/models/studio/StudioIO.hpp"
#include "utility/FileUtilities.hpp"

namespace models::studio
{
std::string FormatSequenceGroupFileName(const std::string& baseName, const std::string_view extension,
	const int index, std::ostringstream& sequenceGroupName)
{
	sequenceGroupName.str({});

	sequenceGroupName << baseName <<
		std::setfill('0') << std::setw(2) << index <<
		std::setw(0) << extension;

	return sequenceGroupName.str();
}

bool IsStudioHeader(FILE* file)
{
	//Assume the file pointer is already at the correct position
	int id, version;

	if (fread(&id, sizeof(id), 1, file) != 1)
	{
		return false;
	}

	const std::string_view idString{reinterpret_cast<const char*>(&id), sizeof(id)};

	if (MainHeaderId.compare(idString) &&
		SequenceGroupHeaderId.compare(idString))
	{
		return false;
	}

	if (fread(&version, sizeof(version), 1, file) != 1)
	{
		return false;
	}

	if (version != FileVersion)
	{
		return false;
	}

	return true;
}

template<typename T>
static studio_ptr<T> LoadHeaderFromFileHandle(const std::filesystem::path& fileName, const int version, const std::string_view headerId, FILE* file)
{
	assert(headerId.length() == sizeof(studiohdr_t::id));

	const auto position = ftell(file);

	fseek(file, 0, SEEK_END);
	const std::size_t size = ftell(file) - position;
	fseek(file, position, SEEK_SET);

	auto buffer = std::make_unique<std::uint8_t[]>(size);

	auto studioHeader = reinterpret_cast<T*>(buffer.get());

	const size_t elementsRead = fread(studioHeader, size, 1, file);

	if (elementsRead != 1)
	{
		throw AssetInvalidFormat(std::string{"Error reading file \""} + fileName.u8string() + "\"");
	}

	const std::string_view idString{reinterpret_cast<const char*>(&studioHeader->id), sizeof(studioHeader->id)};

	if (headerId.compare(idString))
	{
		throw AssetInvalidFormat(std::string{"The file \""} + fileName.u8string() + "\" is not a studio header");
	}

	if (studioHeader->version != version)
	{
		throw AssetVersionDiffers(std::string{"File \""} + fileName.u8string() + "\": version differs: expected \"" +
			std::to_string(version) + "\", got \"" + std::to_string(studioHeader->version) + "\"");
	}

	buffer.release();

	return studio_ptr<T>(studioHeader);
}

template<typename T>
static studio_ptr<T> LoadHeader(const std::filesystem::path& fileName, const int version, const std::string_view headerId)
{
	assert(headerId.length() == sizeof(studiohdr_t::id));

	const file_ptr file{fopen(fileName.u8string().c_str(), "rb")};

	if (!file)
	{
		throw AssetNotFound(std::string{"File \""} + fileName.u8string() + "\" not found");
	}

	return LoadHeaderFromFileHandle<T>(fileName, version, headerId, file.get());
}

static studio_ptr<studiohdr_t> LoadStudioMainHeader(const std::filesystem::path& fileName, FILE* file)
{
	return LoadHeaderFromFileHandle<studiohdr_t>(fileName, FileVersion, MainHeaderId, file);
}

studio_ptr<studiohdr_t> LoadStudioMainHeader(const std::filesystem::path& fileName)
{
	return LoadHeader<studiohdr_t>(fileName, FileVersion, MainHeaderId);
}

studio_ptr<studioseqhdr_t> LoadStudioSequenceHeader(const std::filesystem::path& fileName)
{
	return LoadHeader<studioseqhdr_t>(fileName, FileVersion, SequenceGroupHeaderId);
}

StudioData LoadStudioFiles(const std::filesystem::path& fileName, FILE* file)
{
	std::filesystem::path baseFileName{fileName};

	baseFileName.replace_extension();

	const auto isDol = fileName.extension() == ".dol";

	//Load the model
	auto mainHeader = LoadStudioMainHeader(fileName, file);

	studio_ptr<studiohdr_t> textureHeader;

	// preload textures
	if (mainHeader->numtextures == 0)
	{
		const auto extension = isDol ? "T.dol" : "T.mdl";

		auto textureName = baseFileName;

		textureName += extension;

		textureHeader = LoadStudioMainHeader(textureName.u8string());
	}

	std::vector<studio_ptr<studioseqhdr_t>> sequenceHeaders;

	// preload animations
	if (mainHeader->numseqgroups > 1)
	{
		sequenceHeaders.reserve(mainHeader->numseqgroups - 1);

		const std::string_view extension = isDol ? ".dol" : ".mdl";

		std::ostringstream sequenceGroupName;

		for (int i = 1; i < mainHeader->numseqgroups; ++i)
		{
			const auto sequenceFileName{FormatSequenceGroupFileName(baseFileName.u8string(), extension, i, sequenceGroupName)};

			sequenceHeaders.emplace_back(LoadStudioSequenceHeader(sequenceFileName));
		}
	}

	return {std::move(mainHeader), std::move(textureHeader), std::move(sequenceHeaders)};
}

template<typename T>
static void SaveDataToFile(const std::filesystem::path& fileName, const T& data)
{
	if (auto file = fopen(fileName.u8string().c_str(), "wb"); file)
	{
		const bool success = fwrite(&data, 1, data.length, file) == data.length;

		fclose(file);

		if (!success)
		{
			throw ErrorWritingToFile(std::string{"Error writing file \""} + fileName.u8string() + "\"");
		}
	}
	else
	{
		throw CouldNotOpenFileForWriting(std::string{"Could not open file \""} + fileName.u8string() + "\" for writing");
	}
}

void SaveStudioFiles(const std::filesystem::path& fileName, const StudioData& data)
{
	if (fileName.empty())
	{
		throw std::invalid_argument("Filename must be valid");
	}

	if (!data.MainData)
	{
		throw std::invalid_argument("Main data must be non-null");
	}

	SaveDataToFile(fileName, *data.MainData);

	auto baseFileName{fileName};

	baseFileName.replace_extension();

	// write texture model
	if (data.TextureData)
	{
		auto textureName = baseFileName;

		textureName += "T.mdl";

		SaveDataToFile(textureName.u8string().c_str(), *data.TextureData);
	}

	// write seq groups
	if (!data.SequenceGroupData.empty())
	{
		std::ostringstream sequenceGroupName;

		int index = 1;

		for (const auto& sequenceGroup : data.SequenceGroupData)
		{
			if (!sequenceGroup)
			{
				throw std::invalid_argument(std::string{"Sequence group data " + std::to_string(index) + " must be non-null"});
			}

			const auto sequenceFileName{FormatSequenceGroupFileName(baseFileName.u8string(), ".mdl", index, sequenceGroupName)};

			SaveDataToFile(sequenceFileName, *sequenceGroup);

			++index;
		}
	}
}
}
