#pragma once

#include <vector>

/// <summary>
/// Lattice Boltzmann Method, Bounce-Back boundary condition
/// D2Q9: 2 Dim, 9 Query(vectors)
/// </summary>
class FluidSolver
{
public:
	FluidSolver(int width, int height);
	void Step(float dt);
	void Reset();

	const float* GetRenderData() const { return m_RenderBuffer.data(); }

private:
	void CollideAndStream();
	void ApplyBoundaries();
	void UpdateVisuals();

	inline float GetVelocityU(int x, int y) const;
	inline float GetVelocityV(int x, int y) const;
	float GetEquilibrium(int i, int rho, float u, float v);
	int GetOppositeDirection(int i) const;

	int m_Width;
	int m_Height;
	float m_Time;
	float m_Tau; // viscosity

	// LMB Buffer: 9 floats per cell
	std::vector<float> m_F;
	std::vector<float> m_FNext;
	std::vector<bool> m_IsObstacle;
	std::vector<float> m_RenderBuffer;

	// D2Q9 constants
	// 0: Center
	// 1, 2, 3, 4: Axis
	// 5, 6, 7, 8: Diagonal
	const int dx[9] = { 0, 1, 0, -1, 0, 1, -1, -1, 1 };
	const int dy[9] = { 0, 0, 1, 0, -1, 1, 1, -1, -1 };
	const float weights[9] = { 4 / 9.f, 1 / 9.f, 1 / 9.f, 1 / 9.f, 1 / 9.f, 1 / 36.f, 1 / 36.f, 1 / 36.f, 1 / 36.f };
};