#include "Renderer.h"

Renderer::Renderer(int width, int height) : m_Width(width), m_Height(height)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Set Texture Parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, nullptr);
}

Renderer::~Renderer()
{
	glDeleteTextures(1, &m_textureID);
}

void Renderer::UpdateTexture(const float* data)
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_FLOAT, data);
}
