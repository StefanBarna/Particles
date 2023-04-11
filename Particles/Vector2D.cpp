#include <cmath>
#include "Vector2D.h"

float Vector2D::x() const { return m_x; }
float Vector2D::y() const { return m_y; }

void Vector2D::setx(const float k) { m_x = k; }
void Vector2D::sety(const float k) { m_y = k; }

float Vector2D::magnitude() const {
    return sqrt(x() * x() + y() * y());
}

Vector2D Vector2D::unitVector() const {
    float m = magnitude();
    return m != 0 ? Vector2D(x() / m, y() / m) : Vector2D();
}

bool Vector2D::operator==(const Vector2D& vec) const {
    return (x() == vec.x() && y() == vec.y());
}

bool Vector2D::operator!=(const Vector2D& vec) const {
    return !(*this == vec);
}

Vector2D Vector2D::operator+(const Vector2D& vec) const {
    return Vector2D(x() + vec.x(), y() + vec.y());
}

Vector2D& Vector2D::operator+=(const Vector2D& vec) {
    *this = *this + vec;
    return *this;
}

Vector2D Vector2D::operator-(const Vector2D& vec) const {
    return Vector2D(x() - vec.x(), y() - vec.y());
}

Vector2D& Vector2D::operator-=(const Vector2D& vec) {
    *this = *this - vec;
    return *this;
}

float Vector2D::operator*(const Vector2D& vec) const {
    return x() * vec.x() + y() * vec.y();
}

Vector2D& Vector2D::operator*=(const float k) {
    *this = *this * k;
    return *this;
}

Vector2D operator*(const Vector2D& vec, const float k) {
    return Vector2D(vec.x() * k, vec.y() * k);
};

Vector2D operator*(const float k, const Vector2D& vec) {
    return Vector2D(vec.x() * k, vec.y() * k);
};