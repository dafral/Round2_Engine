#ifndef __MYPRIMITIVES_H__
#define __MYPRIMITIVES_H__

#include "Globals.h"
#include <vector>

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

#pragma comment (lib, "glu32.lib")   
#pragma comment (lib, "opengl32.lib") 
#pragma comment (lib, "glew/lib/glew32.lib")

class myCube
{
public:
	myCube();
	myCube(float size);
	void Draw() const;

public:

	float size;
};

class mySphere
{
public:

	mySphere(float radius, unsigned int rings, unsigned int sectors);
	void Draw() const;

public:

	float radius;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texcoords;
	std::vector<GLushort> indices;
};

class myPlane
{
public:

	myPlane(float size);

	void Draw() const;

public:

	float size;

	std::vector<GLfloat> vertices;
	std::vector<GLushort> indices;
	
};

#endif // !__MYPRIMITIVES_H__