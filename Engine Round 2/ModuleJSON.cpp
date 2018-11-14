#include <string>
#include "Application.h"
#include "ModuleJSON.h"

JSON_Doc::JSON_Doc(JSON_Value * value, JSON_Object * object, const char * path)
{
	this->value = value;
	this->object = object;
	this->first_obj = object;
	this->path = path;
}

JSON_Doc::JSON_Doc(JSON_Object * Entry) :object(Entry)
{}

JSON_Doc::~JSON_Doc()
{}

void JSON_Doc::SetString(const char * set, const char * str)
{
	json_object_dotset_string(object, set, str);
}

void JSON_Doc::SetBool(const char * set, bool bo)
{
	json_object_dotset_boolean(object, set, bo);
}

void JSON_Doc::SetNumber(const char * set, double nu)
{
	json_object_dotset_number(object, set, nu);
}

void JSON_Doc::SetNumber3(const char * set, float3 vals)
{
	SetArray(set);
	AddNumberToArray(set, vals.x);
	AddNumberToArray(set, vals.y);
	AddNumberToArray(set, vals.z);
}

void JSON_Doc::SetNumber4(const char * set, float4 vals)
{
	SetArray(set);
	AddNumberToArray(set, vals.x);
	AddNumberToArray(set, vals.y);
	AddNumberToArray(set, vals.w);
	AddNumberToArray(set, vals.z);
}

void JSON_Doc::SetArray(const char * array_name)
{
	JSON_Value* val = json_value_init_array();
	JSON_Array* arra = json_value_get_array(val);
	json_object_set_value(object, array_name, val);
}

const char * JSON_Doc::GetString(const char * str)
{
	return json_object_dotget_string(object, str);
}

bool JSON_Doc::GetBool(const char * bo)
{
	return json_object_dotget_boolean(object, bo);
}

double JSON_Doc::GetNumber(const char* nu)
{
	return json_object_dotget_number(object, nu);
}

const float3 JSON_Doc::GetNumber3(const char* fl)
{
	float3 ret = float3::zero;

	JSON_Array* array = json_object_get_array(object, fl);

	if (array != nullptr)
	{
		ret.x = GetNumberFromArray(fl, 0);
		ret.y = GetNumberFromArray(fl, 1);
		ret.z = GetNumberFromArray(fl, 2);
	}

	return ret;
}

const float4 JSON_Doc::GetNumber4(const char* fl)
{
	float4 ret = float4::zero;

	JSON_Array* array = json_object_get_array(object, fl);

	if (array != nullptr)
	{
		ret.x = GetNumberFromArray(fl, 0);
		ret.y = GetNumberFromArray(fl, 1);
		ret.w = GetNumberFromArray(fl, 2);
		ret.z = GetNumberFromArray(fl, 3);
	}

	return ret;
}

const double JSON_Doc::GetNumberFromArray(const char* arr, int index)
{
	double ret = 0;

	JSON_Array* array = json_object_get_array(object, arr);

	if (array != nullptr)
	{
		if (FindArrayValue(arr, index, json_value_type::JSONNumber))
		{
			ret = json_array_get_number(array, index);
		}
	}

	return ret;
}

const char * JSON_Doc::GetPath()
{
	return path;
}

void JSON_Doc::Save()
{
	json_serialize_to_file_pretty(value, path);
}

void JSON_Doc::CleanUp()
{
	json_value_free(value);
	value = json_value_init_object();
	object = json_value_get_object(value);
	first_obj = object;
}

JSON_Doc JSON_Doc::GetEntry(const char * set) const
{
	return JSON_Doc(json_object_get_object(object, set));
}

JSON_Doc JSON_Doc::SetEntry(const char * set)
{
	json_object_set_value(object, set, json_value_init_object());
	return GetEntry(set);
}

bool JSON_Doc::MoveToSectionFromArray(const char * field, int index)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(object, field);

	if (array != nullptr)
	{
		JSON_Object* obj = json_array_get_object(array, index);

		object = obj;
		ret = true;
	}

	return ret;
}

JSON_Doc JSON_Doc::GetArray(const char * field, int index) const
{
	JSON_Array* array = json_object_get_array(object, field);
	if (array != nullptr)
		return JSON_Doc(json_array_get_object(array, index));
	return JSON_Doc((JSON_Object*) nullptr);
}

int JSON_Doc::GetArraySize(const char * field) const
{
	int ret = -1;
	JSON_Array* array = json_object_get_array(object, field);
	if (array != nullptr)
		ret = json_array_get_count(array);
	return ret;
}

bool JSON_Doc::FindArrayValue(const char * arr, int index, json_value_type type)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(object, arr);

	if (array != nullptr)
	{
		JSON_Value* val = json_array_get_value(array, index);

		if (val != nullptr && json_value_get_type(val) == type)
			ret = true;
	}

	return ret;
}

void JSON_Doc::AddNumberToArray(const char* arr, double set)
{
	JSON_Array* array = json_object_get_array(object, arr);

	if (array != nullptr)
	{
		json_array_append_number(array, set);
	}
}

void JSON_Doc::AddSectionToArray(const std::string & array_keyword)
{
	JSON_Array* array = json_object_get_array(object, array_keyword.c_str());

	if (array != nullptr)
	{
		json_array_append_value(array, json_value_init_object());
	}
}

void JSON_Doc::MoveToFirstObject()
{
	object = first_obj;
}

//----------------------------------------------------------------------------------
// Constructor
ModuleJSON::ModuleJSON(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleJSON::~ModuleJSON()
{}

bool ModuleJSON::Init()
{
	CONSOLELOG("Module JSON Init");
	config = LoadJSON("Library/config.json");
	return true;
}

bool ModuleJSON::CleanUp()
{
	bool ret = true;

	CONSOLELOG("Unloading JSON Module");

	for (std::list<JSON_Doc*>::iterator it = jsons.begin(); it != jsons.end();)
	{
		(*it)->CleanUp();
		delete (*it);

		it = jsons.erase(it);
	}

	return ret;
}

JSON_Doc * ModuleJSON::LoadJSON(const char * path)
{
	JSON_Doc* ret = nullptr;

	bool exists = false;
	for (std::list<JSON_Doc*>::iterator it = jsons.begin(); it != jsons.end(); it++)
	{
		std::string doc_str = (*it)->GetPath();
		if (doc_str.compare(path))
		{
			ret = (*it);
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		JSON_Value *user_data = json_parse_file(path);
		JSON_Object *root_object = json_value_get_object(user_data);

		if (user_data == nullptr)
		{
			CONSOLELOG("Error loading %s", path);
		}
		else
		{
			CONSOLELOG("Succes loading %s", path);

			JSON_Doc* new_doc = new JSON_Doc(user_data, root_object, path);
			jsons.push_back(new_doc);

			ret = new_doc;
			//ret->Save();
		}
	}

	return ret;
}

JSON_Doc * ModuleJSON::CreateJSON(const char * path)
{
	JSON_Doc* ret = nullptr;

	bool exists = false;
	for (std::list<JSON_Doc*>::iterator it = jsons.begin(); it != jsons.end(); it++)
	{
		std::string doc_str = (*it)->GetPath();
		if (doc_str.compare(path))
		{
			exists = true;
			break;
		}
	}

	if (exists)
	{
		CONSOLELOG("Error creating %s. There is already a file with this path/name", path);
	}
	else
	{
		JSON_Value* root_value = json_value_init_object();

		if (root_value == nullptr)
		{
			CONSOLELOG("Error creating %s. Wrong path?", path);
		}
		else
		{

			JSON_Object* root_object = json_value_get_object(root_value);

			JSON_Doc* new_doc = new JSON_Doc(root_value, root_object, path);
			jsons.push_back(new_doc);

			ret = new_doc;
			ret->Save();
			CONSOLELOG("%s", ret->GetPath());
		}
	}

	return ret;
}