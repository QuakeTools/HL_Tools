#include <cassert>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <sstream>

#include "models/studio/StudioIO.hpp"

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

template<typename T>
static studio_ptr<T> LoadHeader(const std::string& fileName, const int version, const std::string_view headerId)
{
	assert(headerId.length() == sizeof(studiohdr_t::id));

	const auto file = fopen(fileName.c_str(), "rb");

	if (!file)
	{
		throw StudioModelNotFound(std::string{"File \""} + fileName + "\" not found");
	}

	fseek(file, 0, SEEK_END);
	const std::size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);

	auto buffer = std::make_unique<std::uint8_t[]>(size);

	auto studioHeader = reinterpret_cast<T*>(buffer.get());

	const size_t elementsRead = fread(studioHeader, size, 1, file);
	fclose(file);

	if (elementsRead != 1)
	{
		throw StudioModelInvalidFormat(std::string{"Error reading file\""} + fileName + "\"");
	}

	const std::string_view idString{reinterpret_cast<const char*>(&studioHeader->id), sizeof(studioHeader->id)};

	if (headerId.compare(idString))
	{
		throw StudioModelInvalidFormat(std::string{"The file \""} + fileName + "\" is not a studio header");
	}

	if (studioHeader->version != version)
	{
		throw StudioModelVersionDiffers(std::string{"File \""} + fileName + "\": version differs: expected \"" +
			std::to_string(version) + "\", got \"" + std::to_string(studioHeader->version) + "\"",
			studioHeader->version);
	}

	buffer.release();

	return studio_ptr<T>(studioHeader);
}

studio_ptr<studiohdr_t> LoadStudioMainHeader(const std::string& fileName)
{
	return LoadHeader<studiohdr_t>(fileName, FileVersion, MainHeaderId);
}

studio_ptr<studioseqhdr_t> LoadStudioSequenceHeader(const std::string& fileName)
{
	return LoadHeader<studioseqhdr_t>(fileName, FileVersion, SequenceGroupHeaderId);
}

StudioData LoadStudioFiles(const std::string& fileName)
{
	const std::filesystem::path fullFileName{fileName};

	std::filesystem::path baseFileName{fullFileName};

	baseFileName.replace_extension();

	const auto isDol = fullFileName.extension() == ".dol";

	//Load the model
	auto mainHeader = LoadStudioMainHeader(fileName);

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
static void SaveDataToFile(const std::string& fileName, const T& data)
{
	if (auto file = fopen(fileName.c_str(), "wb"); file)
	{
		const bool success = fwrite(&data, 1, data.length, file) == data.length;

		fclose(file);

		if (!success)
		{
			throw ErrorWritingToFile(std::string{"Error writing file \""} +fileName + "\"");
		}
	}
	else
	{
		throw CouldNotOpenFileForWriting(std::string{"Could not open file \""} +fileName + "\" for writing");
	}
}

void SaveStudioFiles(const std::string& fileName, const StudioData& data)
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

	const std::filesystem::path fullFileName{fileName};

	auto baseFileName{fullFileName};

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
