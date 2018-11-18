#ifndef __COMPONENT_H__
#define __COMPONENT_H__

enum Component_Type 
{
	TRANSFORM, 
	MESH, 
	MATERIAL,
	CAMERA
};

class GameObject;
class JSON_Doc;

class Component
{
public: 
	Component(Component_Type type) : type(type) {};
	virtual ~Component() {};

	virtual void Update() {};
	virtual void CleanUp() {};
	virtual void Enable() {};
	virtual void Disable() {};

	virtual void OnSave(JSON_Doc* filetosave) {};

public:
	Component_Type type;
	GameObject* my_go;
};

#endif // !__COMPONENT_H__