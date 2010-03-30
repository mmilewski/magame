#ifndef __PLAYERBULLET_ENTITY_H__
#define __PLAYERBULLET_ENTITY_H__
#include "StdAfx.h"

#include "Entity.h"
#include <iostream>


class PlayerBulletEntity : public Entity {
    enum {
        DefaultXVelocity = 6, DefaultYVelocity = -2,
        DefaultXAcceleration = 0, DefaultYAcceleration = 0,
        DefaultTimeToLive = 2   // czas życia = 2s
    };

public:
    PlayerBulletEntity(double x, double y) :
        Entity(x, y, DefaultXVelocity, DefaultYVelocity,
               DefaultXAcceleration, DefaultYAcceleration),
        m_time_to_live(DefaultTimeToLive) {
    }

    virtual ET::EntityType GetType() const { return ET::PlayerBullet; }
    virtual Aabb GetBasicAabb() const {
        return Aabb(0, 0, .4, .4);
    }

    void SetDefaultMovement() {
    }

    void Update(double dt, LevelPtr level) {
        // usuń obiekt jeżeli żyje zbyt długo
        m_time_to_live -= dt;
        if (m_time_to_live < 0) {
            SetIsDead(true);
            return;
        }

        // sprawdź kolizje
        CheckCollisionsWithLevel(dt, level);
        SetPosition(GetNextXPosition(dt), GetNextYPosition(dt));
    }

    virtual void CheckCollisionsWithLevel(double dt, LevelPtr level) {
        // czy jednostka koliduje z czymś od dołu lub od góry
        if (IsAnyFieldBelowMe(dt, level) || IsAnyFieldAboveMe(dt, level)) {
            NegateYVelocity();
        }

        // czy jednostka koliduje z czymś po lewej lub prawej stronie
        if (IsAnyFieldOnLeft(dt, level)||IsAnyFieldOnRight(dt, level)) {
            NegateXVelocity();
        }
    }

private:
    double m_time_to_live;   // czas, który pozostał do samozniszczenia
};
typedef boost::shared_ptr<PlayerBulletEntity> PlayerBulletEntityPtr;

#endif
