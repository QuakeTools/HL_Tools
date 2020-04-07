#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "models/studio/StudioFileFormat.hpp"

namespace models::studio
{
//TODO: maybe use a set of exceptions common to all model formats

/**
*	@brief Base class for all studio model exceptions
*/
class StudioModelException : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

/**
*	@brief Indicates that the studio model file does not exist
*/
class StudioModelNotFound : public StudioModelException
{
public:
	using StudioModelException::StudioModelException;
};

/**
*	@brief Indicates that a studio model file has the wrong format (i.e. not a studio model)
*/
class StudioModelInvalidFormat : public StudioModelException
{
public:
	using StudioModelException::StudioModelException;
};

/**
*	@brief Indicates that a studio model file has the wrong version (e.g. trying to load a Source model)
*/
class StudioModelVersionDiffers : public StudioModelException
{
public:
	StudioModelVersionDiffers(const std::string& message, int version)
		: StudioModelException(message)
		, _version(version)
	{
	}

	/**
	*	@brief Gets the version that the model has
	*/
	int GetVersion() const { return _version; }

private:
	const int _version;
};

class CouldNotOpenFileForWriting : public StudioModelException
{
public:
	using StudioModelException::StudioModelException;
};

class ErrorWritingToFile : public StudioModelException
{
public:
	using StudioModelException::StudioModelException;
};

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

studio_ptr<studiohdr_t> LoadStudioMainHeader(const std::string& fileName);

studio_ptr<studioseqhdr_t> LoadStudioSequenceHeader(const std::string& fileName);

StudioData LoadStudioFiles(const std::string& fileName);

void SaveStudioFiles(const std::string& fileName, const StudioData& data);
}
