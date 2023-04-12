#ifndef SYSTEM_H
#define SYSTEM_H

#include <algorithm>
#include <cmath>
#include <list>
#include <utility>
#include <random>
#include "Particle.h"

#define m_width (m_br.x - tl.x)
#define m_height (m_br.y - tl.y)

class System {
	Vector2D m_tl;	// top left corner coordinate
	Vector2D m_br;	// bottom right corner coordinate
	UINT32 m_pen;	// system pen colour

	Particle** m_particles;	// collection of particles on screen
	size_t m_cnt;			// number of particles on screen

public:
	// class constructor
	System(Vector2D tl, Vector2D br, size_t cnt, UINT pen = 0xa4cbdb);

	// queries
	Vector2D gettl() const;
	Vector2D getbr() const;

	// mutators
	void settl(const Vector2D tl);
	void setbr(const Vector2D br);

	// linear interpolation between two coordinates; t -> [0, 1]
	float lerpt(const float veci, const float vecf, const float t);	// point in time is known
	float lerpp(const float veci, const float vecf, const float p);	// intermediate position is known

	// checks for particle collisions with border
	void borderCheck(const float dt);

	// employs sweep and prune algorithm to deduce possible collisions
	std::list<std::pair<Particle*, Particle*>> broadDetect();

	// checks that possible collisions are actual collisions and deals with them
	void narrowDetect(std::list<std::pair<Particle*, Particle*>> col, const float dt);

	// resolves overlap of two colliding particles; returns the point at which the collision occurs
	float resolveOverlap(Particle* p1, Particle* p2, const float dt);

	// solves the posiiton of two particles after collision
	void particleCollide(Particle* p1, Particle* p2, const float dt, const float t);

	// updates all system information
	void update(const float dt);

	// display the system information and particles
	void display(ID2D1HwndRenderTarget* target);
};

#endif