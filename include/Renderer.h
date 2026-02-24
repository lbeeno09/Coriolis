#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();

	void UpdateTexture(const float* data);
	GLuint GetTextureID() const { return m_textureID; }

private:
	GLuint m_textureID;
	int m_Width, m_Height;
};