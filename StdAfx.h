#define USE_PRECOMPILED_HEADERS

#ifndef MAGAME_STDAFX_H_INCLUDED
#define MAGAME_STDAFX_H_INCLUDED

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
#include <memory>  // unique_ptr

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/operators.hpp>
#include <boost/bind.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/range/irange.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/remove.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/transform.hpp>

#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <utility>


// deleter for SDL_Surface (used by std::unique_ptr)
namespace std {
template<>
struct default_delete<SDL_Surface> {
  void operator()(SDL_Surface* surface) const {
    SDL_FreeSurface(surface);
  }
};
}

#endif  // USE_PRECOMPILED_HEADERS
#endif  // MAGAME_STDAFX_H_INCLUDED
