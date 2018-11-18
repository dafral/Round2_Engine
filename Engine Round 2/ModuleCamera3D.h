#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"

#include <vector>
#include <string>

class Component_Transform;
class Component_Camera;
class TextureMSAA;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	Component_Camera* CreateComponentCamera(std::string name);

	void Move(Component_Transform* trans, float dt);
	void Rotate(Component_Transform* trans, float dt);
	void Zoom(int mouse_z, float dt);
	void DrawAllFrustums();

	TextureMSAA* GetSceneTexture() { return scene_texture; };
	TextureMSAA* GetGameTexture() { return game_texture; };

	float GetSensitivity() { return sensitivity; };
	bool IsXInverted() { return orb_x_inverted; };
	bool IsYInverted() { return orb_y_inverted; };
	bool IsWheelInverted() { return wheel_inverted; };
	Component_Camera* GetSceneCamera() { return scene_camera; };
	Component_Camera* GetGameCamera() { return game_camera; };

	void SetSensitivity(float new_sen) { sensitivity = new_sen; };
	void SetXInverted(bool new_inverted) { orb_x_inverted = new_inverted; };
	void SetYInverted(bool new_inverted) { orb_y_inverted = new_inverted; };
	void SetWheelInverted(bool new_inverted) { wheel_inverted = new_inverted; };
	void SetSceneCamera(Component_Camera* sc) { scene_camera = sc; };
	void SetGameCamera(Component_Camera* sc) { game_camera = sc; };

private:
	float sensitivity = 0.25f;
	bool orb_x_inverted, orb_y_inverted, wheel_inverted = false;

	TextureMSAA* scene_texture = nullptr;
	TextureMSAA* game_texture = nullptr;

	Component_Camera* scene_camera = nullptr;
	Component_Camera* game_camera = nullptr;
	std::vector<Component_Camera*> cameras;
};

#endif // !__MODULECAMERA3D_H__