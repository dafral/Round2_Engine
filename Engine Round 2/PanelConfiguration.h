#ifndef __P_CONFIGURATION_H__
#define __P_CONFIGURATION_H__

#include "Panel.h"

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration(bool active);
	virtual ~PanelConfiguration();

	void Draw();

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