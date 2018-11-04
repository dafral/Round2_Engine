#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sensitivity = 0.25;
	orb_x_inverted = false;
	orb_y_inverted = false;
	wheel_inverted = false;
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------

bool ModuleCamera3D::Start()
{
	CONSOLELOG("Setting up the camera");
	bool ret = true;

	editor_camera = CreateComponentCamera();

	scene_texture = new TextureMSAA();
	scene_texture->Create(App->window->screen_surface->w, App->window->screen_surface->h);

	return ret;
}

// -----------------------------------------------------------------

bool ModuleCamera3D::CleanUp()
{
	CONSOLELOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------

update_status ModuleCamera3D::Update(float dt)
{
	// Editor camera -------------------------------

	Move(dt);

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		Rotate(dt);

	// Look at Target
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		LookAt({ 0, 0, 0 });

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------

void ModuleCamera3D::LookAt(const float3 & spot)
{
	float3 direction = spot - editor_camera->GetFrustum().pos;

	float3x3 matrix = float3x3::LookAt(editor_camera->GetFrustum().front, direction.Normalized(), editor_camera->GetFrustum().up, float3::unitY);

	editor_camera->GetFrustum().front = matrix.MulDir(editor_camera->GetFrustum().front).Normalized();
	editor_camera->GetFrustum().up = matrix.MulDir(editor_camera->GetFrustum().up).Normalized();
}

void ModuleCamera3D::Rotate(float dt)
{
	float dx = 0.0f, dy = 0.0f;

	orb_x_inverted ? dx = App->input->GetMouseXMotion() * sensitivity * dt : dx = -App->input->GetMouseXMotion() * sensitivity * dt;
	orb_y_inverted ? dy = App->input->GetMouseYMotion() * sensitivity * dt : dy = -App->input->GetMouseYMotion() * sensitivity * dt;

	editor_camera->Rotate(dx, dy);
}

void ModuleCamera3D::Move(float dt)
{
	float speed = 5.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 10.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) editor_camera->MoveForwards(speed);
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) editor_camera->MoveBackwards(speed);

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) editor_camera->MoveLeft(speed);
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) editor_camera->MoveRight(speed);

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) editor_camera->MoveUp(speed);
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) editor_camera->MoveDown(speed);
}

void ModuleCamera3D::Zoom(int mouse_z, float dt)
{
	float speed = 40.0f;

	if (mouse_z == 1)
		wheel_inverted ? editor_camera->MoveForwards(speed * -sensitivity * dt) : editor_camera->MoveForwards(speed *sensitivity * dt);
	else if (mouse_z == -1)
		wheel_inverted ? editor_camera->MoveForwards(speed * sensitivity * dt) : editor_camera->MoveForwards(speed *-sensitivity * dt);
}


// ------------------------------------------------------------------

float ModuleCamera3D::GetSensitivity()
{
	return sensitivity;
}

void ModuleCamera3D::SetSensitivity(float new_sen)
{
	sensitivity = new_sen;
}

// ------------------------------------------------------------------

TextureMSAA* ModuleCamera3D::GetSceneTexture()
{
	return scene_texture;
}

// =================================================================
// Component Camera
// =================================================================

Component_Camera* ModuleCamera3D::CreateComponentCamera()
{
	GameObject* my_go = new GameObject("name", App->scene->root_node);
	Component_Camera* ccamera = new Component_Camera();
	ccamera->my_go = my_go;
	my_go->AddComponent(ccamera);
	cameras.push_back(ccamera);
	return ccamera;
}