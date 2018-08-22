#ifndef __MYPRIMITIVES_H__
#define __MYPRIMITIVES_H__

#include "Globals.h"
#include <vector>

#include "glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glew/lib/glew32.lib")


class myCube
{
public:

	float size;

	myCube();
	myCube(float size);
	void DrawCubeArrays();
	void DrawCubeElements();
};

class mySphere
{
public:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texcoords;
	std::vector<GLushort> indices;

public:
	mySphere(float radius, unsigned int rings, unsigned int sectors);
	void draw(GLfloat x, GLfloat y, GLfloat z);
};

#endif // !__MYPRIMITIVES_H__