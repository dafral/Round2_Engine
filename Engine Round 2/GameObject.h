#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "Component.h"

#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"
#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/Math/float3.h"


class GameObject
{
public:

	void Update();
	ComponentType* CreateComponent(ComponentType type);

	bool active = true;
	char* name;
	std::vector<Component*> components;
};

#endif
