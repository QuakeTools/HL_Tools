#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

struct RGB24
{
	std::uint8_t R;
	std::uint8_t G;
	std::uint8_t B;
};

struct RGBA32
{
	RGB24& AsRGB() { return *reinterpret_cast<RGB24*>(this); }

	std::uint8_t R;
	std::uint8_t G;
	std::uint8_t B;
	std::uint8_t A;
};

template<typename ColorType>
class Palette final
{
public:
	static constexpr std::size_t ColorCount = 256;

	using Type = ColorType;

	Palette() = default;
	Palette(const Palette&) = default;
	Palette& operator=(const Palette&) = default;

	Type operator[](std::size_t index) const { return _palette[index]; }

	Type& operator[](std::size_t index) { return _palette[index]; }

	const Type* data() const { return _palette; }
	Type* data() { return _palette; }

	std::size_t size() const { return ColorCount; }

private:
	std::array<Type, ColorCount> _palette;
};

using RGB24Palette = Palette<RGB24>;
using RGBA32Palette = Palette<RGBA32>;

template<typename TColorType>
class Image final
{
public:
	using ColorType = TColorType;
	using PaletteType = Palette<TColorType>;

	Image() = default;

	Image(const PaletteType& palette, std::vector<std::uint8_t>&& pixels, unsigned int width, unsigned int height)
		: _palette(palette)
		, _pixels(std::move(pixels))
		, _width(width)
		, _height(height)
	{
		if ((_width * _height) != _pixels.size())
		{
			throw std::invalid_argument("width * height must equal pixels.size()");
		}
	}

	Image(const Image&) = default;
	Image& operator=(const Image&) = default;

	Image(Image&&) = default;
	Image& operator=(Image&&) = default;

	~Image() = default;

	const PaletteType& GetPalette() const { return _palette; }

	const std::vector<std::uint8_t>& GetPixels() const { return _pixels; }

	unsigned int GetWidth() const { return _width; }

	unsigned int GetHeight() const { return _height; }

private:
	PaletteType _palette;
	std::vector<std::uint8_t> _pixels;

	unsigned int _width;
	unsigned int _height;
};

using RGB24Image = Image<RGB24>;
using RGB32Image = Image<RGBA32>;

/**
*	@brief Given an image, creates a buffer of data that contains the image in a format that can be given to a graphics API
*/
template<typename TColorType>
std::vector<TColorType> CreateImageData(const Image<TColorType>& image)
{
	const auto& palette = image.GetPalette();
	const auto& pixels = image.GetPixels();

	std::vector<TColorType> data;

	data.resize(pixels.size());

	std::size_t index = 0;

	for (auto pixel : pixels)
	{
		data[index++] = palette[pixel];
	}

	return data;
}