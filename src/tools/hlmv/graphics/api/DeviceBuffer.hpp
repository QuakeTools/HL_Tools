#pragma once

#include <cstdint>

#include "graphics/api/BindableResource.hpp"
#include "graphics/api/Enums.hpp"

namespace graphics::api
{
struct BufferDescription
{
	const unsigned int SizeInBytes;
	const std::uint8_t Usage;
};

class DeviceBuffer : public BindableResource
{
protected:
	DeviceBuffer() = default;

public:

	DeviceBuffer(DeviceBuffer&&) = delete;
	DeviceBuffer& operator=(DeviceBuffer&&) = delete;

	DeviceBuffer(const DeviceBuffer&) = delete;
	DeviceBuffer& operator=(const DeviceBuffer&) = delete;

	virtual ~DeviceBuffer() = 0 {}

	virtual unsigned int GetSizeInBytes() const = 0;

	virtual std::uint8_t GetUsage() const = 0;
};
}
