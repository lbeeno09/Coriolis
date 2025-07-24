#include "coriolis/particle.h"
#include "coriolis/precision.h"

#include <cassert>

using namespace coriolis;

void Particle::integrate(real duration)
{
	if(inverseMass <= 0.0f) return;

	assert(duration > 0.0f);

	position.addScaledVector(velocity, duration);

	Vector3 resultingAcc = acceleration;

	velocity.addScaledVector(resultingAcc, duration);
	velocity *= real_pow(damping, duration);

	clearAccumulator();
}


void Particle::setMass(const real mass)
{
	assert(mass != 0);

	this->inverseMass = ((real)1.0) / mass;
}

real Particle::getMass() const
{
	if(inverseMass == 0)
	{
		return REAL_MAX;
	}
	else
	{
		return ((real)1.0) / this->inverseMass;
	}
}


void Particle::setInverseMass(const real inverseMass)
{
	this->inverseMass = inverseMass;
}

real Particle::getInverseMass() const
{
	return this->inverseMass;
}


bool Particle::hasFiniteMass() const
{
	return this->inverseMass > 0.0f;
}


void Particle::setDamping(const real damping)
{
	this->damping = damping;
}

real Particle::getDamping() const
{
	return this->damping;
}

void Particle::setPosition(const Vector3 &position)
{
	this->position = position;
}

void Particle::setPosition(const real x, const real y, const real z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

void  Particle::getPosition(Vector3 *position) const
{
	*position = this->position;
}

Vector3 Particle::getPosition() const
{
	return this->position;
}


void Particle::setVelocity(const Vector3 &velocity)
{
	this->velocity = velocity;
}

void Particle::setVelocity(const real x, const real y, const real z)
{
	this->velocity.x = x;
	this->velocity.y = y;
	this->velocity.z = z;
}

void Particle::getVelocity(Vector3 *velocity) const
{
	*velocity = this->velocity;
}

Vector3 Particle::getVelocity() const
{
	return this->velocity;
}


void Particle::setAcceleration(const Vector3 &acceleration)
{
	this->acceleration = acceleration;
}

void Particle::setAcceleration(const real x, const real y, const real z)
{
	this->acceleration.x = x;
	this->acceleration.y = y;
	this->acceleration.z = z;
}

void Particle::getAcceleration(Vector3 *acceleration) const
{
	*acceleration = this->acceleration;
}

Vector3 Particle::getAcceleration() const
{
	return this->acceleration;
}


void Particle::addForce(const Vector3 &force)
{
	this->forceAccum += force;
}

void Particle::clearAccumulator()
{
	this->forceAccum.clear();
}
