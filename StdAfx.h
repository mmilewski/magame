#define USE_PRECOMPILED_HEADERS

#ifndef __STDAFX_H_INCLUDED__
#define __STDAFX_H_INCLUDED__

#ifdef USE_PRECOMPILED_HEADERS

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <stdexcept>  // logic_error
#include <cassert>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cctype>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/operators.hpp>
#include <boost/bind.hpp>

#include <string>
#include <vector> 
#include <deque>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <utility>


#endif  // USE_PRECOMPILED_HEADERS
#endif  // __STDAFX_H_INCLUDED__
