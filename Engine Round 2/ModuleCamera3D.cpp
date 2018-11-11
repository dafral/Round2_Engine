#include "Application.h"
#include "ModuleCamera3D.h"
#include "TextureMSAA.h"
#include "Component_Camera.h"
#include "Color.h"

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
	Move(dt);

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		Rotate(dt);

	if (App->debug->IsDebugDrawActive()) DrawAllFrustums();

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Rotate(float dt)
{
	float dx = 0.0f, dy = 0.0f;

	orb_x_inverted ? dx = App->input->GetMouseXMotion() * sensitivity * dt : dx = -App->input->GetMouseXMotion() * sensitivity * dt;
	orb_y_inverted ? dy = App->input->GetMouseYMotion() * sensitivity * dt : dy = -App->input->GetMouseYMotion() * sensitivity * dt;

	scene_camera->Rotate(dx, dy);
}

void ModuleCamera3D::Move(float dt)
{
	float speed = 5.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 10.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) scene_camera->MoveForwards(speed);
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) scene_camera->MoveBackwards(speed);

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) scene_camera->MoveLeft(speed);
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) scene_camera->MoveRight(speed);

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) scene_camera->MoveUp(speed);
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) scene_camera->MoveDown(speed);
}

void ModuleCamera3D::Zoom(int mouse_z, float dt)
{
	float speed = 40.0f;

	if (mouse_z == 1)
		wheel_inverted ? scene_camera->MoveForwards(speed * -sensitivity * dt) : scene_camera->MoveForwards(speed *sensitivity * dt);
	else if (mouse_z == -1)
		wheel_inverted ? scene_camera->MoveForwards(speed * sensitivity * dt) : scene_camera->MoveForwards(speed *-sensitivity * dt);
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