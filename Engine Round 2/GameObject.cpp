#include "GameObject.h"
#include "Application.h"
#include "Component_Transform.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
#include "Component_Camera.h"

#include "PCG/pcg_basic.h"

GameObject::GameObject(std::string name, GameObject* parent) : name(name), parent(parent)
{
	unique_id = pcg32_random_r(&App->rng);
}

GameObject::~GameObject()
{}

void GameObject::Update()
{
	Component_Mesh* mesh = (Component_Mesh*)FindComponentWithType(MESH);

	if (!is_static && mesh != nullptr)
	{
		App->renderer3D->AddGOToDraw(this, App->camera->GetSceneCamera());
		App->renderer3D->AddGOToDraw(this, App->camera->GetGameCamera());
	}

	// Recursion
	for (int i = 0; i < childrens.size(); i++)
		childrens[i]->Update();
}

void GameObject::CleanUp() 
{

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->CleanUp();
		delete components[i];
	}

	components.clear();
	childrens.clear();
	////Clean all components
	//for (std::vector<Component*>::iterator c = components.begin(); c != components.end();)
	//{
	//	(*c)->CleanUp();
	//	RELEASE(*c);

	//	c = components.erase(c);
	//}

	////Clean all childs
	//for (std::vector<GameObject*>::iterator go = childrens.begin(); go != childrens.end();)
	//{
	//	(*go)->CleanUp();
	//	RELEASE(*go);

	//	go = App->scene->gameobjects.erase(go);
	//	go = childrens.erase(go);
	//}
}

void GameObject::Draw(bool is_scene_camera)
{
	Component_Mesh* mesh = (Component_Mesh*)FindComponentWithType(MESH);
	if (mesh != nullptr)
	{
		glPushMatrix();

		Component_Material* texture = (Component_Material*)FindComponentWithType(MATERIAL);
		Component_Transform* trans = (Component_Transform*)FindComponentWithType(TRANSFORM);

		if (trans != nullptr) glMultMatrixf(trans->GetGlobalTransform().ptr());

		glColor3f(mesh->GetColor().x, mesh->GetColor().y, mesh->GetColor().z);

		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->GetIdVertices());
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIdIndices());

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->GetIdUVs());
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);

		if (texture != nullptr) glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());
		glDrawElements(GL_TRIANGLES, mesh->GetNumIndices(), GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);

		if (App->debug->IsDebugDrawActive() && is_scene_camera) DrawDebug(mesh);

		glPopMatrix();
	}
}

void GameObject::DrawDebug(Component_Mesh* curr_mesh)
{
	float3 vertices[8];
	curr_mesh->GetBoundingVolume().bounding_box.GetCornerPoints(vertices);
	App->debug->DrawBox(vertices, 1.5, Green);

	App->debug->DrawSphere(curr_mesh->GetBoundingVolume().bounding_box.CenterPoint(), curr_mesh->GetBoundingVolume().sphere.r, 1.5, Blue);
}


void GameObject::AddChildren(GameObject* children)
{
	if (children != nullptr)
		childrens.push_back(children);
}

void GameObject::AddComponent(Component* component)
{
	if (component != nullptr) 
	{
		components.push_back(component);
		component->my_go = this;
	}
}

Component* GameObject::FindComponentWithType(Component_Type type)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type) return components[i];
	}

	return nullptr;
}

void GameObject::SetStatic(bool new_static) 
{
	is_static = new_static; 

	// Adding to or removing from static list
	if (is_static) App->octree->AddStatic(this);
	else App->octree->RemoveStatic(this);

	// Recursion
	for (int i = 0; i < childrens.size(); i++)
	{
		childrens[i]->SetStatic(new_static);
	}
};

void GameObject::SetVisible(bool new_visible)
{ 
	is_visible = new_visible; 

	// Recursion
	for (int i = 0; i < childrens.size(); i++)
	{
		childrens[i]->SetVisible(new_visible);
	}
};

void GameObject::SetUniqueID(uint id)
{
	unique_id = id;
}

void GameObject::GetChildrenByUniqueID(uint UID, GameObject *& go) const
{
	for (int i = 0; i < childrens.size(); ++i)
	{
		if (childrens[i]->GetUniqueID() == UID)
		{
			go = childrens[i];
			break;
		}
		else
		{
			childrens[i]->GetChildrenByUniqueID(UID, go);
			if (go != nullptr)
				break;
		}
	}
}
// ------------------------------------------------------------------

void GameObject::OnSave(JSON_Doc* filetosave)
{

	//Save this game object
	filetosave->AddSectionToArray("GameObjects");
	filetosave->MoveToSectionFromArray("GameObjects", filetosave->GetArraySize("GameObjects") - 1);
	filetosave->SetNumber("unique ID", unique_id);
	filetosave->SetString("name", name.c_str());
	
	filetosave->SetBool("root", false);
	if(this == App->scene->GetRootNode())
		filetosave->SetBool("root", true);
	
	filetosave->SetBool("visible", is_visible);
	filetosave->SetBool("is_static", is_static);
	
	if (parent != nullptr)
		filetosave->SetNumber("parent", parent->GetUniqueID());
	else
		filetosave->SetNumber("parent", 0);
	filetosave->MoveToFirstObject();

	//Save components 
	for (std::vector<Component*>::iterator c = components.begin(); c != components.end(); ++c)
	{
		(*c)->OnSave(filetosave);
		filetosave->MoveToFirstObject();
	}

	filetosave->MoveToFirstObject();


};

