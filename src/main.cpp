// Coriolis.cpp : Defines the entry point for the application.
//

#include "Coriolis.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "FluidSolver.h"
#include "SimSettings.h"
#include "Renderer.h"

#include <algorithm>
#include <iostream>

static void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char*[])
{
	glfwSetErrorCallback(glfwErrorCallback);
	if(!glfwInit())
	{
		return -1;
	}

	// OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create windows with graphics  context
	float mainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
	GLFWwindow* window = glfwCreateWindow((int)(1280 * mainScale), (int)(800 * mainScale), "Coriolis", nullptr, nullptr);
	if(window == nullptr)
	{
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Init glad
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize glad" << std::endl;
		return -1;
	}

	// Setup Solver, Renderer, etc
	const int GRID_W = 256;
	const int GRID_H = 256;
	FluidSolver solver(GRID_W, GRID_H);
	Renderer renderer(GRID_W, GRID_H);
	SimulationSettings settings;

	solver.SetObstacle(GRID_W / 4, GRID_H / 2, 12);

	bool isPaused = false;


	// Setup Dear ImGui Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui Style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(mainScale);
	style.FontScaleDpi = mainScale;
	// for OpenGL 3.3+
	io.ConfigDpiScaleFonts = true;
	io.ConfigDpiScaleViewports = true;

	// Tweak WindowRounding/WindowBg when viewport is on
	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");

	// Load Fonts
	// - If fonts are not explicitly loaded, Dear ImGui will select an embedded font: either AddFontDefaultVector() or AddFontDefaultBitmap().
	//   This selection is based on (style.FontSizeBase * style.FontScaleMain * style.FontScaleDpi) reaching a small threshold.
	// - You can load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - If a file cannot be loaded, AddFont functions will return a nullptr. Please handle those errors in your code (e.g. use an assertion, display an error and quit).
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use FreeType for higher quality font rendering.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	// - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
	//style.FontSizeBase = 20.0f;
	//io.Fonts->AddFontDefaultVector();
	//io.Fonts->AddFontDefaultBitmap();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
	//IM_ASSERT(font != nullptr);

	// Main Loop
	while(!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();
		if(glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}


		// Update Physics
		if(!isPaused)
		{
			wfSolver.Update(settings);
		}

		// Pass Pointer to Solver to Renderer
		renderer.UpdateData(wfSolver.GetDensityData());
		renderer.Render();

		// UI Render
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		// Control Panel
		{
			ImGui::Begin("Settings");

			ImGui::Checkbox("Pause", &isPaused);
			if(ImGui::Button("Reset"))
			{
				wfSolver.Reset();
			}

			ImGui::Separator();
			ImGui::Text("Constants");
			ImGui::SliderFloat("dt(Time Step)", &settings.timeStep, 0.0001f, 1.0f, "%.4f");
			ImGui::SliderFloat("Mass", &settings.mass, 0.1f, 10.0f);

			ImGui::Separator();
			ImGui::Text("Wave Packets Settings");
			ImGui::SliderFloat("Spread", &settings.spawnSigma, 1.0f, 20.0f);
			ImGui::SliderFloat("Vel X", &settings.spawnKx, -5.0f, 5.0f);
			ImGui::SliderFloat("Vel Y", &settings.spawnKy, -5.0f, 5.0f);

			ImGui::End();
		}

		// Viewport Panel
		{
			ImGui::Begin("Simulation");

			ImVec2 vPos = ImGui::GetCursorScreenPos();
			ImVec2 vSize = ImGui::GetContentRegionAvail();

			std::cout << vSize.x << ", " << vSize.y << std::endl;
			GLuint textureID = renderer.GetOutputTexture();
			ImGui::Image((void*)(intptr_t)textureID, vSize, ImVec2(0, 1), ImVec2(1, 0));

			if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				ImVec2 mousePos = ImGui::GetMousePos();
				
				float simX = static_cast<float>(mousePos.x - vPos.x) / static_cast<float>(vSize.x);
				float simY = 1.0f - static_cast<float>(mousePos.y - vPos.y) / static_cast<float>(vSize.y);

				simX = std::clamp(simX, 0.0f, 1.0f);
				simY = std::clamp(simY, 0.0f, 1.0f);

				wfSolver.SpawnPacket(simX, simY, settings);

				//std::cout << "Click detected at: " << simX << ", " << simY << std::endl;
				std::cout << wfSolver.GetWidth() << ", " << wfSolver.GetHeight() << std::endl;
			}

			ImGui::End();
		}

		// Render Screen
		ImGui::Render();
		int displayW, displayH;
		glfwGetFramebufferSize(window, &displayW, &displayH);
		glViewport(0, 0, displayW, displayH);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional platform windows
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
