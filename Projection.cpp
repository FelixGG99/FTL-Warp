#include "Projection.h"

#define pi 3.141592f


mat4x4 lookAt(const vec3 &camPos, const vec3 &target) {

	vec3 camFw = camPos - target; //vector f (forward)
	camFw.make_unit_vector();
	vec3 camRight = cross(vec3(0.0f, 1.0f, 0.0f), camFw); //vector l (left of the target, right of the camera)
	camRight.make_unit_vector();
	vec3 camUp = cross(camFw, camRight); //vector u (up)
	camUp.make_unit_vector();

	//DEFINITION: TRANSFORMATION MATRIX: a matrix such that multiplying î,j^ and k^ (component vectors of x,y and z axis, the basis of R^3) _____describes how each one of these ends up after the transformation_____
	static mat4x4 view;

	/*	ROTATION OF THE CAMERA	ROTATION OF THE SCENE (INVERSE OF ROTATION OF THE CAMERA)	TRANSLATION OF THE SCENE	COMBINATION OF BOTH MATRICES
		| lx ux fx 0 |			| lx ly lz 0 |												| 1 0 0 -cx |				| lx ly lz -lx*cx - ly*cy - lz*cz |
		| ly uy fy 0 |			| ux uy uz 0 |												| 0 1 0 -cy |				| ux uy uz -ux*cx - uy*cy - uz*cz |
		| lz uz fz 0 |			| fx fy fz 0 |												| 0 0 1 -cz |				| fx fy fz -fx*cx - fy*cy - fz*cz |
		| 0  0  0  1 |			| 0  0  0  1 |												| 0 0 0  1  |				| 0  0  0   1					  |
								**inverse is jus the transpose								**where c is camPos
								because f,l and u are orthonormal*/

								//ROTATION: rotate scene in the inverse direction the camera is rotating to
								//if the rotation of the camera (how its three axis are left after the transformation) is described by the matrix A built using camFw, camRight and camUp,
								//then A^-1, the inverse matrix, describes how the scene is rotated

								/*GLSL uses column major order when using arrays as matrices
								---i dimension
								| 0,0  0,1  0,2  0,3 |
								| 1,0  1,1  1,2  1,3 |
								| 2,0  2,1  2,2  2,3 |
								| 3,0  3,1  3,2  3,3 |

								is the same as

								| 0 4 8  12 |
								| 1 5 9  13 |   == {0,1,2,3... 15}, where these values are indices or positions
								| 2 6 10 14 |
								| 3 7 11 15 |*/


	view(0, 0) = camRight.x();
	view(1, 0) = camRight.y();
	view(2, 0) = camRight.z();
	view(0, 1) = camUp.x();
	view(1, 1) = camUp.y();
	view(2, 1) = camUp.z();
	view(0, 2) = camFw.x();
	view(1, 2) = camFw.y();
	view(2, 2) = camFw.z();
	view(3, 3) = 1;
	//translation of the scene
	view(3, 0) = (camPos.x() * camRight.x() + camRight.y() * camPos.y() + camRight.z() * camPos.z())*(-1.0f);
	view(3, 1) = (camPos.x() * camUp.x() + camUp.y() * camPos.y() + camUp.z() * camPos.z())*(-1.0f);
	view(3, 2) = (camPos.x() * camFw.x() + camFw.y() * camPos.y() + camFw.z() * camPos.z())*(-1.0f);
	return view;
}

mat4x4 setProjMatrix(float w, float h, float n, float f, float fov) {

	static mat4x4 mP;

	//Define vertical and horizontal limits of the image plane, given fov and dimensions of viewport.
	//Upper right corner of the image plane is (r,t). Both r and t are positive. 
	//Lower left corner of the image plane is (l,b). Both l and b are positive.
	//Image plane is symmetrical both vertical and horizontally, so l = -r and b = -t.

	//Calculate using a triangle formed by half the angle of view (fov)
	float t = (float)tan((fov / 2.0f) * (pi / 180.0f)) * n;	//upper limit of the image plane
	float b = -t;									//lower limit of the image plane
	float aspectRatio = w / h;						//aspect ratio of the screen

	float r = t * aspectRatio;	//right limit of the image plane
								//Proportion between w and h must be kept while defining coordinates of the image plane
								//aspectRatio = w / h = r / t;	---->	r = t * aspectRatio

	float l = -r;				//left limit of the image plane

	mP(0, 0) = (2.0f * n) / (r - l);
	mP(1, 0) = 0;
	mP(2, 0) = (r + l) / (r - l);;
	mP(3, 0) = 0;

	mP(0, 1) = 0;
	mP(1, 1) = (2.0f * n) / (t - b);
	mP(2, 1) = (t + b) / (t - b);
	mP(3, 1) = 0;

	mP(0, 2) = 0;
	mP(1, 2) = 0;
	mP(2, 2) = -(f + n) / (f - n);
	mP(3, 2) = -2.0f*f*n / (f - n);

	mP(0, 3) = 0;
	mP(1, 3) = 0;
	mP(2, 3) = -1;
	mP(3, 3) = 0;

	return mP;
}

float getScaleFromFOV(float fov, float n) {
	return 1.0f / (n*tan((fov / 2.0f)*(pi / 180.0f)));
}