#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Globals.h"
#include "ModuleJSON.h"
#include "ImGui/imgui.h"

struct Material
{
	uint texture_id = 0;
	uint width, height = 0;
};

// 0..1
class Component_Material : public Component
{
public:
	Component_Material();
	Component_Material(uint id, uint w, uint h);
	Component_Material(uint uid, uint id, uint w, uint h);
	virtual ~Component_Material() {};

	uint GetTextureId() { return material.texture_id; };
	void SetTextureId(uint id) { material.texture_id = id; };
	void SetTextureDimensions(uint w, uint h) { material.width = w; material.height = h; };
	void SetTextureSnap(ImTextureID s) { snap = s; };
	uint GetTextureWidth() { return material.width; };
	uint GetTextureHeight() { return material.height; };
	ImTextureID GetTextureSnap() { return snap; };
	uint GetUniqueID() { return unique_id; };
	void SetUniqueID(uint uid) { unique_id = uid; };
	void LoadBuffers();

	void OnSave(JSON_Doc* filetosave);

private:
	Material material;
	ImTextureID snap = nullptr;
	uint unique_id = 0;
};

#endif
