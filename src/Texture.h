#pragma once
#include "Image.h"

class Texture
{
private:
	uint32_t m_RendererID;
	
public:
	Texture(const Image& img);
	~Texture();

	void Bind(); 
	void Unbind();

	const uint32_t GetID() const { return m_RendererID; }
};