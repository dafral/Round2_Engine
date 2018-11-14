#include "Application.h"
#include "PanelResources.h"
#include "ModuleFileSystem.h"
#include <string>
#include <vector>

PanelResources::PanelResources(bool active = true) : Panel(active)
{}

PanelResources::~PanelResources()
{}

void PanelResources::Draw()
{
	//ImGui::BeginDock("Resources monitoring", false, &visible, false, flags);

	//std::vector<ResourceLoader*> loaders = App->resource_manager->GetLoaders();

	//for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	//{
	//	ResourceLoader* curr_loader = (*it);

	//	std::map<std::string, Resource*> resources = curr_loader->GetResources();

	//	for (std::map<std::string, Resource*>::iterator re = resources.begin(); re != resources.end(); ++re)
	//	{
	//		Resource* curr_resource = (*re).second;

	//		ImGui::Text(curr_resource->GetUniqueId().c_str());
	//	}
	//}

	//ImGui::EndDock();
}
