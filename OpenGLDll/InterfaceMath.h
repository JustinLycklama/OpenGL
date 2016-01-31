#pragma once

#include "InterfaceMathDefinitions.h"

#ifdef OPENGLDLL_EXPORTS   
#define EXPORT __declspec(dllexport) 
#else   
#define EXPORT __declspec(dllimport) 
#endif

class InterfaceMath
{
public:
	InterfaceMath(void);
	~InterfaceMath(void);

	//static point location();
};

