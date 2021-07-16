
#ifndef MAT44_H
#define MAT44_H

class mat4x4 {
public:
	float e[16];

	mat4x4() : e{ 1,0,0,0,	0,1,0,0,	0,0,1,0,	0,0,0,1 } {}
	mat4x4(const float(&m)[4][4]) : e{ m[0][0],m[0][1],m[0][2],m[0][3],	m[1][0],m[1][1],m[1][2],m[1][3],	m[2][0],m[2][1],m[2][2],m[2][3],	m[3][0],m[3][1],m[3][2],m[3][3] } {}

	inline void valueFromMatrix(const float(&m)[4][4]);
	inline void makeIdentity();
	inline float operator()(const int &i, const int &j) const { /*if (i >= 4 || i < 0 || j >= 4 || j < 0) throw BadIndex("Index out of bounds");*/ return e[i * 4 + j]; }
	inline float& operator()(const int &i, const int &j) { /*if (i >= 4 || i < 0 || j >= 4 || j < 0) throw BadIndex("Index out of bounds");*/ return e[i * 4 + j]; }

};

inline void mat4x4::valueFromMatrix(const float(&m)[4][4]) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			e[i * 4 + j] = m[i][j];
}

inline void mat4x4::makeIdentity() {
	e[0] = 1; e[1] = 0; e[2] = 0; e[3] = 0; e[4] = 0; e[5] = 1; e[6] = 0; e[7] = 0; e[8] = 0;
	e[9] = 0; e[10] = 1; e[11] = 0; e[12] = 0; e[13] = 0; e[14] = 0; e[15] = 1;
}


#endif