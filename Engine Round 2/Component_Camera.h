#pragma once

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class Component_Camera : public Component
{
public:
	Component_Camera();
	virtual ~Component_Camera() {};

	float* GetViewMatrix() const;
	float* GetProjectionMatrix() const;
	Frustum GetFrustum() { return frustum; };

	// Movement ----------------------------------

	void MoveUp(const float movement);
	void MoveDown(const float movement);
	void MoveLeft(const float movement);
	void MoveRight(const float movement);
	void MoveForwards(const float movement);
	void MoveBackwards(const float movement);
	void Rotate(const float dx, const float dy);

	// ------------------------------------------

	float3 GetPosition() { return frustum.pos; };

private:
	Frustum frustum;
	float aspect_ratio;
};
