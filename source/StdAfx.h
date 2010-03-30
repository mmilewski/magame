#define USE_PRECOMPILED_HEADERS

#ifndef __STDAFX_H_INCLUDED__
#define __STDAFX_H_INCLUDED__

#ifdef USE_PRECOMPILED_HEADERS


//#include "targetver.h"
#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif
// end of targetver.h

#include <sstream>
#include <iostream>
#include <fstream>

#include <cassert>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cctype>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_mixer.h>
#include <algorithm>

#include <string>
#include <vector> 

//#include <cstdio>
//#include <tchar.h>

#endif  // USE_PRECOMPILED_HEADERS
#endif  // __STDAFX_H_INCLUDED__
