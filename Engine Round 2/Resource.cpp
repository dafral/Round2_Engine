#include "Application.h"
#include "Resource.h"

Resource::Resource()
{}

Resource::~Resource()
{}

uint Resource::GetUID() const
{
	return uid;
}