#pragma once
#include "Component.h"
#include "Globals.h"

// 0..1
class Component_Material : public Component
{
public:
	Component_Material() : Component(MATERIAL) {};
	virtual ~Component_Material() {};

	uint GetTextureId() { return texture_id; };
	void SetTextureId(uint id) { texture_id = id; };
	void LoadBuffers();

private:
	uint texture_id = 0;

};
