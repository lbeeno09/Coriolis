#pragma once

#include <complex>
#include <vector>

class WFSolver
{
public:
	struct Params
	{
		int resX = 256;
		int resY = 256;
		float dt = 0.02f;
		float hBar = 1.0f;
		float mass = 1.0f;
	};

	WFSolver(const Params& param);

	void Step();
	void InjectWavePacket(int mouseX, int mouseY, float kx, float ky);

	const std::vector<float>& GetHeightMap() const { return m_HeightMap; }

	void Reset();

private:
	Params m_Param;

	std::vector<std::complex<float>> m_Psi;
	std::vector<std::complex<float>> m_PsiNext;
	std::vector<float> m_Potential;
	std::vector<float> m_HeightMap;
	
	void ApplyHamiltonian();
	void Normalize();
};