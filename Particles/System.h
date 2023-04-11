#ifndef SYSTEM_H
#define SYSTEM_H

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
	System(Vector2D tl, Vector2D br, size_t cnt, UINT pen = 0x1F2F6F7);

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

	// solves the collision of two particles
	void particleCollide(Particle* p1, Particle* p2);

	// updates all system information
	void update(const float dt);

	// display the system information and particles
	void display(ID2D1HwndRenderTarget* target);
};

#endif