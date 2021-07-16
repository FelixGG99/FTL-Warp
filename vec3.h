#ifndef VEC3_H
#define VEC3_H
#include<math.h>

class vec3 {
public:
	vec3() {}
	vec3(float x, float y, float z) { e[0] = x;	 e[1] = y; e[2] = z; }

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	//inline const vec3& operator +() const { std::cout << "First overload\n"; return *this; }
	inline vec3 operator -() const { return vec3(-e[0], -e[1], -e[2]); }
	inline float operator[] (int i) const { return e[i]; }
	inline float& operator[] (int i) { return e[i]; }

	inline vec3& operator+=(const vec3 &v2);
	inline vec3& operator-=(const vec3 &v2);
	inline vec3& operator*=(const vec3 &v2);
	inline vec3& operator/=(const vec3 &v2);
	inline vec3& operator*=(const float &t);
	inline vec3& operator/=(const float &t);

	inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }; //length of vector
	inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }; //squared length of vector
	inline void make_unit_vector(); //normalize vector

	float e[3]; //x,y,z - r,g,b
};

inline vec3& vec3::operator+=(const vec3 &v2) {
	e[0] += v2.e[0];
	e[1] += v2.e[1];
	e[2] += v2.e[2];
	return *this;
}

inline vec3& vec3::operator-= (const vec3 &v2) {
	e[0] -= v2.e[0];
	e[1] -= v2.e[1];
	e[2] -= v2.e[2];
	return *this;
}

inline vec3& vec3::operator*= (const vec3 &v2) {
	e[0] *= v2.e[0];
	e[1] *= v2.e[1];
	e[2] *= v2.e[2];
	return *this;
}

inline vec3& vec3::operator/= (const vec3 &v2) {
	e[0] += v2.e[0];
	e[1] += v2.e[1];
	e[2] += v2.e[2];
	return *this;
}

inline vec3& vec3::operator*= (const float &t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec3& vec3::operator/= (const float &t) {
	e[0] /= t;
	e[1] /= t;
	e[2] /= t;
	return *this;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const float &t) {
	return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline vec3 operator/(const vec3 &v1, const float &t) {
	return vec3(v1.e[0] / t, v1.e[1] / t, v1.e[2] / t);
}

inline void vec3::make_unit_vector() {
	float k = 1.0f / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k; e[1] *= k; e[2] *= k;
}

inline vec3 unit_vector(vec3 &v) {
	return v / v.length();
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
	return vec3((v1.y()*v2.z() - v1.z()*v2.y()), -(v1.x()*v2.z() - v1.z()*v2.x()), (v1.x()*v2.y() - v1.y()*v2.x()));
}

inline float dot(const vec3 &v1, const vec3 &v2) {
	return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}
#endif