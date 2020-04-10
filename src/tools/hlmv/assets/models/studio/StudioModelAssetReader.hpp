#pragma once

#include "assets/IAssetReader.hpp"

#include "assets/models/studio/EditableStudioModel.hpp"
#include "assets/models/studio/StudioIO.hpp"

namespace models::studio
{
struct StudioModelAssetReader final : public IAssetReader
{
	bool CanRead(FILE* file) const override final
	{
		return IsStudioHeader(file);
	}

	std::unique_ptr<Asset> Read(const std::filesystem::path& fileName, FILE* file) override final
	{
		auto data = models::studio::LoadStudioFiles(fileName, file);

		auto editable = models::studio::ConvertToEditable(fileName, data);

		return std::make_unique<EditableStudioModel>(std::move(editable));
	}
};
}
