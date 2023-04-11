#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2D {
    // dimensional values
    float m_x, m_y;

public:
    // class constructors
    Vector2D() : m_x(0.f), m_y(0.f) {};
    Vector2D(float x, float y) : m_x(x), m_y(y) {};

    // queries
    float x() const;
    float y() const;

    // mutators
    void setx(const float k);
    void sety(const float k);

    // returns the directionless magnitude of the vector
    float magnitude() const;

    // produces a unit vector in the direction of the target
    Vector2D unitVector() const;

    // equation/inequality overload
    bool operator==(const Vector2D& vec) const;
    bool operator!=(const Vector2D& vec) const;

    // vector addition overload
    Vector2D operator+(const Vector2D& vec) const;
    Vector2D& operator+=(const Vector2D& vec);

    // vector subtraction overload
    Vector2D operator-(const Vector2D& vec) const;
    Vector2D& operator-=(const Vector2D& vec);

    // dot product overload
    float operator*(const Vector2D& vec) const;

    // scalar/vector multiplication
    Vector2D& operator*=(const float k);
    friend Vector2D operator*(const Vector2D& vec, const float k);
    friend Vector2D operator*(const float k, const Vector2D& vec);
};

#endif