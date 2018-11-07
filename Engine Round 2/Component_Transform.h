#pragma once
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

// 0...1
class Component_Transform : public Component
{
public:
	Component_Transform();
	~Component_Transform();

	float3 GetPosition() { return position; };
	float3 GetScale() { return scale; };
	Quat GetRotation() { return rotation; };
	float3 GetEulerRotation() { return RadToDeg(rotation.ToEulerXYZ()); };

	void SetPosition(float3 pos) { position = pos; };
	void SetScale(float3 scl) { scale = scl; };
	void SetRotation(Quat rot) { rotation = rot; };
	void SetRotation(float3 rot) { rotation = Quat::FromEulerXYZ(rot.x, rot.y, rot.z); };

	float4x4 GetTransform();
	const float4x4 GetGlobalTransform();

	void OnSave(JSON_Doc* filetosave);

private:
	float3 position;
	float3 scale;
	Quat rotation;

	float3 euler_rotation;
	float4x4 transform;
};
