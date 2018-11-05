#pragma once
#include "Component.h"
#include "Globals.h"
#include "ImGui/imgui.h"

// 0..1
class Component_Material : public Component
{
public:
	Component_Material() : Component(MATERIAL) {};
	virtual ~Component_Material() {};

	uint GetTextureId() { return texture_id; };
	void SetTextureId(uint id) { texture_id = id; };
	void SetTextureDimensions(uint w, uint h) { width = w; height = h; };
	void SetTextureSnap(ImTextureID s) { snap = s; };
	uint GetTextureWidth() { return width; };
	uint GetTextureHeight() { return height; };
	ImTextureID GetTextureSnap() { return snap; };
	void LoadBuffers();

private:
	uint texture_id = 0;
	uint width, height = 0;
	ImTextureID snap = nullptr;
};
