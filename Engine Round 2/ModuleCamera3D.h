#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "TextureMSAA.h"
#include "Component_Camera.h"
#include <vector>

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	Component_Camera* CreateComponentCamera();

	void LookAt(const float3 &Spot);
	void Move(float dt);
	void Rotate(float dt);
	void Zoom(int mouse_z, float dt);

	TextureMSAA* GetSceneTexture();

	float GetSensitivity();
	void SetSensitivity(float new_sen);
	bool orb_x_inverted, orb_y_inverted, wheel_inverted;
	float sensitivity;

	Component_Camera* editor_camera = nullptr;

private:

	TextureMSAA* scene_texture = nullptr;

	std::vector<Component_Camera*> cameras;
};