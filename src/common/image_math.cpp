#include "Log.h"
#include "image_math.h"
#include "Image.h"

float conv_mat3(const mat3& matA, const mat3& matB)
{
	float result = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			result += matA[i][j] * matB[2-i][2-j];
	return result;
}

float conv_mat4(const mat4& matA, const mat4& matB)
{
	float result = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result += matA[i][j] * matB[j][i];
	return result;
}

// applies the convolution operation to image section
float* convolution(const Image& img, int i, int j, const float* kernel, int kwidth, int kheight)
{
	CORE_ASSERT(i + kheight < img.height() && j + kwidth < img.width(), "Invalid indices!");
	// iterate through kernel
	float* kv = new float[3]{ 0.0f, 0.0f, 0.0f };
	for (int k = 0; k < kheight; k++)
	{
		for (int l = 0; l < kwidth; l++)
		{
			Pixel p = img.get(i + k, j + l);
			kv[0] += kernel[(kheight - k - 1) * kwidth + kwidth - l - 1] * p.r;
			kv[1] += kernel[(kheight - k - 1) * kwidth + kwidth - l - 1] * p.g;
			kv[2] += kernel[(kheight - k - 1) * kwidth + kwidth - l - 1] * p.b;
		}
	}
	return kv;
}

// sigmoid with range (0,255)
float sigmoid(float x)
{
	return 255 / (1 + pow(2.718, (-x + 100) / 40));
}

Image flip_horizontal(const Image& img)
{
	Image flipped(img.get_specs());
	for (int i = 0; i < img.height(); i++)
	{
		for (int j = 0; j < img.width(); j++)
		{
			flipped.set(i, j, img.get(i, img.width() - j - 1));
		}
	}
	return flipped;
}

Image flip_vertical(const Image& img)
{
	Image flipped(img.get_specs());
	for (int i = 0; i < img.height(); i++)
	{
		for (int j = 0; j < img.width(); j++)
		{
			flipped.set(i, j, img.get(img.height() - i - 1, j));
		}
	}
	return flipped;
}

Image gamma_correct(const Image& img, float gamma)
{
	float gc = 1 / gamma;
	Image new_gamma(img.get_specs());
	for (int i = 0; i < img.height(); i++)
	{
		for (int j = 0; j < img.width(); j++)
		{
			Pixel p = img.get(i, j);
			new_gamma.set(i, j, {
				uint8_t(255.0f * pow(p.r / 255.0f, gc)),
				uint8_t(255.0f * pow(p.g / 255.0f, gc)),
				uint8_t(255.0f * pow(p.b / 255.0f, gc)) });
		}
	}
	return new_gamma;
}

Image grayscale(const Image& img)
{
	Image gray(img.get_specs());
	for (int i = 0; i < img.height(); i++)
	{
		for (int j = 0; j < img.width(); j++)
		{
			Pixel p = img.get(i, j);
			uint8_t avg = (p.r + p.g + p.b) / 3;
			gray.set(i, j, { avg, avg, avg });
		}
	}
	return gray;
}

Image alpha_blend(const Image& img1, const Image& img2, float alpha)
{
	CORE_VALIDATE(img2.width() == img1.width() && img2.height() == img1.height(),
		return Image(img1.get_specs()), "Dimensions of images must be the same!");
	Image blended(img1.get_specs());
	for (int i = 0; i < img1.height(); i++)
	{
		for (int j = 0; j < img1.width(); j++)
		{
			Pixel p = img1.get(i, j);
			Pixel po = img2.get(i, j);
			Pixel bp = {
				uint8_t((float)p.r * (1 - alpha) + (float)po.r * alpha),
				uint8_t((float)p.g * (1 - alpha) + (float)po.g * alpha),
				uint8_t((float)p.b * (1 - alpha) + (float)po.b * alpha)
			};

			blended.set(i, j, bp);
		}
	}
	return blended;
}

Image subimage(const Image& img, int row, int column, int width, int height)
{
	CORE_VALIDATE(row + height <= img.height() && column + width <= img.width(),
		return Image(img.get_specs()), "Parameters out of bounds of image!");
	Image sub({ width, height, img.channels() });
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Pixel p = img.get(i + row, j + column);
			sub.set(i, j, p);
		}
	}
	return sub;
}

Image replace(const Image& img1, const Image& img2, int row, int column)
{
	ImageSpecification spec = {
		glm::max(row + img2.width(), img1.width()),
		glm::max(column + img2.height(), img1.height()),
		img1.channels()
	};
	Image repl(spec);
	for (int i = 0; i < img1.height(); i++)
	{
		for (int j = 0; j < img1.width(); j++)
		{
			repl.set(i, j, img1.get(i, j));
		}
	}
	for (int i = 0; i < img2.height(); i++)
	{
		for (int j = 0; j < img2.width(); j++)
		{
			repl.set(i + row, j + column, img2.get(i, j));
		}
	}
	return repl;
}

Image resize(const Image& img, int width, int height)
{
	float jfac = (float)img.width() / (float)width;
	float ifac = (float)img.height() / (float)height;
	Image resized({ width, height, img.channels() });
	for (int i = 0; i < resized.height(); i++)
	{
		for (int j = 0; j < resized.width(); j++)
		{
			Pixel p = img.get(ifac * (float)i, jfac * (float)j);
			resized.set(i, j, p);
		}
	}
	return resized;
}

Image edge_detect(const Image& img, float cutoff)
{
	float gx[9] = {
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1
	};
	float gy[9] = {
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1
	};

	int newWidth = img.width() - 3;
	int newHeight = img.height() - 3;
	ImageSpecification spec = { newWidth, newHeight, img.channels() };
	Image result(spec);
	CORE_VALIDATE(3 <= img.width() && 3 <= img.height(),
		return Image(), "Image must be larger than kernel!");

	// iterate through image pixels
	for (int i = 0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
		{
			float* kx = convolution(img, i, j, gx, 3, 3);
			float* ky = convolution(img, i, j, gy, 3, 3);

			//Pixel px = abs_post(kx);
			//Pixel py = abs_post(ky);
			Pixel p = {
				(uint8_t)sqrt(pow(kx[0], 2) + pow(ky[0], 2)),
				(uint8_t)sqrt(pow(kx[1], 2) + pow(ky[1], 2)),
				(uint8_t)sqrt(pow(kx[2], 2) + pow(ky[2], 2)),
				(uint8_t)255
			};
			result.set(i, j, p);
		}
	}
	return threshold(result, cutoff);
}

// cutoff is a float in range [0, 1]
Image threshold(const Image& img, float cutoff)
{
	Image result(img.get_specs());
	for (int i = 0; i < img.height(); i++)
	{
		for (int j = 0; j < img.width(); j++)
		{
			Pixel p = img.get(i, j);
			if (float(p.r + p.g + p.b) / (3.0f * 255.0f) > cutoff)
				result.set(i, j, p);
		}
	}
	return result;
}

Image apply_kernel(const Image& img, const float* kernel, int kwidth, int kheight, std::function<float* (float[3])> post)
{
	CORE_ASSERT(img.width() > kwidth && img.height() > kheight, "Image must be larger than kernel!");
	int newWidth = img.width() - kwidth;
	int newHeight = img.height() - kheight;
	ImageSpecification spec = { newWidth, newHeight, img.channels() };
	Image result(spec);

	// iterate through image pixels
	for (int i = 0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
		{
			float* kv = convolution(img, i, j, kernel, kwidth, kheight);
			Pixel p = abs_post(kv);
			p.a = img.get(i, j).a;
			result.set(i, j, p);
		}
	}
	return result;
}

Image channel_red(const Image& img)
{
	Image result = Image(img.get_specs());
	for (int i = 0; i < img.height(); i++)
	{
		for (int j = 0; j < img.width(); j++)
		{
			result.set(i, j, { img.get(i, j).r, 0, 0 });
		}
	}
	return result;
}

Image channel_green(const Image& img)
{
	Image result = Image(img.get_specs());
	for (int i = 0; i < img.height(); i++)
	{
		for (int j = 0; j < img.width(); j++)
		{
			result.set(i, j, { 0, img.get(i, j).g, 0 });
		}
	}
	return result;
}

Image channel_blue(const Image& img)
{
	Image result = Image(img.get_specs());
	for (int i = 0; i < img.height(); i++)
	{
		for (int j = 0; j < img.width(); j++)
		{
			result.set(i, j, { 0, 0, img.get(i, j).b });
		}
	}
	return result;
}

float* sig_post(float* kv)
{
	return new float[3]{
		sigmoid(kv[0]),
		sigmoid(kv[1]),
		sigmoid(kv[2])
	};
}

float* clamp_post(float* kv)
{
	return new float[3]{
		min(255.0f, max(0.0f, kv[0])),
		min(255.0f, max(0.0f, kv[1])),
		min(255.0f, max(0.0f, kv[2]))
	};
}

float* abs_post(float* kv)
{
	return new float[3]{
		min(255.0f, abs(kv[0])),
		min(255.0f, abs(kv[1])),
		min(255.0f, abs(kv[2]))
	};
}

float* max_rgb_post(float* kv)
{
	float mv = max(abs(kv[0]), max(abs(kv[1]), abs(kv[2])));
	return new float[3]{ mv, mv, mv };
}