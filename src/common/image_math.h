#pragma once
#include "glm/glm.hpp"

using namespace glm;
class Pixel;
class Image;

Image flip_horizontal(const Image& img);
Image flip_vertical(const Image& img);
Image gamma_correct(const Image& img, float gamma);
Image resize(const Image& img, int width, int height);
Image grayscale(const Image& img);
Image alpha_blend(const Image& img1, const Image& img2, float alpha);
Image subimage(const Image& img, int row, int column, int width, int height);
Image replace(const Image& img1, const Image& img2, int row, int column);

Image edge_detect(const Image& img, float cutoff);
Image threshold(const Image& img, float cutoff);
Image apply_kernel(const Image& img, const float* kernel, int width, int height, std::function<float* (float[3])> post);
Image channel_red(const Image& img);
Image channel_green(const Image& img);
Image channel_blue(const Image& img);

float conv_mat3(const mat3& matA, const mat3& matB);
float conv_mat4(const mat4& matA, const mat4& matB);
float sigmoid(float x);
float* convolution(const Image& img, int i, int j, const float* kernel, int kwidth, int kheight);

// post kernel functions
float* sig_post(float* kv);
float* clamp_post(float* kv);
float* abs_post(float* kv);
float* max_rgb_post(float* kv);

