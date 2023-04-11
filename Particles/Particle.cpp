#include "Particle.h"

Vector2D& Particle::pos() { return m_pos; }
Vector2D& Particle::vel() { return m_vel; }
float Particle::mass() const { return m_mass; }
float Particle::rad() const { return m_rad; }
UINT32 Particle::col() const { return m_col; }

void Particle::setPos(const Vector2D p) { m_pos = p; }
void Particle::setVel(const Vector2D v) { m_vel = v; }
void Particle::setMass(const float m) { m_mass = m; }
void Particle::setRad(const float r) { m_rad = r; }
void Particle::setCol(const UINT c) { m_col = c; }

void Particle::update(float dt) {
	m_pos += m_vel * dt;
}

void Particle::display(ID2D1HwndRenderTarget* target) {
	// create coloured brush
	HRESULT hr;
	ID2D1SolidColorBrush* brush;
	hr = target->CreateSolidColorBrush(D2D1::ColorF(m_col, 1.0f), &brush);

	if (SUCCEEDED(hr)) {
		// paint particle
		D2D1_ELLIPSE figure = D2D1::Ellipse(D2D1::Point2F(m_pos.x(), m_pos.y()), m_rad, m_rad);
		target->FillEllipse(&figure, brush);

		// cleanup
		brush->Release();
	}
}