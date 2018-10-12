#pragma once

#include "Globals.h"

class TextureMSAA
{
public:

	TextureMSAA();
	~TextureMSAA();

	bool Create(uint width, uint height);

	void Bind();
	void Render();
	void Unbind();

	uint GetTextureID();

private:

	uint framebuffer_msaa_id;
	uint renderbuffer_depth_id;
	uint renderbuffer_color_id;

	uint framebuffer_id;
	uint renderbuffer_id;

	uint texture_id;

	unsigned int width;
	unsigned int height;
};

