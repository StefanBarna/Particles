#include "System.h"

System::System(Vector2D tl, Vector2D br, size_t cnt, UINT pen) :
	m_cnt(cnt), m_tl(tl), m_br(br), m_pen(pen) {
	// introduce particles into region
	m_particles = new Particle * [cnt];
	for (size_t i = 0; i < m_cnt; ++i) {
		// TODO: maxwell-boltzmann distribution of energies translated into velocity with KE formula
		// TODO: radomize positions (below is a temporary implementation for testing)
		m_particles[i] = new Particle(Vector2D(200.0f + (50.0f * i), 200.0f + (2.0f * i)), Vector2D(200.0f * (i + 1), 200.0f * (i + 1)), 10.0f, 10.0f, 0xa4cbdb);
	}
}

Vector2D System::gettl() const { return m_tl; }
Vector2D System::getbr() const { return m_br; }

void System::settl(const Vector2D tl) { m_tl = tl; }
void System::setbr(const Vector2D br) { m_br = br; }

float System::lerpt(const float a, const float b, const float t) { return a + t * (b - a); }
float System::lerpp(const float a, const float b, const float p) {
	return (p - a) / (b - a);
}

void System::borderCheck(const float dt) {
	for (size_t i = 0; i < m_cnt; ++i) {
		Particle* p = m_particles[i];

		// as it is lateron checked, this condition may seem redundant, but it prevents a
		// great deal of memory consumption and unnecessary computation
		if (p->pos().x() - p->rad() < m_tl.x() || p->pos().x() + p->rad() > m_br.x() ||
			p->pos().y() - p->rad() < m_tl.y() || p->pos().y() + p->rad() > m_br.y()) {
			// pf is current pos, pi is pos before latest transformation
			Vector2D pi = p->pos() - p->vel() * dt;
			float t = 1.f;
			p->vel() *= -1;

			// horizontal collision
			if (p->pos().x() - p->rad() < m_tl.x()) {
				t = lerpp(pi.x(), p->pos().x(), m_tl.x() + p->rad());
			}
			else if (p->pos().x() + p->rad() > m_br.x()) {
				t = lerpp(pi.x(), p->pos().x(), m_br.x() - p->rad());
			}
			else {
				p->vel().setx(p->vel().x() * -1);
			}

			// update horizontal position past point of collision
			p->pos().setx(lerpt(pi.x(), p->pos().x(), t));
			p->pos().setx(p->pos().x() + (p->vel().x() * lerpt(dt, 0, t)));
			t = 1.f;

			// vertical collision
			if (p->pos().y() - p->rad() < m_tl.y()) {
				t = lerpp(pi.y(), p->pos().y(), m_tl.y() + p->rad());
			}
			else if (p->pos().y() + p->rad() > m_br.y()) {
				t = lerpp(pi.y(), p->pos().y(), m_br.y() - p->rad());
			}
			else {
				p->vel().sety(p->vel().y() * -1);
			}

			// update vertical position past the point of collision
			p->pos().sety(lerpt(pi.y(), p->pos().y(), t));
			p->pos().sety(p->pos().y() + (p->vel().y() * lerpt(dt, 0, t)));
		}
	}
}

float System::resolveOverlap(Particle* p1, Particle* p2, const float dt) {
	// determine initial and final positions of the two particles
	Vector2D pi1 = p1->pos() - p1->vel() * dt;
	Vector2D pi2 = p2->pos() - p2->vel() * dt;
	Vector2D* pf1 = &(p1->pos());
	Vector2D* pf2 = &(p2->pos());
	
	// set up variables for quadratic formula
	float a, b, c, d, f, g, h, r, t1, t2, t;
	r = p1->rad() + p2->rad();
	a = pi2.x() - pi1.x();
	b = pf2->x() - pf1->x() + pi1.x() - pi2.x();
	c = pi2.y() - pi1.y();
	d = pf2->y() - pf1->y() + pi1.y() - pi2.y();

	// use the quadratic formula to determine the value of t in lerpt
	// 0 = fx^2 + gx + h
	f = (b * b) + (d * d);
	g = (2 * a * b) + (2 * c * d);
	h = (a * a) + (c * c) - (r * r);

	t1 = (-g + sqrt(g * g - 4 * f * h)) / (2 * f);
	t2 = (-g - sqrt(g * g - 4 * f * h)) / (2 * f);
	(1 > t1 && t1 > 0) ? t = t1 : t = t2;	// by definition t must be between 0 and 1

	p1->pos().setx(lerpt(pi1.x(), pf1->x(), t));
	p1->pos().sety(lerpt(pi1.y(), pf1->y(), t));
	p2->pos().setx(lerpt(pi2.x(), pf2->x(), t));
	p2->pos().sety(lerpt(pi2.y(), pf2->y(), t));

	return t;
}

void System::particleCollide(Particle* p1, Particle* p2, const float dt, const float t) {
	// find unit normal and tangent vectors
	Vector2D un(p1->pos().x() - p2->pos().x(), p1->pos().y() - p2->pos().y());
	un = un.unitVector();
	Vector2D ut(-un.y(), un.x());

	// project velocities to determine normal and tangential components
	float vni1 = un * p1->vel();
	float vt1 = ut * p1->vel();
	float vni2 = un * p2->vel();
	float vt2 = ut * p2->vel();

	// determine the final normal components (tangential components are unaffected by collision)
	float vnf1 = (vni1 * (p1->mass() - p2->mass()) + 2 * p2->mass() * vni2) / (p1->mass() + p2->mass());
	float vnf2 = (vni2 * (p2->mass() - p1->mass()) + 2 * p1->mass() * vni1) / (p1->mass() + p2->mass());

	// convert final components to updated velocity vectors
	p1->setVel(vnf1 * un + vt1 * ut);
	p2->setVel(vnf2 * un + vt2 * ut);

	// complete the motion of the particles with updated velocities
	p1->setPos(Vector2D(p1->pos().x() + (p1->vel().x() * lerpt(dt, 0, t)), 
		p1->pos().y() + (p1->vel().y() * lerpt(dt, 0, t))));
	p2->setPos(Vector2D(p2->pos().x() + (p2->vel().x() * lerpt(dt, 0, t)), 
		p2->pos().y() + (p2->vel().y() * lerpt(dt, 0, t))));
}

void System::update(const float dt) {
	// TODO: temporarily disabled to allow for collision check testing
	for (size_t i = 0; i < m_cnt; ++i)
		m_particles[i]->update(dt);

	// resolve border collisions
	borderCheck(dt);

	// check for collisions

	// resolve existing collisions
	// TODO: temporary fix for testing:
	if (m_particles[0]->rad() + m_particles[1]->rad() > sqrt((m_particles[0]->pos().x() - m_particles[1]->pos().x()) * (m_particles[0]->pos().x() - m_particles[1]->pos().x()) + (m_particles[0]->pos().y() - m_particles[1]->pos().y()) * (m_particles[0]->pos().y() - m_particles[1]->pos().y())))
		particleCollide(m_particles[0], m_particles[1], dt,
			resolveOverlap(m_particles[0], m_particles[1], dt));
}

void System::display(ID2D1HwndRenderTarget* target) {
	// paint particle region
	HRESULT hr;
	ID2D1SolidColorBrush* brush;
	hr = target->CreateSolidColorBrush(D2D1::ColorF(m_pen, 1.0f), &brush);

	if (SUCCEEDED(hr)) {
		// paint borders
		D2D1_RECT_F box = D2D1::RectF(m_tl.x(), m_tl.y(), m_br.x(), m_br.y());
		target->DrawRectangle(&box, brush);

		// paint particles
		for (size_t i = 0; i < m_cnt; ++i)
			m_particles[i]->display(target);

		// cleanup
		brush->Release();
	}
}