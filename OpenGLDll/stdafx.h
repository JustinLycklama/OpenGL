// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN             

// Windows Header Files:
#include <windows.h>
#include <cassert>
#include <stdexcept>

#include <fstream>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

// third-party libraries
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

//uses stb_image to try load files
#include <stb_image.c>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

// Local includes
#include "InterfaceMathDefinitions.h"
#include "InterfaceMath.h"
#include "OpenGLDll.h"

#include "Program.h"
#include "Mesh.h"
#include "Texture.h"
#include "Bitmap.h"

#include "Viewer.h"
#include "Camera.h"

#include "Asset.h"
#include "Instance.h"

#include "Light.h"
