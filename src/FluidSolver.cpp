#include "FluidSolver.h"

#include <algorithm>
#include <cmath>
#include <iostream>

FluidSolver::FluidSolver(int width, int height): m_Width(width), m_Height(height), m_Time(0.0f), m_Tau(0.505f)
{
	m_F.resize(width * height * 9, 0.0f);
	m_FNext.resize(width * height * 9, 0.0f);
	m_IsObstacle.resize(width * height * 9, false);
	m_RenderBuffer.resize(width * height * 3, 0.0f); // 3 for RGB

	// Initial states
	for(int i = 0; i < width * height; i++)
	{
		for(int d = 0; d < 9; d++)
		{
			m_F[i * 9 + d] = weights[d];
			m_FNext[i * 9 + d] = weights[d];
		}
	}

	// place cylinder obstacle
	int cx = width / 4, cy = height / 2, r = 45;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float distSq = (float)((x - cx) * (x - cx) + (y - cy) * (y - cy));
			if(distSq < r * r)
			{
				m_IsObstacle[y * width + x] = true;
			}
		}
	}
}

void FluidSolver::Step(float dt)
{
	m_Time += dt;

	CollideAndStream();
	ApplyBoundaries();
	UpdateVisuals();

	m_F.swap(m_FNext);
}

void FluidSolver::CollideAndStream()
{
	float omega = 1.0f / m_Tau;
	for(int y = 0; y < m_Height; y++)
	{
		for(int x = 0; x < m_Width; x++)
		{
			int idx = y * m_Width + x;
			if(m_IsObstacle[idx])
			{
				continue;
			}

			// Compute Macro density and velocity
			float rho = 0, u = 0, v = 0;
			for(int i = 0; i < 9; i++)
			{
				float fi = m_F[idx * 9 + i];
				rho += fi;
				u += fi * dx[i];
				v += fi * dy[i];
			}
			if(std::isnan(rho) || rho < 0.5f || rho > 1.5f)
			{
				rho = 1.0f;
				for(int i = 0; i < 9; i++)
				{
					m_F[idx * 9 + i] = weights[i];
				}
			}
			if(rho > 0)
			{
				u /= rho;
				v /= rho;
			}

			// Collide & stream
			for(int i = 0; i < 9; i++)
			{
				float dot = dx[i] * u + dy[i] * v;
				float fEq = weights[i] * rho * (1.0f + 3.0f * dot + 4.5f * dot * dot - 1.5f * (u * u + v * v));
				float fPost = m_F[idx * 9 + i] * (1.0f - omega) + fEq * omega;

				int nx = (x + dx[i] + m_Width) % m_Width;
				int ny = (y + dy[i] + m_Height) % m_Height;
				int targetIdx = (ny * m_Width + nx) * 9 + i;
				m_FNext[targetIdx] = fPost;
			}
		}
	}
}

void FluidSolver::ApplyBoundaries()
{
	// Inlet
	float u_inlet = 0.22f;
	for(int y = 0; y < m_Height; y++)
	{
		int idx = (y * m_Width) * 9;
		for(int d = 0; d < 9; d++)
		{
			m_FNext[idx + d] = GetEquilibrium(d, 1.0f, u_inlet, 0.0f);
		}
	}

	// Bounce-back
	for(int i = 0; i < m_Width * m_Height; i++)
	{
		if(m_IsObstacle[i])
		{
			for(int d = 0; d < 9; d++)
			{
				int opp = GetOppositeDirection(d);

				m_FNext[i * 9 + opp] = m_F[i * 9 + d];
			}
		}
	}

	// Outlet
	for(int y = 0; y < m_Height; y++)
	{
		int lastColIdx = (y * m_Width + (m_Width - 1)) * 9;
		int prefColIdx = (y * m_Width + (m_Width - 2)) * 9;
		for(int d = 0; d < 9; d++)
		{
			m_FNext[lastColIdx + d] = m_FNext[prefColIdx + d];
		}
	}
}

void FluidSolver::UpdateVisuals()
{
	for(int y = 1; y < m_Height - 1; y++)
	{
		for(int x = 1; x < m_Width - 1; x++)
		{
			int idx = y * m_Width + x;

			if(m_IsObstacle[idx])
			{
				// Color obstacles white
				m_RenderBuffer[idx * 3 + 0] = 1.0f;
				m_RenderBuffer[idx * 3 + 1] = 1.0f;
				m_RenderBuffer[idx * 3 + 2] = 1.0f;
				continue;
			}

			// Physical data
			float u = GetVelocityU(x, y);
			float v = GetVelocityV(x, y);
			float speed = std::sqrt(u * u + v * v);

			// Central differences
			float dv_dx = (GetVelocityV(x + 1, y) - GetVelocityV(x - 1, y)) * 0.5f;
			float du_dy = (GetVelocityU(x, y + 1) - GetVelocityU(x, y - 1)) * 0.5f;
			float vorticity = dv_dx - du_dy;

			// brightness vs speed
			float brightness = std::clamp(speed * 8.0f, 0.0f, 1.0f);

			// color vs vorticity
			float shift = std::clamp(vorticity * 35.0f, -0.5f, 0.5f);

			m_RenderBuffer[idx * 3 + 0] = brightness * (0.5f + shift);
			m_RenderBuffer[idx * 3 + 1] = brightness * 0.1f;
			m_RenderBuffer[idx * 3 + 2] = brightness * (0.5f - shift);
		}
	}
}

void FluidSolver::Reset()
{
	m_Time = 0.0f;

	for(int i = 0; i < m_Width * m_Height; i++)
	{
		for(int d = 0; d < 9; d++)
		{
			m_F[i * 9 + d] = weights[d];
			m_FNext[i * 9 + d] = weights[d];
		}
	}

	std::fill(m_RenderBuffer.begin(), m_RenderBuffer.end(), 0.0f);
}

inline float FluidSolver::GetVelocityU(int x, int y) const
{
	int idx = (y * m_Width + x) * 9;

	float rho = 0.0f;
	float u = 0.0f;
	for(int i = 0; i < 9; i++)
	{
		rho += m_F[idx + i];
		u += m_F[idx + i] * dx[i];
	}

	return (rho > 0.0f) ? u / rho : 0.0f;
}

inline float FluidSolver::GetVelocityV(int x, int y) const
{
	int idx = (y * m_Width + x) * 9;

	float rho = 0.0f;
	float v = 0.0f;
	for(int i = 0; i < 9; i++)
	{
		rho += m_F[idx + i];
		v += m_F[idx + i] * dy[i];
	}

	return (rho > 0.0f) ? v / rho : 0.0f;
}

float FluidSolver::GetEquilibrium(int i, int rho, float u, float v)
{
	float dot = (float)dx[i] * u + dy[i] * v;
	float uSq = u * u + v * v;

	return weights[i] * rho * (1.0f + 3.0f * dot + 4.5f * dot * dot - 1.5f * uSq);
}

int FluidSolver::GetOppositeDirection(int i) const
{
	// 0 <-> 0
	// 1 <-> 3
	// 2 <-> 4
	// 5 <-> 7
	// 6 <-> 8
	static const int opposites[] = { 0, 3, 4, 1, 2, 7, 8, 5, 6 };

	if(i < 0 || i > 8)
	{
		return 0;
	}

	return opposites[i];
}
