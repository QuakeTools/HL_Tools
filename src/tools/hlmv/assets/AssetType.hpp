#pragma once

#include <memory>

#include <entt/core/fwd.hpp>
#include <entt/core/type_info.hpp>

#include "assets/IAssetReader.hpp"
#include "assets/IAssetWriter.hpp"

/**
*	@brief Describes an asset type and provides operations to perform on them
*/
struct AssetType final
{
	AssetType(entt::id_type typeId, std::unique_ptr<IAssetReader>&& reader, std::unique_ptr<IAssetWriter>&& writer)
		: _typeId(typeId)
		, _reader(std::move(reader))
		, _writer(std::move(writer))
	{
	}

	entt::id_type GetTypeId() const { return _typeId; }

	IAssetReader* GetReader() const { return _reader.get(); }

	IAssetWriter* GetWriter() const { return _writer.get(); }

private:
	const entt::id_type _typeId;

	const std::unique_ptr<IAssetReader> _reader;
	const std::unique_ptr<IAssetWriter> _writer;
};
