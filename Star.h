#pragma once
#include<random>
#include<time.h>

extern std::default_random_engine r;
extern float cameraSpeed;
extern float dt;
extern int getRandom(int mini, int maxi);
extern const float radiusEffect;
class Star {
public:
	Star() {}
	Star(float x, float y, float z) { e[0] = x; e[1] = y; e[2] = z; }
	inline float x() { return e[0]; }
	inline float y() { return e[1]; }
	inline float z() { return e[2]; }


	void update() {
		e[2] += dt * cameraSpeed;

		if (e[2] > 0.0f) {
			e[0] = (float)getRandom(-1600, 1600) / 1000.0f;
			e[1] = (float)getRandom(-900, 900) / 1000.0f;
			e[2] = -10.0f;
			if (e[0] >= -radiusEffect && e[0] <= radiusEffect && e[1] >= -radiusEffect && e[1] <= radiusEffect) {
				if (e[0] < 0.0f) e[0] -= radiusEffect; else e[0] += radiusEffect;
				if (e[1] < 0.0f) e[1] -= radiusEffect; else e[1] += radiusEffect;
			}
		}
	}
	float e[3];
};

/*#pragma once
#include<random>
#include<time.h>

extern std::default_random_engine r;
extern const float cameraSpeed;
extern float dt;
extern int getRandom(int mini, int maxi);

class Star {
public:
	Star() {}
	Star(float x, float y, float z) { e[0] = e[3] = x; e[1] = e[4] = y; e[2] = z; e[5] = z - 0.25f; }
	inline float x() { return e[0]; }
	inline float y() { return e[1]; }
	inline float z() { return e[2]; }

	void draw() {
		glBindVertexArray(vao);
		glDrawArrays(GL_LINES, 0, 6);
	}

	void update() {
		e[5] = e[2];
		e[2] += dt * cameraSpeed;

		if (e[2] > 0.0f) {
			e[0] = (float)getRandom(-1000, 1000) / 1000.0f;
			e[1] = (float)getRandom(-1000, 1000) / 1000.0f;
			e[2] = (float)getRandom(-10000, -1000) / 1000.0f;
			e[3] = e[0];
			e[4] = e[1];
			e[5] = e[2] - 0.25f;
		}
	}
	float e[6];
	unsigned int vao, vbo, ebo;
};
*/
