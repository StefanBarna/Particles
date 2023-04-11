#include "System.h"

System::System(Vector2D tl, Vector2D br, size_t cnt, UINT pen) :
	m_cnt(cnt), m_tl(tl), m_br(br), m_pen(pen) {
	// introduce particles into region
	m_particles = new Particle * [cnt];
	for (size_t i = 0; i < m_cnt; ++i) {
		// TODO: maxwell-boltzmann distribution of energies translated into velocity with KE formula
		// TODO: radomize positions (below is a temporary implementation for testing)
		m_particles[i] = new Particle(Vector2D(300.0f, 300.0f), Vector2D(200.0f, 200.0f), 10.0f, 10.0f, 0xa4cbdb);
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
			// posf is current pos, posi is pos before latest transformation
			Vector2D posi = p->pos() - p->vel() * dt;
			float t = 1.f;
			p->vel() *= -1;

			// horizontal collision
			if (p->pos().x() - p->rad() < m_tl.x()) {
				t = lerpp(posi.x(), p->pos().x(), m_tl.x() + p->rad());
			}
			else if (p->pos().x() + p->rad() > m_br.x()) {
				t = lerpp(posi.x(), p->pos().x(), m_br.x() - p->rad());
			}
			else {
				p->vel().setx(p->vel().x() * -1);
			}

			// update horizontal position past point of collision
			p->pos().setx(lerpt(posi.x(), p->pos().x(), t));
			p->pos().setx(p->pos().x() + (p->vel().x() * lerpt(dt, 0, t)));
			t = 1.f;

			// vertical collision
			if (p->pos().y() - p->rad() < m_tl.y()) {
				t = lerpp(posi.y(), p->pos().y(), m_tl.y() + p->rad());
			}
			else if (p->pos().y() + p->rad() > m_br.y()) {
				t = lerpp(posi.y(), p->pos().y(), m_br.y() - p->rad());
			}
			else {
				p->vel().sety(p->vel().y() * -1);
			}

			// update vertical position past the point of collision
			p->pos().sety(lerpt(posi.y(), p->pos().y(), t));
			p->pos().sety(p->pos().y() + (p->vel().y() * lerpt(dt, 0, t)));
		}
	}
}

void System::particleCollide(Particle* p1, Particle* p2) {

}

void System::update(const float dt) {
	for (size_t i = 0; i < m_cnt; ++i)
		m_particles[i]->update(dt);

	// resolve border collisions
	borderCheck(dt);

	// check for collisions

	// resolve existing collisions
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