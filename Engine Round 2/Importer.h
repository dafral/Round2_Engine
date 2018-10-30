#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Globals.h"

#include <string>

template <class TYPE>

class Importer
{
public:
	virtual TYPE* Load(const char* myfile) { return false; };
	virtual bool Save(const char* myfile) { return false; }
};


#endif // !__IMPORTER_H__
