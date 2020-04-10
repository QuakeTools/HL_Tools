#pragma once

#include "assets/IAssetWriter.hpp"

#include "assets/models/studio/EditableStudioModel.hpp"
#include "assets/models/studio/StudioIO.hpp"

namespace models::studio
{
struct StudioModelAssetWriter final : public IAssetWriter
{
	void Write(const std::filesystem::path& fileName, FILE* file, const Asset& asset) override final
	{
		const auto& studioModel = static_cast<const EditableStudioModel&>(asset);
		//TODO
	}
};
}
