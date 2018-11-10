#ifndef __MODULEJSON_H__
#define __MODULEJSON_H__

#include "Module.h"
#include "Parson/parson.h"
#include <list>

#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"

class JSON_Doc
{
public:
	JSON_Doc(JSON_Value* value, JSON_Object* object, const char* path);
	JSON_Doc(JSON_Object* Entry);
	~JSON_Doc();

	void SetString(const char* set, const char* str);
	void SetBool(const char* set, bool bo);
	void SetNumber(const char* set, double nu);
	void SetNumber3(const char* set, float3 vals);
	void SetNumber4(const char* set, float4 vals);
	void SetArray(const char* array_name);
	const char* GetString(const char* str);
	bool GetBool(const char* bo);
	double GetNumber(const char* nu);
	JSON_Doc GetArray(const char* field, int index) const;
	int GetArraySize(const char * field) const;

	JSON_Doc SetEntry(const char* set);
	JSON_Doc GetEntry(const char* set) const;
	const char* GetPath();
	void AddNumberToArray(const char* arr, double set);
	void AddSectionToArray(const std::string& array_keyword);
	bool MoveToSectionFromArray(const char * field, int index);

	void MoveToFirstObject();

	void Save();
	void CleanUp();

private:
	JSON_Value*  value = nullptr;
	JSON_Object* object = nullptr;
	JSON_Object* first_obj = nullptr;
	const char*  path;

};

class ModuleJSON : public Module
{
public:
	ModuleJSON(Application* app, bool start_enabled = true);
	~ModuleJSON();

	bool Init();
	bool CleanUp();

	JSON_Doc* LoadJSON(const char* path);
	JSON_Doc* CreateJSON(const char* path);


private:
	std::list<JSON_Doc*> jsons;

public:
	JSON_Doc* config = nullptr;

};

#endif // !__MODULEJSON_H__

