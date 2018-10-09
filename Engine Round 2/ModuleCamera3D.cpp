#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	positive = true;
	x = 0;

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
	MoveCamera();

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		RotateCamera();

	// Look at Target
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		LookAt({ 0, 0, 0 });

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------

void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------

void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------

void ModuleCamera3D::Move(const vec3 &Movement)
{
	vec3 newPos(0, 0, 0);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * sensitivity;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * sensitivity;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * sensitivity;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * sensitivity;

	Position += newPos;
	Reference += newPos;

	Position += Movement;
	Reference += Movement;
}

// -----------------------------------------------------------------

float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------

void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

// -----------------------------------------------------------------

void ModuleCamera3D::MoveCamera()
{
	//Forwards & Backwards
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) 
	{
		Position -= Z * sensitivity;
		Reference -= Z * sensitivity;
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) 
	{
		Position += Z * sensitivity;
		Reference += Z * sensitivity;
	}

	//Left & Right
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
	{
		Position -= X * sensitivity;
		Reference -= X * sensitivity;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
	{
		Position += X * sensitivity;
		Reference += X * sensitivity;
	}

	//Up & Down
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
	{
		Position += Y * sensitivity;
		Reference += Y * sensitivity;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	{
		Position -= Y * sensitivity;
		Reference -= Y * sensitivity;
	}
}

void ModuleCamera3D::RotateCamera()
{
	int oldX;
	int oldY;

	orb_x_inverted ? oldX = App->input->GetMouseXMotion() : oldX = -App->input->GetMouseXMotion();
	orb_y_inverted ? oldY = App->input->GetMouseYMotion() : oldY = -App->input->GetMouseYMotion();

	Position -= Reference;

	if (oldX != 0)
	{
		float DeltaX = (float)oldX * sensitivity;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (oldY != 0)
	{
		float DeltaY = (float)oldY * sensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	Position = Reference + Z * length(Position);
}

// ------------------------------------------------------------------

TextureMSAA* ModuleCamera3D::GetSceneTexture()
{
	return scene_texture;
}