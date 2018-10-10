#ifndef __P_CONFIGURATION_H__
#define __P_CONFIGURATION_H__

#include "Panel.h"

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration(bool active);
	virtual ~PanelConfiguration();

	void Draw();
	void RendererConfig();
	void WindowConfig();
	void InputConfig();
	void TexturesConfig();

	//window
	float brightness;
	float width;
	float height;
	bool bordered = false;
	bool fullscreen = false;
	bool resizable = true;

	//input
	float sensitivity;

	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2d = true;
	bool blend = true;
	bool dither = true;
	bool wireframe = false;
};


#endif