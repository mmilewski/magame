#ifndef __MISC_H_INCLUDED__
#define __MISC_H_INCLUDED__
#include "StdAfx.h"

#include "Entity.h"

class Orb : public Entity {
public:
    Orb(double x, double y)
        : Entity(x,y, 0, 0) {
    }

    ET::EntityType GetType() const { return ET::Orb; }
};


class HigherJumpUpgrade : public Entity {
public:
    HigherJumpUpgrade(double x, double y)
        : Entity(x,y, 0, 0) {
    }

    ET::EntityType GetType() const { return ET::HigherJump; }
};


class Thorns : public Entity {
public:
    Thorns(double x, double y)
        : Entity(x, y, 0, 0) {
    }

    virtual ET::EntityType GetType() const { return ET::Thorns; }
    virtual Aabb GetBasicAabb() const { return Aabb(0,0,1,.6); }

};
typedef boost::shared_ptr<Thorns> ThornsPtr;


class Arrow : public Entity {
public:
    Arrow(double x, double y)
        : Entity(x, y, 0, 0),
        m_last_known_state(ES::GoLeft)
        {
    }

    virtual void Update(double dt, LevelPtr level);
    virtual ET::EntityType GetType()  const { return ET::Arrow; }
    virtual Aabb GetBasicAabb()       const { return Aabb(0.05, 0.4, 0.95, 0.6); }
    virtual int GetScoresWhenKilled() const { return 50; }
    
private:
    virtual void CheckCollisionsWithLevel(double dt, LevelPtr level);

private:
    ES::EntityState m_last_known_state;   // ostatni stan inny niż ES::Stand
};
typedef boost::shared_ptr<Arrow> ArrowPtr;


class ArrowTrigger : public Entity {
public:
    enum Orientation { Left, Right };

    class ArrowCreator : public Creator {
        double m_x, m_y;                      // pozycja początkowa
        double m_vx, m_vy;                    // prędkość początkowa
    public:
        explicit ArrowCreator(double x, double y, double vx, double vy);
        virtual void Create(Game& game);
    };

    ArrowTrigger(double x, double y, Orientation orientation);

    void TriggerArrow();
    virtual void Update(double dt, LevelPtr level);
    virtual ET::EntityType GetType() const { return ET::ArrowTrigger; }
    virtual Aabb GetBasicAabb()      const { return Aabb(0,0,1,1); }

private:
    double m_seconds_since_last_shot;     // czas od ostatniego strzału
    const double m_cooldown;              // czas (w sekundach) między strzałami
    Orientation m_orientation;            // zwrot wyzwalacza (lewo lub prawo)
};
typedef boost::shared_ptr<ArrowTrigger> ArrowTriggerPtr;


#endif
