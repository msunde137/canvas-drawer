#include <filesystem>
#include <fstream>
#include <functional>
#include "Log.h"
#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "STB/stb_image_write.h"

namespace fs = std::filesystem;

Image::Image(const ImageSpecification& specs, Pixel* data)
	: m_Specs(specs), m_Data(data)
{
	if (m_Data)
		return;

	m_Data = (Pixel*)malloc(height() * width() * sizeof(Pixel));
	for (int i = 0; i < height() * width(); i++)
		m_Data[i] = { 0, 0, 0, 255 };
}

Image::Image(const Image& other)
	: m_Specs(other.get_specs())
{
	m_Data = (Pixel*)malloc(height() * width() * sizeof(Pixel));
	for (int i = 0; i < height() * width(); i++)
		m_Data[i] = other.data()[i];
}

Image::~Image()
{
	clear();
}

void Image::clear()
{
	switch (m_Specs.allocation) {
	case LOAD_PNG:
		stbi_image_free(m_Data); return;
	case LOAD_PPM:
		free(m_Data); return;
	case NONE:
		free(m_Data); return;
	}
	CORE_ERROR("Unknown AllocationType!");
}

Image& Image::operator=(const Image& other)
{
	if (*this == other)
		return *this;

	clear();
	set_specs(other.get_specs());
	m_Data = (Pixel*)malloc(height() * width() * sizeof(Pixel));
	for (int i = 0; i < height() * width(); i++)
		m_Data[i] = other.data()[i];
	return *this;
}

bool Image::operator==(const Image& other)
{
	bool result = true;
	result *= other.height() == height();
	result *= other.width() == width();
	result *= other.channels() == channels();
	if (!result)
		return false;
	for (int i = 0; i < height(); i++)
		for (int j = 0; j < width(); j++)
			result *= other.get(i, j) == get(i, j);
	return result;
}

// loads ppm image
// only supports 'P3' spec
bool load_ppm(Image& img, const char* path)
{
	FILE* file;
	int is_eof = 0;
	fopen_s(&file, path, "rb");
	CORE_ASSERT(file, "Filepath doesn't exist!");

	char spec[16];
	int width, height, max;
	is_eof = fscanf(file, "%s", spec);
	is_eof = fscanf(file, "%i %i", &width, &height);
	is_eof = fscanf(file, "%i", &max);
	CORE_VALIDATE(is_eof != EOF, return false, "Image contains no data!");
	CORE_VALIDATE(std::string(spec) == "P3", return false, "Only supports 'P3' specification!");

	Pixel* data = (Pixel*)malloc(height * width * sizeof(Pixel));
	for (int i = 0; i < height * width; i++)
	{
		uint32_t r, g, b;
		is_eof = fscanf(file, "%u", &r);
		is_eof = fscanf(file, "%u", &g);
		is_eof = fscanf(file, "%u", &b);

		CORE_VALIDATE(is_eof != EOF, return false, "Image data corrupted, can't read file!");
		data[i] = { (uint8_t)r, (uint8_t)g, (uint8_t)b, 255 };

	}
	fclose(file);

	img = Image({ width, height, 3, LOAD_PPM }, data);
	return true;
}

// saves Image to .ppm format
// only supports 'P3' spec
bool save_ppm(const Image& img, const char* path)
{
	std::ofstream imgFile(path);
	std::string spec = "P3";
	imgFile << spec << std::endl;
	imgFile << img.width() << " " << img.height() << std::endl;
	imgFile << 255 << std::endl;
	for (int i = 0; i < img.height(); i++)
	{
		for (int j = 0; j < img.width(); j++)
		{
			Pixel pixel = img.get(i, j);
			imgFile << (int)pixel.r << std::endl << (int)pixel.g << std::endl << (int)pixel.b << std::endl;
		}
	}
	imgFile.close();
	return true;
}

// loads png image using stb library
bool load_png(Image& img, const char* path)
{
	CORE_VALIDATE(fs::exists(path), return false, "Filepath doesn't exist!");
	int width, height, channels;
	Pixel* data = (Pixel*)stbi_load(path, &width, &height, &channels, 4);
	CORE_VALIDATE(data, return false, "Unable to load image!");
	img = Image({ width, height, channels, LOAD_PNG }, data);
}

// saves Image to png using stb library
bool save_png(Image& img, const char* path)
{
	// had to keep comp at 4 for some reason ...
	int result = stbi_write_png(path, img.width(), img.height(), 4, (uint32_t*)img.data(), img.width() * sizeof(Pixel));
	return result;
}

Pixel::Pixel(const std::initializer_list<uint8_t>& other)
{
	CORE_ASSERT(other.size() <= sizeof(Pixel), "Too many elements in initializer list!");
	memcpy(this, other.begin(), other.size());
}

Pixel Pixel::operator=(const float* kv)
{
	*this = { (uint8_t)kv[0], (uint8_t)kv[1], (uint8_t)kv[2] };
	return *this;
}

Pixel Pixel::operator=(const std::initializer_list<uint8_t>& other)
{
	CORE_ASSERT(other.size() <= sizeof(Pixel), "Too many elements in initializer list!");
	memcpy(this, other.begin(), other.size());
	return *this;
}