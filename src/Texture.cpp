#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Texture.h"


Texture::Texture(const Image& image)
{
    std::cout << "Loaded image: " << image.width() << "x" << image.height() << std::endl;

    glEnable(GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, image.width(), image.height());
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width(), image.height(),
        GL_RGB, GL_UNSIGNED_BYTE, image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE0);
}

Texture::~Texture()
{

}

void Texture::Bind()
{
}

void Texture::Unbind()
{

}
