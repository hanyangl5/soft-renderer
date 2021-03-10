#pragma once
#include <cmath>
#include <iostream>
namespace srmath {

#define _PI 3.1415926

// interpolation x,y
inline float interp(float x1, float x2, float t) { return x1 + (x2 - x1) * t; }



class Indices {
public:
	Indices() : x(0), y(0), z(0) {}
	Indices(unsigned int _x, unsigned int _y, unsigned int _z)
		: x(_x), y(_y), z(_z) {}
	unsigned int x, y, z;
};

typedef Indices Color;

//  operator* (scale * Color)
Color operator*(float s, const Color& color) {
	return Color{ static_cast<unsigned int>(s * color.x), static_cast<unsigned int>(s * color.y),
				 static_cast<unsigned int>(s * color.z) };
}

// operator* (Color * Color)
Color operator*(const Color& color1, const Color& color2) {
	return Color{ (color1.x * color2.x) / (255), (color1.y * color2.y) / 255,
				 (color1.z * color2.z) / 255 };
}

// operator+
Color operator+(const Color& color1, const Color& color2) {
	return Color{ color1.x + color2.x, color1.y + color2.y, color1.z + color2.z };
}


class Vector2f {
public:
	float u, v;
	Vector2f() : u(0.0f), v(0.0f) {};
	Vector2f(float _u, float _v) : u(_u), v(_v) {};
};

// operator*
Vector2f operator*(float s, const Vector2f& vec) {
	return Vector2f{ s * vec.u, s * vec.v };
}

// operator/
Vector2f operator/(const Vector2f& vec, float s) {
	return Vector2f{ vec.u / s, vec.v / s };
}

// operator+
Vector2f operator+(const Vector2f& Vector2f1, const Vector2f& Vector2f2) {
	return Vector2f{ Vector2f1.u + Vector2f2.u, Vector2f1.v + Vector2f2.v };
}


class Vector4f {
public:
	Vector4f() : x(0), y(0), z(0), w(1) {}
	Vector4f(float _x, float _y, float _z, float _w = 1)
		: x(_x), y(_y), z(_z), w(_w) {}

	Vector4f operator-() const; // neg

	Vector4f& operator+=(const Vector4f& v);
	Vector4f& operator-=(const Vector4f& v);
	Vector4f& operator*=(const Vector4f& v);
	Vector4f& operator/=(const Vector4f& v);
	Vector4f& operator*=(const float t);
	Vector4f& operator/=(const float t);

	float Length() const;
	float SquaredLength() const;
	void  Normalized();

	float x, y, z, w;
};

// operator*
Vector4f Vector4f::operator-() const {
	return Vector4f(-this->x, -this->y, -this->z);
}

// operator+=
Vector4f& Vector4f::operator+=(const Vector4f& v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

// operator-=
Vector4f& Vector4f::operator-=(const Vector4f& v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

// operator*=
Vector4f& Vector4f::operator*=(const Vector4f& v) {
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	return *this;
}

// operator/=
Vector4f& Vector4f::operator/=(const Vector4f& v) {
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	return *this;
}

// operator*=
Vector4f& Vector4f::operator*=(const float t) {
	this->x *= t;
	this->y *= t;
	this->z *= t;
	return *this;
}

// operator/=
Vector4f& Vector4f::operator/=(const float t) {
	this->x /= t;
	this->y /= t;
	this->z /= t;
	return *this;
}

// length
float Vector4f::Length() const {
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

// squared lenth
float Vector4f::SquaredLength() const {
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

// normalize
void Vector4f::Normalized() {
	float k = 1.0 / this->Length();
	this->x *= k;
	this->y *= k;
	this->z *= k;
}

// non-member functions

std::ostream& operator<<(std::ostream& os, const Vector4f& t) {
	os << t.x << " " << t.y << " " << t.z << " " << t.w;
	return os;
}

std::istream& operator>>(std::istream& is, Vector4f& t) {
	is >> t.x >> t.y >> t.z >> t.w;
	return is;
}

// operator+
Vector4f operator+(const Vector4f& v1, const Vector4f& v2) {
	return Vector4f(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

// operator-
Vector4f operator-(const Vector4f& v1, const Vector4f& v2) {
	return Vector4f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

// operator* (Vector4f * Vector4f)
Vector4f operator*(const Vector4f& v1, const Vector4f& v2) {
	return Vector4f(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

// operator* (float * Vector4f)
Vector4f operator*(float t, const Vector4f& v) {
	return Vector4f(t * v.x, t * v.y, t * v.z);
}

// operator* (Vector4f * float)
Vector4f operator*(const Vector4f& v, float t) {
	return Vector4f(t * v.x, t * v.y, t * v.z);
}

// operator/ (Vector4f / Vcctor4f)
Vector4f operator/(const Vector4f& v1, const Vector4f& v2) {
	return Vector4f(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

// operator/ (Vector4f / float)
Vector4f operator/(Vector4f v, float t) {
	return Vector4f(v.x / t, v.y / t, v.z / t, v.w);
}

// normalize
Vector4f normalize(const Vector4f& v) {
	Vector4f o = v / v.Length();
	return o;
}

// dot
float dot(const Vector4f& v1, const Vector4f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// cross
Vector4f cross(const Vector4f& v1, const Vector4f& v2) {
	float m1, m2, m3;
	m1 = v1.y * v2.z - v1.z * v2.y;
	m2 = v1.z * v2.x - v1.x * v2.z;
	m3 = v1.x * v2.y - v1.y * v2.x;

	return Vector4f(m1, m2, m3);
}

// vector4f interp
Vector4f Vector4f_interp(const Vector4f& x1, const Vector4f& x2, float t) {
	return Vector4f(interp(x1.x, x2.x, t), interp(x1.y, x2.y, t),
		interp(x1.z, x2.z, t));
}

// convert point to vector
Vector4f Point2Vector(Vector4f vec) { return Vector4f(vec.x, vec.y, vec.z, 0); }

class Matrix4f {
public:
	Matrix4f() { this->SetIdentity(); }
	void SetIdentity();
	void SetZero();

	Matrix4f Scale(const Vector4f s) const;
	Matrix4f Scale(const float s) const;
	Matrix4f Translate(const Vector4f t) const;
	Matrix4f Rotate(const Vector4f axis, const float angle) const;

	Matrix4f& operator=(const Matrix4f& m);
	Matrix4f& operator+=(const Matrix4f& m);
	Matrix4f& operator-=(const Matrix4f& m);
	Matrix4f& operator*=(const float t);
	Matrix4f& operator/=(const float t);
	Matrix4f  operator*(const Matrix4f& m) const;
	Vector4f  operator*(const Vector4f& v) const;

	// comma initializer
	// https://stackoverflow.com/questions/29523119/how-could-comma-separated-initialization-such-as-in-eigen-be-possibly-implemente
	struct CommaInitializer {

		Matrix4f& m;
		int       i;
		CommaInitializer(Matrix4f& m, int i) : m(m), i(i) {}
		CommaInitializer operator,(double x) {
			m.e[i / 4][i % 4] = x;
			return CommaInitializer(m, i + 1);
		}
	};
	CommaInitializer operator<<(double x) {
		e[0][0] = x;
		return CommaInitializer(*this, 1);
	}

	float e[4][4] = { 0.0f };
};

std::ostream& operator<<(std::ostream& os, const Matrix4f& m) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			os << m.e[i][j] << " ";
		}
		os << "\n";
	}
	return os;
}
std::istream& operator>>(std::istream& is, Matrix4f& m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			is >> m.e[i][j];
		}
	}

	return is;
}

// operator=
Matrix4f& Matrix4f::operator=(const Matrix4f& m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->e[i][j] = m.e[i][j];
		}
	}
	return *this;
}

// opreator+=
Matrix4f& Matrix4f::operator+=(const Matrix4f& m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->e[i][j] += m.e[i][j];
		}
	}
	return *this;
}

// operator-=
Matrix4f& Matrix4f::operator-=(const Matrix4f& m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->e[i][j] -= m.e[i][j];
		}
	}
	return *this;
}

// operator*=
Matrix4f& Matrix4f::operator*=(const float t) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->e[i][j] *= t;
		}
	}
	return *this;
}

// operator/=
Matrix4f& Matrix4f::operator/=(const float t) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->e[i][j] /= t;
		}
	}
	return *this;
}

// operator* (Matrix4f * Matrix4f)
Matrix4f Matrix4f::operator*(const Matrix4f& m) const {
	Matrix4f o;
	for (unsigned int i = 0; i < 4; ++i) {
		for (unsigned int j = 0; j < 4; ++j) {
			o.e[i][j] = this->e[i][0] * m.e[0][j] + this->e[i][1] * m.e[1][j] +
				this->e[i][2] * m.e[2][j] + this->e[i][3] * m.e[3][j];
		}
	}
	return o;
}

// operator* (Matrix4f * Vector4f)
Vector4f Matrix4f::operator*(const Vector4f& v) const {
	Vector4f o;

	o.x = this->e[0][0] * v.x + this->e[0][1] * v.y + this->e[0][2] * v.z +
		this->e[0][3] * v.w;
	o.y = this->e[1][0] * v.x + this->e[1][1] * v.y + this->e[1][2] * v.z +
		this->e[1][3] * v.w;
	o.z = this->e[2][0] * v.x + this->e[2][1] * v.y + this->e[2][2] * v.z +
		this->e[2][3] * v.w;
	o.w = this->e[3][0] * v.x + this->e[3][1] * v.y + this->e[3][2] * v.z +
		this->e[3][3] * v.w;

	return o;
}

// set matrix identity
void Matrix4f::SetIdentity() {
	this->e[0][0] = 1;
	this->e[1][1] = 1;
	this->e[2][2] = 1;
	this->e[3][3] = 1;
}

// set all matrix elements by 0
void Matrix4f::SetZero() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->e[i][j] = 0;
		}
	}
}

// scale by vector
Matrix4f Matrix4f::Scale(const Vector4f s) const {
	Matrix4f o;
	o.e[0][0] *= s.x;
	o.e[1][1] *= s.y;
	o.e[2][2] *= s.z;
	return o;
}

// scale by single parameter
Matrix4f Matrix4f::Scale(const float s) const {
	Matrix4f o;
	o.e[0][0] *= s;
	o.e[1][1] *= s;
	o.e[2][2] *= s;
	return o;
}

// translation / move point
Matrix4f Matrix4f::Translate(const Vector4f t) const {
	Matrix4f o;
	o.e[0][3] = t.x;
	o.e[1][3] = t.y;
	o.e[2][3] = t.z;
	return o;
}

// rotate by arbitrary axis
Matrix4f Matrix4f::Rotate(const Vector4f axis, const float angle) const {
	Matrix4f o;
	o.e[0][0] = cos(angle) + (1 - cos(angle)) * axis.x * axis.x;
	o.e[0][1] = (1 - cos(angle)) * axis.x * axis.y - axis.z * sin(angle);
	o.e[0][2] = (1 - cos(angle)) * axis.x * axis.y + axis.y * sin(angle);
	o.e[1][0] = (1 - cos(angle)) * axis.x * axis.y + axis.z * sin(angle);
	o.e[1][1] = cos(angle) + (1 - cos(angle)) * axis.y * axis.y;
	o.e[1][2] = (1 - cos(angle)) * axis.y * axis.z - axis.x * sin(angle);
	o.e[2][0] = (1 - cos(angle)) * axis.x * axis.z - axis.y * sin(angle);
	o.e[2][1] = (1 - cos(angle)) * axis.y * axis.z + axis.x * sin(angle);
	o.e[2][2] = cos(angle) + (1 - cos(angle)) * axis.z * axis.z;
	return o;
}

// Matrix4f * Matrix4f
Matrix4f mul(const Matrix4f& m1, const Matrix4f& m2) {
	Matrix4f z;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			z.e[j][i] = (m1.e[j][0] * m2.e[0][i]) + (m1.e[j][1] * m2.e[1][i]) +
				(m1.e[j][2] * m2.e[2][i]) + (m1.e[j][3] * m2.e[3][i]);
		}
	}
	return z;
}

// Matrix4f * Vector4f
Vector4f mul(const Matrix4f& m, const Vector4f& v) {
	Vector4f z;

	z.x = (m.e[0][0] * v.x) + (m.e[0][1] * v.y) + (m.e[0][2] * v.z) +
		(m.e[0][3] * v.w);
	z.y = (m.e[1][0] * v.x) + (m.e[1][1] * v.y) + (m.e[1][2] * v.z) +
		(m.e[1][3] * v.w);
	z.z = (m.e[2][0] * v.x) + (m.e[2][1] * v.y) + (m.e[2][2] * v.z) +
		(m.e[2][3] * v.w);
	z.w = (m.e[3][0] * v.x) + (m.e[3][1] * v.y) + (m.e[3][2] * v.z) +
		(m.e[3][3] * v.w);

	return z;
}


} // namespace srmath