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

	Move(dt);

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->editor->scene->isHovered())
		Rotate(dt);

	if (App->input->GetMouseButton(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) && App->editor->scene->isHovered())
		LookAt({ 0, 0, 0 });

	if (App->debug->IsDebugDrawActive()) DrawAllFrustums();

	// Mouse picking -------------------------------

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->editor->scene->isHovered())
	{
		float2 mouse_pos_norm = App->editor->scene->GetMousePosNormalized();
		LineSegment mouse_ray = scene_camera->GetFrustum().UnProjectLineSegment(mouse_pos_norm.x, mouse_pos_norm.y);

		if (mouse_ray.Length() != 0)
			App->scene->RayCast(mouse_ray);
	}

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::LookAt(float3 spot)
{
	scene_camera->LookAt(spot);
}

void ModuleCamera3D::MoveAt(float3 new_pos)
{
	scene_camera->MoveAt(new_pos);
}

void ModuleCamera3D::Rotate(float dt)
{
	float dx = 0;
	float dy = 0;

	orb_x_inverted ? dx += App->input->GetMouseXMotion() * sensitivity * dt : dx += -App->input->GetMouseXMotion() * sensitivity * dt;
	orb_y_inverted ? dy += App->input->GetMouseYMotion() * sensitivity * dt : dy += -App->input->GetMouseYMotion() * sensitivity * dt;

	scene_camera->SetFrustumRot(dx, dy);
}

void ModuleCamera3D::Move(float dt)
{
	float speed = 10.f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 2.f;

	float3 pos = scene_camera->GetPosition();
	float3 new_pos(0, 0, 0);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) new_pos += scene_camera->GetFrustum().front * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) new_pos -= scene_camera->GetFrustum().front * speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) new_pos -= scene_camera->GetFrustum().WorldRight() * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) new_pos += scene_camera->GetFrustum().WorldRight() * speed;

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) new_pos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) new_pos.y -= speed;

	scene_camera->SetFrustumPos(pos + new_pos);
}

void ModuleCamera3D::Zoom(int mouse_z, float dt)
{
	float speed = 30.0f * dt;

	float3 pos = scene_camera->GetPosition();
	float3 new_pos(0, 0, 0);

	if (mouse_z == 1)
		wheel_inverted ? new_pos -= scene_camera->GetFrustum().front * -speed  : new_pos -= scene_camera->GetFrustum().front * speed;
	else if (mouse_z == -1)
		wheel_inverted ? new_pos += scene_camera->GetFrustum().front * -speed : new_pos += scene_camera->GetFrustum().front * speed;

		scene_camera->SetFrustumPos(pos + new_pos);
}

void ModuleCamera3D::DrawAllFrustums()
{
	for (int i = 0; i < cameras.size(); i++)
	{
		if (cameras[i] != scene_camera)
		{
			float3 vertices[8];
			cameras[i]->GetFrustum().GetCornerPoints(vertices);

			App->debug->DrawBox(vertices, 3.f, Red);
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