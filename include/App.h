#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"

#include <memory>

class FluidSolver;
class WFSolver;
class Renderer;

class App
{
public:
	App();
	~App();

	void Run();


private:
	// Lifecycle
	bool Init();
	void Shutdown();

	// Loop Stages
	void NewFrame();
	void Update(float dt);
	void RenderUI();
	void EndFrame();

	GLFWwindow* window;
	bool isRunning;

	std::unique_ptr<Renderer> m_Renderer;
	//std::unique_ptr<FluidSolver> m_FluidSolver;
	std::unique_ptr<WFSolver> m_WFSolver;

	int m_SimWidth = 600;
	int m_SimHeight = 300;
	float m_TimeScale = 1.0f;
	bool m_IsPaused = false;

	// Our state
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};