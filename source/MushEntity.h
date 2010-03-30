#ifndef __MUSH_ENTITY_H__
#define __MUSH_ENTITY_H__
#include "StdAfx.h"

#include <iostream>
#include "Types.h"
#include "Entity.h"


class MushEntity : public Entity {
    enum {
        DefaultXVelocity = 2, DefaultYVelocity = 0, DefaultXAcceleration = 0, DefaultYAcceleration = -60
    };

public:
    MushEntity(double x, double y) :
        Entity(x, y, DefaultXVelocity, DefaultYVelocity, DefaultXAcceleration, DefaultYAcceleration) {
    }

    ET::EntityType GetType() const   { return ET::Mush; }
    int GetScoresWhenKilled() const  { return 100; }

    Aabb GetBasicAabb() const {
        return Aabb(.05, 0, .9, .9);
    }

    void CheckCollisionsWithLevel(double dt, LevelPtr level) {
        // ruszaj się zamiast stać
        if (GetXVelocity() == 0) {
            GoLeft();
        }

        // czy jednostka koliduje z czymś od dołu
        if (IsAnyFieldBelowMe(dt, level)) {
            EntityOnGround();
        }

        // koniec podłoża lub brak przejścia z lewej strony
        if (DoFieldsEndOnLeft(dt, level) || IsAnyFieldOnLeft(dt, level)) {
            StopMovement();
            GoRight();
        }

        // koniec podłoża lub brak przejścia z prawej strony
        if (DoFieldsEndOnRight(dt, level) || IsAnyFieldOnRight(dt, level)) {
            StopMovement();
            GoLeft();
        }
    }
};
typedef boost::shared_ptr<MushEntity> MushEntityPtr;

#endif
