#include "Component_Camera.h"
#include "GameObject.h"

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

Component_Camera::Component_Camera() : Component(CAMERA)
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 100.0f;

	vertical_fov = 50;
	aspect_ratio = ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
	SetFOV(vertical_fov);

	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
}

// -----------------------------------------------------------------

void Component_Camera::MoveUp(const float movement)
{
	float3 move = float3::zero;
	move += frustum.up * movement;
	frustum.Translate(move);
}

void Component_Camera::MoveDown(const float movement)
{
	float3 move = float3::zero;
	move -= frustum.up * movement;
	frustum.Translate(move);
}

void Component_Camera::MoveLeft(const float movement)
{
	float3 move = float3::zero;
	move -= frustum.WorldRight() * movement;
	frustum.Translate(move);
}

void Component_Camera::MoveRight(const float movement)
{
	float3 move = float3::zero;
	move += frustum.WorldRight() * movement;
	frustum.Translate(move);
}

void Component_Camera::MoveForwards(const float movement)
{
	float3 move = float3::zero;
	move += frustum.front * movement;
	frustum.Translate(move);
}

void Component_Camera::MoveBackwards(const float movement)
{
	float3 move = float3::zero;
	move -= frustum.front * movement;
	frustum.Translate(move);
}

void Component_Camera::Rotate(const float dx, const float dy)
{
	if (dx != 0)
	{
		Quat rotation_x = Quat::RotateY(dx);
		frustum.front = rotation_x.Mul(frustum.front).Normalized();
		frustum.up = rotation_x.Mul(frustum.up).Normalized();
	}

	if (dy != 0)
	{
		Quat rotation_y = Quat::RotateAxisAngle(frustum.WorldRight(), dy);

		float3 new_up = rotation_y.Mul(frustum.up).Normalized();

		if (new_up.y > 0.0f)
		{
			frustum.up = new_up;
			frustum.front = rotation_y.Mul(frustum.front).Normalized();
		}
	}
}

void Component_Camera::TransformPos(float3 pos)
{
	frustum.pos = pos;
}

void Component_Camera::TransformRot(Quat rot)
{
	float3 new_rot = rot.ToEulerXYZ();

	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	Quat rotation_y = Quat::RotateY(new_rot.y);
	frustum.front = rotation_y.Mul(frustum.front.Normalized());
	frustum.up = rotation_y.Mul(frustum.up.Normalized());

	Quat rotation_x = Quat::RotateAxisAngle(frustum.WorldRight(), new_rot.x);
	frustum.front = rotation_x.Mul(frustum.front.Normalized());
	frustum.up = rotation_x.Mul(frustum.up.Normalized());

	Quat rotation_z = Quat::RotateZ(new_rot.z);
	frustum.front = rotation_z.Mul(frustum.front.Normalized());
	frustum.up = rotation_z.Mul(frustum.up.Normalized());
}

void Component_Camera::SetFOV(uint v_fov)
{
	vertical_fov = v_fov;
	frustum.verticalFov = DEGTORAD * v_fov;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov / 2.f) * aspect_ratio);
}

void Component_Camera::SetAspectRatio(float n_aspect_ratio)
{
	aspect_ratio = n_aspect_ratio;
}

void Component_Camera::SetPlanes(float n_near_plane, float n_far_plane)
{
	frustum.nearPlaneDistance = n_near_plane;
	frustum.farPlaneDistance = n_far_plane;
}

// -----------------------------------------------------------------

float* Component_Camera::GetViewMatrix() const
{
	float4x4 matrix = frustum.ViewMatrix();
	return (float*)matrix.Transposed().v;
}

float* Component_Camera::GetProjectionMatrix() const
{
	return (float*)frustum.ProjectionMatrix().Transposed().v;
}

// -----------------------------------------------------------------

void Component_Camera::OnSave(JSON_Doc* filetosave)
{
	filetosave->AddSectionToArray("Components");
	filetosave->MoveToSectionFromArray("Components", filetosave->GetArraySize("Components") - 1);

	filetosave->SetNumber("type", 3);
	filetosave->SetNumber("owner", my_go->GetUniqueID());

	filetosave->SetNumber("aspect_ratio", aspect_ratio);
	filetosave->SetNumber("vertical_fov", frustum.verticalFov);
	filetosave->SetNumber("near", frustum.nearPlaneDistance);
	filetosave->SetNumber("far", frustum.farPlaneDistance);
}