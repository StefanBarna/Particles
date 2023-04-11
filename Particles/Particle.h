#ifndef PARTICLE_H
#define PARTICLE_H

#include <d2d1.h>
#include "Vector2D.h"

// TODO: have a map connecting element names to particular colours!
// notice: color is purely an internal value that is dealt with by display alone, and thus will never have to be retrieved
// notice: vector2d queries do not need to be constant, as their internal queries are themseleves constant, and thus this enables more versatility in the query, as you can also call the mutator function from the query function!

class Particle {
	Vector2D m_pos;	// position
	Vector2D m_vel;	// velocity
	float m_mass;	// mass
	float m_rad;	// radius
	UINT32 m_col;	// colour

public:
	// class constructors
	Particle() : m_pos(), m_vel(), m_mass(0), m_rad(0), m_col(0) {};
	Particle(Vector2D pos, Vector2D vel, float mass, float rad, UINT32 col) : m_pos(pos), m_vel(vel), m_mass(mass), m_rad(rad), m_col(col) {};

	// queries
	Vector2D& pos();
	Vector2D& vel();
	float mass() const;
	float rad() const;
	UINT32 col() const;

	// mutators
	void setPos(const Vector2D p);
	void setVel(const Vector2D v);
	void setMass(const float m);
	void setRad(const float r);
	void setCol(const UINT c);

	// advances particle position in accordance to time elapsed and velocity
	void update(float dt);

	// display the particle at its position
	void display(ID2D1HwndRenderTarget* target);
};

#endif