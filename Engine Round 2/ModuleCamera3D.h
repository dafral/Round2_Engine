#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "TextureMSAA.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	TextureMSAA* GetSceneTexture();

	//Camera controls
	void MoveCamera();
	void RotateCamera();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	bool orb_x_inverted, orb_y_inverted, wheel_inverted;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
	float x, sensitivity;
	bool positive;
	TextureMSAA* scene_texture = nullptr;
};