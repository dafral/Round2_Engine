#pragma once

enum Component_Type 
{
	TRANSFORM, 
	MESH, 
	TEXTURE
};

class GameObject;

class Component
{
public: 
	Component(Component_Type type) : type(type) {};
	virtual ~Component() {};

	virtual void Update() {};
	virtual void Enable() {};
	virtual void Disable() {};

public:
	Component_Type type;
	GameObject* my_go;
};