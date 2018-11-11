#ifndef __DEBUGDRAW_H__
#define __DEBUGDRAW_H__

#include "MathGeoLib/MathGeoLib.h"
#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

struct Color;

class DebugDraw
{
public:
	DebugDraw() {};
	~DebugDraw() {};

	void DrawBox(float3* corners, float line_width, Color color);
	bool IsDebugDrawActive() { return debug_draw; };
	void SetDebugDraw(bool active) { debug_draw = active; };

private:
	bool debug_draw = false;
};

#endif
