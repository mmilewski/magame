#ifndef _CREATOR_H_INCLUDED__
#define _CREATOR_H_INCLUDED__
#include "StdAfx.h"

#include <boost/shared_ptr.hpp>

class Game;

class Creator {
public:
    virtual void Create(Game& game) = 0;
};

typedef boost::shared_ptr<Creator> CreatorPtr;

#endif
