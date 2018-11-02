#ifndef __P_PROPERTIES_H__
#define __P_PROPERTIES_H__

#include "Globals.h"
#include "Panel.h"

class GameObject;
class Component_Mesh;
class Component_Transform;
class Component_Material;

class PanelProperties : public Panel
{
public:
	PanelProperties(bool active);
	virtual ~PanelProperties();

	void Init();
	void Draw();

private:
	void GOInfo(GameObject* go);
	void MeshInfo(Component_Mesh* mesh);
	void MaterialInfo(Component_Material* material);
	void TransInfo(Component_Transform* trans, bool is_static);
};

#endif // !__P_PROPERTIES_H__