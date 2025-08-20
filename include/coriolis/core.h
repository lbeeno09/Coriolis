#pragma once

#include "precision.h"

#include <cmath>

namespace coriolis
{

class Vector3
{
public:
	/* Constructors */
	Vector3() : x(0), y(0), z(0) { }

	Vector3(const real x, const real y, const real z) { }


	/* Operators */
	/* Index */
	real operator[](unsigned i) const
	{
		if(i == 0) return x;
		if(i == 1) return y;
		return z;
	}

	real &operator[](unsigned i)
	{
		if(i == 0) return x;
		if(i == 1) return y;
		return z;
	}

	/* Four Basic Ops*/
	Vector3 operator+(const Vector3 &v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	void operator+=(const Vector3 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}


	Vector3 operator-(const Vector3 &v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	void operator-=(const Vector3 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}


	Vector3 operator*(const real value) const
	{
		return Vector3(x * value, y * value, z * value);
	}

	void operator*=(const real value)
	{
		x *= value;
		y *= value;
		z *= value;
	}


	Vector3 componentProduct(const Vector3 &vector) const
	{
		return Vector3(x * vector.x, y * vector.y, z * vector.z);
	}

	void componentVectorUpdate(const Vector3 &vector)
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;
	}

	Vector3 vectorProduct(const Vector3 &vector) const
	{
		return Vector3(y * vector.z - z * vector.y,
			z * vector.x - x * vector.z,
			x * vector.y - y * vector.z);
	}


	Vector3 operator%(const Vector3 &vector) const
	{
		return Vector3(y * vector.z - z * vector.y,
			z * vector.x - x * vector.z,
			x * vector.y - y * vector.z);
	}

	void operator%=(const Vector3 &vector)
	{
		*this = vectorProduct(vector);
	}


	real scalerProduct(const Vector3 &vector) const
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}

	real operator*(const Vector3 &vector) const
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}



	/* Other Methods */
	void addScaledVector(const Vector3 &vector, real scale)
	{
		x += vector.x * scale;
		y += vector.y * scale;
		z += vector.z * scale;
	}


	real magnitude() const
	{
		return real_sqrt(x * x + y * y + z * z);
	}

	real squareMagnitude() const
	{
		return x * x + y * y + z * z;
	}

	void normalize()
	{
		real l = magnitude();
		if(l > 0)
		{
			(*this) *= (real)1.0 / l;
		}
	}


	bool operator==(const Vector3 &other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const Vector3 &other) const
	{
		return !(*this == other);
	}


	bool operator<(const Vector3 &other) const
	{
		return x < other.x && y < other.y && z < other.z;
	}

	bool operator<=(const Vector3 &other) const
	{
		return x <= other.x && y <= other.y && z <= other.z;
	}


	bool operator>(const Vector3 &other) const
	{
		return x > other.x && y > other.y && z > other.z;
	}

	bool operator>=(const Vector3 &other) const
	{
		return x >= other.x && y >= other.y && z >= other.z;
	}


	void clear()
	{
		x = y = z = 0;
	}

	real invert()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	real x;
	real y;
	real z;

private:
	real pad;
};

}