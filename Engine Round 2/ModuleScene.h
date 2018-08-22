#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include <vector>

#include "glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glew/lib/glew32.lib")


#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	
	void Draw();

};

class glCube
{
public:
	void DrawCubeArrays();
	void DrawCubeElements();
};

class SolidSphere
{
public:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texcoords;
	std::vector<GLushort> indices;

public:
	SolidSphere(float radius, unsigned int rings, unsigned int sectors);
	void draw(GLfloat x, GLfloat y, GLfloat z);
};
