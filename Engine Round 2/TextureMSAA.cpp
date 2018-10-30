#include "TextureMSAA.h"
#include "Application.h"
#include "Module.h"
#include "ModuleWindow.h"

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

TextureMSAA::TextureMSAA()
{}

TextureMSAA::~TextureMSAA()
{}

// We have three tasks: creating the texture in which we’re going to render, 
// actually rendering something in it, and using the generated texture.

bool TextureMSAA::Create(uint width, uint height)
{
	this->width = width;
	this->height = height;

	// ---------------------------------------------------------------
	// What we’re going to render to is called a Framebuffer.
	// It’s a container for textures and an optional depth buffer.
	// ---------------------------------------------------------------

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &framebuffer_msaa_id);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_msaa_id);

	// ---------------------------------------------------------------
	// Create the texture which will contain the RGB output 
	// of our shader.
	// ---------------------------------------------------------------

	// The texture we're going to render to
	glGenTextures(1, &texture_id);
	// "Bind" the newly created texture: all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	// Poor filtering. Needed!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	// Give an empty image to OpenGL (the last "0")
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
	// ---------------------------------------------------------------
	// We also need a depth buffer. This is optional, depending 
	// on what you actually need to draw in your texture.
	// ---------------------------------------------------------------

	// The color buffer
	glGenRenderbuffers(1, &renderbuffer_color_id);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_color_id);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 0, GL_RGB8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// The depth buffer
	glGenRenderbuffers(1, &renderbuffer_depth_id);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_depth_id);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 0, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach MSAA render buffer objects to frame buffer object attachment points
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer_color_id);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_depth_id);

	// ---------------------------------------------------------------
	// Finally, we configure our framebuffer
	// ---------------------------------------------------------------

	// Create a normal (no MSAA) frame buffer object to hold a render-to-texture
	glGenFramebuffers(1, &framebuffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

	glGenRenderbuffers(1, &renderbuffer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach a texture to FBO color attachement point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	// Attach a rbo to FBO depth attachement point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_id);

	// ---------------------------------------------------------------
	// Something may have gone wrong during the process. This is 
	// how you check it:
	// ---------------------------------------------------------------

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	return true;
}

void TextureMSAA::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_msaa_id);
	glViewport(0, 0, width, height);
}

void TextureMSAA::Render()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer_msaa_id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer_id);
	glBlitFramebuffer(0, 0, width, height,  // src rect
					  0, 0, width, height,  // dst rect
		              GL_COLOR_BUFFER_BIT, // buffer mask
		              GL_LINEAR); // scale filter
}

void TextureMSAA::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, App->window->screen_surface->w, App->window->screen_surface->h);
}

uint TextureMSAA::GetTextureID()
{
	return texture_id;
}