#ifndef MAGAME_CREATOR_H_INCLUDED
#define MAGAME_CREATOR_H_INCLUDED
#include "StdAfx.h"

class Game;

class Creator {
public:
    virtual void Create(Game& game) = 0;
    virtual ~Creator() {}
};

typedef boost::shared_ptr<Creator> CreatorPtr;

#endif /* MAGAME_CREATOR_H_INCLUDED */
