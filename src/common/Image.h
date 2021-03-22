#pragma once
#include <string>
#include <functional>
#include "glm/glm.hpp"
#include "glm/gtx/scalar_multiplication.hpp"
// code from: https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/

enum ImageAllocation
{
	UNDEFINED_ALLOC = -1, NONE, LOAD_PPM, LOAD_PNG
};

struct ImageSpecification
{
	int width, height, channels;
	ImageAllocation allocation;
};

struct Pixel
{
	uint8_t r, g, b;
	uint8_t a = 255;

	Pixel() = default;
	Pixel(const float* kv) : r((uint8_t)kv[0]), g((uint8_t)kv[0]), b((uint8_t)kv[0]) {}
	Pixel(const std::initializer_list<uint8_t>& other);
	Pixel operator=(const float* kv);
	Pixel operator=(const std::initializer_list<uint8_t>& other);

	bool operator==(const Pixel& other) { return r == other.r && g == other.g && b == other.b; }
	bool operator!=(const Pixel& other) { return !(r == other.r && g == other.g && b == other.b); }
};

class Image
{
public:
private:
	Pixel* m_Data = NULL;
	ImageSpecification m_Specs = { 0, 0, 3 , ImageAllocation::NONE };

public:
	Image(const ImageSpecification& specs = { 0, 0, 3 , ImageAllocation::NONE }, Pixel* data = NULL);
	Image(const Image& other);
	~Image();

	Pixel* data() const { return m_Data; }
	const ImageSpecification get_specs() const { return m_Specs; }
	void set_specs(const ImageSpecification& specs) { m_Specs = specs; }
	const int width() const { return m_Specs.width; }
	const int height() const { return m_Specs.height; }
	const int size() const { return width() * height(); }
	const int channels() const { return m_Specs.channels; }
	Pixel& get(int x, int y) const { return m_Data[x * width() + y]; }
	void set(int x, int y, const Pixel& pixel) { m_Data[x * width() + y] = pixel; }
	void clear();

	Image& operator=(const Image& other);
	bool operator==(const Image& other);

};

bool load_ppm(Image& img, const char* path);
bool save_ppm(const Image& img, const char* path);
bool load_png(Image& img, const char* path);
bool save_png(Image& img, const char* path);
