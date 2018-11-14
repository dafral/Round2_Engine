#pragma once
#include "Component.h"
#include "Globals.h"
#include "ModuleJSON.h"
#include "ImGui/imgui.h"

// 0..1
class Component_Material : public Component
{
public:
	Component_Material();
	Component_Material(uint id, uint w, uint h);
	Component_Material(uint uid, uint id, uint w, uint h);
	virtual ~Component_Material() {};

	uint GetTextureId() { return texture_id; };
	void SetTextureId(uint id) { texture_id = id; };
	void SetTextureDimensions(uint w, uint h) { width = w; height = h; };
	void SetTextureSnap(ImTextureID s) { snap = s; };
	uint GetTextureWidth() { return width; };
	uint GetTextureHeight() { return height; };
	ImTextureID GetTextureSnap() { return snap; };
	uint GetUniqueID() { return unique_id; };
	void LoadBuffers();

	void OnSave(JSON_Doc* filetosave);

private:
	uint texture_id = 0;
	uint width, height = 0;
	ImTextureID snap = nullptr;

	uint unique_id = 0;
};
