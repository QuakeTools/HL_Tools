#pragma once

#include <stdexcept>

/**
*	@brief Base class for all asset exceptions
*/
struct AssetException : public std::runtime_error
{
	using std::runtime_error::runtime_error;
};

/**
*	@brief Indicates that an asset does not exist
*/
struct AssetNotFound final : public AssetException
{
	using AssetException::AssetException;
};

/**
*	@brief Indicates that an asset has the wrong format (e.g. not a studio model)
*/
struct AssetInvalidFormat final : public AssetException
{
	using AssetException::AssetException;
};

/**
*	@brief Indicates that an asset has the wrong version (e.g. trying to load a Source model)
*/
struct AssetVersionDiffers final : public AssetException
{
	using AssetException::AssetException;
};

struct CouldNotOpenFileForWriting final : public AssetException
{
	using AssetException::AssetException;
};

struct ErrorWritingToFile : public AssetException
{
	using AssetException::AssetException;
};

struct UnknownAssetType final : public AssetException
{
	using AssetException::AssetException;
};