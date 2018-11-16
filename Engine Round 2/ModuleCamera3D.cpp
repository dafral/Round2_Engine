#include "Application.h"
#include "ModuleCamera3D.h"
#include "TextureMSAA.h"
#include "Component_Camera.h"
#include "Component_Transform.h"
#include "Color.h"
#include "Panel.h"
#include "PanelScene.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Start()
{
	CONSOLELOG("Setting up the camera");
	bool ret = true;

	scene_texture = new TextureMSAA();
	scene_texture->Create(App->window->screen_surface->w, App->window->screen_surface->h);

	game_texture = new TextureMSAA();
	game_texture->Create(App->window->screen_surface->w, App->window->screen_surface->h);

	return ret;
}

bool ModuleCamera3D::CleanUp()
{
	CONSOLELOG("Cleaning camera");

	return true;
}

update_status ModuleCamera3D::Update(float dt)
{
	// Movement ------------------------------------

	Component_Transform* trans = (Component_Transform*)scene_camera->my_go->FindComponentWithType(TRANSFORM);

	Move(trans, dt);

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		Rotate(trans, dt);

	if (App->debug->IsDebugDrawActive()) DrawAllFrustums();

	// Mouse picking -------------------------------

	/*if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->editor->scene->IsHovered())
	{
		LineSegment mouse_ray;
		float2 mouse_pos_norm = App->editor->scene->GetMousePosNormalized();

		if (mouse_pos_norm.x > -1 && mouse_pos_norm.x < 1)
			if (mouse_pos_norm.y > -1 && mouse_pos_norm.y < 1)
				mouse_ray = scene_camera->GetFrustum().UnProjectLineSegment(mouse_pos_norm.x, mouse_pos_norm.y);

		if (mouse_ray.Length() != 0)
			App->scene->RayCollision(mouse_ray);
	}*/

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Rotate(Component_Transform* trans, float dt)
{
	float dx = RadToDeg(trans->GetRotation().ToEulerXYZ().x);
	float dy = RadToDeg(trans->GetRotation().ToEulerXYZ().y);

	orb_x_inverted ? dy += App->input->GetMouseXMotion() * 10 * dt : dy += -App->input->GetMouseXMotion() * 10 * dt;
	orb_y_inverted ? dx += App->input->GetMouseYMotion() * 10 * dt : dx += -App->input->GetMouseYMotion() * 10 * dt;

	trans->SetRotation(DegToRad({ dx, dy, 0 }));
	scene_camera->SetFrustumRot(trans->GetRotation());
}

void ModuleCamera3D::Move(Component_Transform* trans, float dt)
{
	float speed = 15.f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 2.f;

	float3 X = { 1.0f, 0.0f, 0.0f };
	float3 Y = { 0.0f, 1.0f, 0.0f };
	float3 Z = { 0.0f, 0.0f, 1.0f };

	float3 new_pos = trans->GetPosition();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) new_pos += Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) new_pos -= Z * speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) new_pos += X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) new_pos -= X * speed;

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) new_pos -= Y * speed;
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) new_pos += Y * speed;

	trans->SetPosition(new_pos);
	scene_camera->SetFrustumPos(new_pos);
}

void ModuleCamera3D::Zoom(int mouse_z, float dt)
{
	Component_Transform* trans = (Component_Transform*)scene_camera->my_go->FindComponentWithType(TRANSFORM);

	float speed = 40.0f * dt;

	float3 X = { 1.0f, 0.0f, 0.0f };
	float3 Y = { 0.0f, 1.0f, 0.0f };
	float3 Z = { 0.0f, 0.0f, 1.0f };

	float3 new_pos = trans->GetPosition();

	if (mouse_z == 1)
		wheel_inverted ? new_pos -= Z * speed * -sensitivity : new_pos -= Z * speed * sensitivity;
	else if (mouse_z == -1)
		wheel_inverted ? new_pos += Z * speed * -sensitivity : new_pos += Z * speed * sensitivity;

	trans->SetPosition(new_pos);
	scene_camera->SetFrustumPos(new_pos);
}

void ModuleCamera3D::DrawAllFrustums()
{
	for (int i = 0; i < cameras.size(); i++)
	{
		if (cameras[i] != scene_camera)
		{
			float3 vertices[8];
			cameras[i]->GetFrustum().GetCornerPoints(vertices);

			App->debug->DrawBox(vertices, 1.5f, Red);
		}
	}
}

// ===========================================================================================================
// Component Camera
// ===========================================================================================================

Component_Camera* ModuleCamera3D::CreateComponentCamera(std::string name)
{
	GameObject* my_go = App->scene->CreateGameObject(name, App->scene->root_node);
	Component_Camera* ccamera = new Component_Camera();
	ccamera->my_go = my_go;
	my_go->AddComponent(ccamera);
	cameras.push_back(ccamera);
	return ccamera;
}