#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "Component.h"
#include <string>

#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"
#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/Math/float3.h"

class GameObject
{
public:

	GameObject();
	~GameObject();

	void Update();
	void AddComponent(Component* component);

	bool active = true;
	std::string name;
	std::vector<Component*> components;
};

#endif
