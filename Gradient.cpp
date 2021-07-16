#include "Gradient.h"

void setGradientColor(const vec3 &top, const vec3 &bot) {
	
	static Shader bkg;
	static unsigned int bkgvao = 0;
	if (!bkg.ID) {
		bkg.compile("shaders/vs_background_dg.vs", "shaders/fs_background_dg.fs");
		glGenVertexArrays(1, &bkgvao);
	}

	bkg.use();
	bkg.setVec3f("topColor", top);
	bkg.setVec3f("botColor", bot);
	glBindVertexArray(bkgvao);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glEnable(GL_DEPTH_TEST);

}
