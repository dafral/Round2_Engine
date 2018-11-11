#ifndef __P_CONFIGURATION_H__
#define __P_CONFIGURATION_H__

#include "Panel.h"

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration(bool active);
	virtual ~PanelConfiguration();

	void Init();
	void Draw();

	void ApplicationConfig();
	void RendererConfig();
	void WindowConfig();
	void InputConfig();
	void TexturesConfig();
	void AboutConfig();

	void AddFps(float fps);
	void AddMs(float ms);
	int GetFPSCap();

	//application
	std::vector<float> fps_buffer;
	std::vector<float> ms_buffer;
	int fps_cap = 60;
	bool vsync = true;

	//window
	float brightness;
	float width;
	float height;
	bool bordered = false;
	bool resizable = true;

	//render
	bool grid = true;
	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2d = true;
	bool blend = true;
	bool dither = true;
	bool wireframe = false;
	bool points = false;
	bool color = true;
};


#endif