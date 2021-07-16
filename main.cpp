#include<Shader.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#include "Star.h"
#include "Gradient.h"
#include "Projection.h"
#include <algorithm>
int w = 1920;
int h = 1080;
const float nearPlane = 1.0f;
const float farPlane = 10.0f;
float dt, currentTime = 0.0f, lastTime = 0.0f;
vec3 camPos(0.0f, 0.0f, 1.000001f), camFront;
unsigned char isShipVisible = 0, autoPilot = 1, fullscreen = 0;
unsigned char keys[30];

const vec3 colors[] = { {0.0784,0.3176,0.8784}, {0.6,0.6,1.0}, {0.2431,0.3705,0.9803} };
float cameraSpeed = 0.0f;
const float radiusEffect = 0.3f;
const int nStars = 500;
Star *stars[nStars];
std::default_random_engine r;
GLFWwindow* mainWindow;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){ //update framebuffer when changed
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) keys['x' - 'a'] = 1;
	else if (keys['x' - 'a'] == 1 && glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE) { isShipVisible ^= 1; keys['x' - 'a'] = 0; }

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) keys['q' - 'a'] = 1;
	else if (keys['q' - 'a'] == 1 && glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) { if (autoPilot) autoPilot = 0; else autoPilot=1; keys['q' - 'a'] = 0; }

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) keys['w' - 'a'] = 1;
	else if (keys['w' - 'a'] == 1 && glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) { cameraSpeed=std::min(25.0f,cameraSpeed+3.0f); keys['w' - 'a'] = 0; }

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) keys['s' - 'a'] = 1;
	else if (keys['s' - 'a'] == 1 && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) { cameraSpeed = std::max(5.0f, cameraSpeed - 3.0f); keys['s' - 'a'] = 0; }

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) keys['b' - 'a'] = 1;
	else if (keys['b' - 'a'] == 1 && glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) { if (!isShipVisible) isShipVisible = 1; else isShipVisible = 0; keys['b' - 'a'] = 0; }

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) keys['p' - 'a'] = 1;
	else if (keys['p' - 'a'] == 1 && glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) { if (!autoPilot) autoPilot = 1; else autoPilot = 0; keys['p' - 'a'] = 0; }

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) keys['f' - 'a'] = 1;
	else if (keys['f' - 'a'] == 1 && glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
		if (!fullscreen) { fullscreen = 1; mainWindow = glfwCreateWindow(w, h, "FTL Travel", glfwGetPrimaryMonitor(), NULL); }
		else { fullscreen = 0; mainWindow = glfwCreateWindow(w, h, "FTL Travel", NULL, NULL); }
		keys['f' - 'a'] = 0;
	}
																																																			
}

//const vec3 star[] = {	{0.0f,0.0f,0.0f},	{0.0f,-0.01f,0.0f},	{0.0f,0.0f,-0.25f},	{0.0f,-0.01f,-0.25f} };
const float star[] = {	0.0f,0.0f,0.0f,0.0f,1.0f,	0.0f,-0.03f,0.0f,0.0f,0.0f,	0.0f,0.0f,-0.75f,1.0f,1.0f,	0.0f,-0.03f,-0.75f,1.0f,0.0f };
//const vec3 star[] = { {0.0f,0.0f,0.0f},	{0.0f,0.0f,-0.5f}};
const float ship[] = {	
	-0.96f, 0.54f, 0.0f, 0.0f, 1.0f,
	-0.96f,-0.54f, 0.0f, 0.0f, 0.0f,
	 0.96f, 0.54f, 0.0f, 1.0f, 1.0f,	
	 0.96f,-0.54f, 0.0f, 1.0f, 0.0f
};


int getRandom(int mini, int maxi) {

	int fix = 0;
	if (maxi < 0) {
		fix += maxi;
		maxi = 0;
	}
	if (mini < 0) {
		maxi -= mini;
		fix += mini;
		mini = 0;
	}
	int res = r() % (maxi + 1) + mini;
	if (fix) res += fix;
	return res;
}

int main() {
	FreeConsole();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(w, h, "FTL Travel", NULL, nullptr);
	if (mainWindow == NULL) { //check whether the main window was created or not
		std::cout << "Window creation failed.\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mainWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD load failed.\n";
		glfwTerminate();
		return -2;
	}

	glViewport(0, 0, w, h);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback); //set framebuffer size callbacl

	camFront = vec3(camPos.x(), camPos.y(), camPos.z() - 1.0f);

	//generate stars
	r.seed(time(NULL));
	for (int i = 0; i < nStars; i++) {
		stars[i] = new Star((float)getRandom(-1300, 1300) / 1000.0f, (float)getRandom(-700, 700) / 1000.0f, (float)getRandom(-10000, -1000) / 500.0f);
		if (stars[i]->x() >= -radiusEffect && stars[i]->x() <= radiusEffect && stars[i]->y() >= -radiusEffect && stars[i]->y() <= radiusEffect) {
			if (stars[i]->x() < 0.0f) stars[i]->e[0] -= 0.3f; else stars[i]->e[0] += 0.3f;
			if (stars[i]->y() < 0.0f) stars[i]->e[1] -= 0.3f; else stars[i]->e[1] += 0.3f;
		}
	}

	// star's vao
	GLuint vaoStar, vboStar;
	glGenVertexArrays(1, &vaoStar);
	glGenBuffers(1, &vboStar);
	glBindVertexArray(vaoStar);
	glBindBuffer(GL_ARRAY_BUFFER,vboStar);
	glBufferData(GL_ARRAY_BUFFER, sizeof(star), star, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// shaders creation
	Shader projTex("shaders/vs_projTex.vs", "shaders/fs_plaintex.fs");
	Shader proj("shaders/vs_unmod.vs", "shaders/fs_unmod.fs");

	// stbi and texture settings and variables
	int imgWidth, imgHeight, imgCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ship texture
	unsigned int texture = 0;
	data = stbi_load("bridge.png", &imgWidth, &imgHeight, &imgCh, 0);
	if (data) {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "Failed to load texture.\n";
	stbi_image_free(data);

	// star texture
	unsigned int starTex;
	data = stbi_load("laser.png", &imgWidth, &imgHeight, &imgCh, 0);
	if (data) {
		glGenTextures(1, &starTex);
		glBindTexture(GL_TEXTURE_2D, starTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "Failed to load texture.\n";
	stbi_image_free(data);

	// ship vao
	GLuint vaoShip, vboShip;
	glGenVertexArrays(1, &vaoShip);
	glGenBuffers(1, &vboShip);
	glBindVertexArray(vaoShip);
	glBindBuffer(GL_ARRAY_BUFFER, vboShip);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ship), ship, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	mat4x4 model;
	mat4x4 projection = setProjMatrix((float)w, (float)h, 1.0f, 10.0f, 50.0f);
	mat4x4 view = lookAt(camPos, camFront);
	const float s = getScaleFromFOV(60.0f, 1.0f);
	proj.use();
	proj.setMat4f("projection", projection);
	proj.setFloat("s", s);
	proj.setMat4f("view", view);

	projTex.use();
	projTex.setMat4f("projection", projection);
	projTex.setMat4f("model", model);
	projTex.setFloat("s", s);
	projTex.setMat4f("view", view);

	while (!glfwWindowShouldClose(mainWindow)) {
		processInput(mainWindow);
		if(autoPilot) cameraSpeed = std::min(25.0f, cameraSpeed + 0.02f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentTime = glfwGetTime();
		dt = std::min(0.01666f,currentTime - lastTime);
		lastTime = currentTime;
		setGradientColor(vec3(0.1f, 0.0f, 0.2f), vec3(0.0f, 0.0f, 0.0f));
		

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		proj.use();
		glBindVertexArray(vaoStar);
		for (int i = 0; i < nStars; i++) {
			model(3, 0) = stars[i]->x();
			model(3, 1) = stars[i]->y();
			model(3, 2) = stars[i]->z();
			proj.setMat4f("model", model);
			proj.setVec3f("col", colors[i % 3]);
			glBindVertexArray(vaoStar);
			glBindTexture(GL_TEXTURE_2D, starTex);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			stars[i]->update();
		}
		if (isShipVisible) {
			projTex.use();
			glBindVertexArray(vaoShip);
			glBindTexture(GL_TEXTURE_2D, texture);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glDisable(GL_BLEND);
			glDepthMask(GL_TRUE);	
		}
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}
	return 0;
}