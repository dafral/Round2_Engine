#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Globals.h"
#include "Component.h"
#include "ModuleJSON.h"
#include "MathGeoLib/MathGeoLib.h"

class Component_Camera : public Component
{
public:
	Component_Camera();
	virtual ~Component_Camera() {};

	float* GetViewMatrix() const;
	float* GetProjectionMatrix() const;
	Frustum GetFrustum() { return frustum; };	
	float3 GetPosition() { return frustum.pos; };
	uint GetFOV() { return vertical_fov; };

	// ------------------------------------------

	void SetFOV(uint h_fov);
	void SetFrustumPos(float3 pos);
	void SetFrustumRot(float dx, float dy);
	void SetFrustumRot(Quat rot);
	void SetAspectRatio(float aspect_ratio);
	void SetPlanes(float n_near_plane, float n_far_plane);

	// -------------------------------------------

	bool IsAABBInside(AABB &aabb);
	bool IsSphereInside(Sphere &sphere);
	void OnSave(JSON_Doc* filetosave);

	// --------------------------------------------

	void LookAt(float3 spot);
	void MoveAt(float3 new_pos);

private:
	Frustum frustum;
	float aspect_ratio;
	uint vertical_fov;
};

#endif // !__C_CAMERA_H__
