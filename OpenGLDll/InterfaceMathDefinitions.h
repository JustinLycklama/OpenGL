#pragma once

#ifdef OPENGLDLL_EXPORTS   
#define EXPORT __declspec(dllexport) 
#else   
#define EXPORT __declspec(dllimport) 
#endif

struct point
{
	double x;
	double y;
	double z;
};

typedef point atttvector;