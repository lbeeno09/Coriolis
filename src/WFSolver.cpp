#include "WFSolver.h"

#include <cmath>

WFSolver::WFSolver(const Params& param) : m_Param(param)
{
	m_Psi.resize(param.resX * param.resY, 0.0f);
	m_PsiNext.resize(param.resX * param.resY, 0.0f);
	m_Potential.resize(param.resX * param.resY, 0.0f);
	m_HeightMap.resize(param.resX * param.resY, 0.0f);

	for(int y = 0; y < m_Param.resY; y++)
	{
		for(int x = 120; x < 130; x++)
		{
			m_Potential[y * m_Param.resX + x] = 5.0f;
		}
	}
}

void WFSolver::Step()
{
	ApplyHamiltonian();

	m_Psi.swap(m_PsiNext);

	Normalize();

	for(size_t i = 0; i < m_Psi.size(); i++)
	{
		m_HeightMap[i] = std::norm(m_Psi[i]) * 100.0f;
	}
}

void WFSolver::InjectWavePacket(int mouseX, int mouseY, float kx, float ky)
{
	float sigma = 10.0f;
	for(int y = 0; y < m_Param.resY; y++)
	{
		for(int x = 0; x < m_Param.resX; x++)
		{
			float dx = (float)x - mouseX;
			float dy = (float)y - mouseY;

			float envelope = std::exp(-(dx * dx + dy * dy) / (2.0f * sigma * sigma));
			float phase = kx * (float)x + ky * (float)y;

			m_Psi[y * m_Param.resX + x] += std::polar(envelope, phase);
		}
	}
}

void WFSolver::ApplyHamiltonian()
{
	float lapCoef = (m_Param.hBar * m_Param.dt) / (2.0f * m_Param.mass));
	float potCoef = m_Param.dt / m_Param.hBar;

	for(int y = 1; y < m_Param.resY - 1; y++)
	{
		for(int x = 1; x < m_Param.resX - 1; x++)
		{
			int idx = y * m_Param.resX + x;

			std::complex<float> laplacian = m_Psi[idx + 1] + m_Psi[idx - 1] + m_Psi[idx + m_Param.resX] + m_Psi[idx - m_Param.resX] - 4.0f * m_Psi[idx];

			float V = m_Potential[idx];

			float HPsiReal = -lapCoef + laplacian.real() + V * m_Psi[idx].real() * potCoef;
			float HPsiImag = -lapCoef + laplacian.imag() + V * m_Psi[idx].imag() * potCoef;

			m_PsiNext[idx] = m_Psi[idx] + std::complex<float>(HPsiImag, -HPsiReal);
		}
	}
}

void WFSolver::Normalize()
{
	float totalProb = 0.0f;
	for(const auto& p : m_Psi)
	{
		totalProb += std::norm(p);
	}

	if(totalProb > 1e-9f)
	{
		float factor = 1.0f / std::sqrt(totalProb);
		for(auto& p : m_Psi)
		{
			p *= factor;
		}
	}
}

void WFSolver::Reset()
{
	m_Psi.resize(m_Param.resX * m_Param.resY, 0.0f);
	m_PsiNext.resize(m_Param.resX * m_Param.resY, 0.0f);
	m_HeightMap.resize(m_Param.resX * m_Param.resY, 0.0f);
}
