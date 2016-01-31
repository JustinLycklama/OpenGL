
#include "Viewer.h"
#include "Camera.h"

#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

enum ErrorType
{
	ErrorTypeError
};

Viewer* viewer;
Camera* camera;

bool EXPORT initialize(ErrorType &error);
