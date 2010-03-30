#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "StdAfx.h"

#include <boost/shared_ptr.hpp> 
#include "Aabb.h"
#include "Sprite.h"
#include "Types.h"
#include "Engine.h"
#include "Level.h"
#include "CreatorProducer.h"


class Entity : public CreatorProducer {
public:
    explicit Entity(double x, double y, double def_velocity_x, double def_velocity_y,
                    double def_acceleration_x = 0, double default_acceleration_y = 0);

    virtual ET::EntityType GetType() const = 0;
    virtual int GetScoresWhenKilled() const { return 0; }
    virtual void Update(double dt, LevelPtr level);
    virtual void Draw() const;
    virtual void CheckCollisionsWithLevel(double dt, LevelPtr level);
    void SetSprites(SpritePtr left, SpritePtr right, SpritePtr stop);
    bool IsDead() const                { return m_is_dead; }
    bool IsAlive() const               { return !m_is_dead; }
    void SetIsDead(bool is_dead=true)  { m_is_dead = is_dead; }
    void KilledByPlayer();
    void KilledWithBullet();

    // zarządzanie położeniem 
    double GetX() const { return m_x; }
    double GetY() const { return m_y; }
    virtual double GetNextXPosition(double dt) const { return m_x + GetNextXVelocity(dt) * dt; }
    virtual double GetNextYPosition(double dt) const { return m_y + GetNextYVelocity(dt) * dt; }
    void SetX(double newx)                   { m_x = newx; }
    void SetY(double newy)                   { m_y = newy; }
    void SetPosition(double x, double y)     { m_x = x; m_y = y; }

    // zarządzanie prędkością
    double GetXVelocity() const              { return m_vx; }
    double GetYVelocity() const              { return m_vy; }
    double GetNextXVelocity(double dt) const { return m_vx + m_ax * dt; }
    double GetNextYVelocity(double dt) const { return m_vy + m_ay * dt; }
    double GetDefaultXVelocity() const       { return m_default_velocity_x; }
    double GetDefaultYVelocity() const       { return m_default_velocity_y; }
    void  NegateXVelocity()                  { m_vx = -m_vx; }
    void  NegateYVelocity()                  { m_vy = -m_vy; }
    void  NegateVelocity()                   { NegateXVelocity(); NegateYVelocity(); }
    void  SetXVelocity(double velocity)      { m_vx = velocity; }
    void  SetYVelocity(double velocity)      { m_vy = velocity; }
    void  SetVelocity(double vx, double vy)  { m_vx = vx; m_vy = vy; }

    // zarządzanie przyspieszeniem
    double GetXAcceleration() const          { return m_ax; }
    double GetYAcceleration() const          { return m_ay; }
    double GetDefaultXAcceleration() const   { return m_default_acceleration_x; }
    double GetDefaultYAcceleration() const   { return m_default_acceleration_y; }
    void  SetXAcceleration(double accel)     { m_ax = accel; }
    void  SetYAcceleration(double accel)     { m_ay = accel; }

    // podstawowa zmiana stanu ruchu jednostki
    virtual void GoLeft()       { m_vx -= GetDefaultXVelocity(); m_state=ES::GoLeft; }
    virtual void GoRight()      { m_vx += GetDefaultXVelocity(); m_state=ES::GoRight; }
    virtual void StopLeft()     { m_vx += GetDefaultXVelocity(); m_state=ES::Stand; }
    virtual void StopRight()    { m_vx -= GetDefaultXVelocity(); m_state=ES::Stand; }
    virtual void StopMovement() { m_vx = 0; m_state = ES::Stand; }
    void ForbidGoingLeft()      { m_can_go_left = false; }
    void ForbidGoingRight()     { m_can_go_right = false; }
    void Fall()                 { m_vy =  0.0; m_is_on_ground = false; }
    virtual void SetDefaultMovement()  { m_is_on_ground = false; m_can_go_right = m_can_go_left = true; }
    void EntityOnGround() {
        m_is_on_ground = true;
        m_vy = 0;
    }

    // kilka podstawowych informacji nt. pól mapy wokół jednostki
    bool IsAnyFieldBelowMe(double dt, LevelPtr level) const;
    bool IsAnyFieldAboveMe(double dt, LevelPtr level) const;
    bool IsAnyFieldOnLeft(double dt, LevelPtr level) const;
    bool IsAnyFieldOnRight(double dt, LevelPtr level) const;
    bool DoFieldsEndOnLeft(double dt, LevelPtr level) const;
    bool DoFieldsEndOnRight(double dt, LevelPtr level) const;

    // pod argumenty x, y zapisuje numer aktualnego kafla
    void GetCurrentTile(size_t *x, size_t *y) const {
        const size_t v_tiles_count = Engine::Get().GetRenderer()->GetVerticalTilesOnScreenCount();
        *y = (size_t)(v_tiles_count - (GetAabb().GetMinY() + GetAabb().GetMaxY()) / 2);
        *x = (size_t)(GetX() + GetBasicAabb().GetMaxX() / 2);
    }

    // prostokąt otaczający jednostkę bez uwzględniania pozycji jednostki
    virtual Aabb GetBasicAabb() const { return Aabb(0, 0, 1, 1);  }

    Aabb GetAabb() const { return GetBasicAabb().Move(m_x, m_y, m_x, m_y); }

    Aabb GetNextHorizontalAabb(double dt) const {
        return GetBasicAabb().Move(GetNextXPosition(dt), m_y, GetNextXPosition(dt), m_y);
    }

    Aabb GetNextVerticalAabb(double dt) const {
        return GetBasicAabb().Move(m_x, GetNextYPosition(dt), m_x, GetNextYPosition(dt));
    }

    Aabb GetNextAabb(double dt) const {
        return GetBasicAabb().Move(GetNextXPosition(dt), GetNextYPosition(dt),
                                   GetNextXPosition(dt), GetNextYPosition(dt) );
    }

private:
    double m_default_velocity_x;      // domyślna prędkość
    double m_default_velocity_y;
    double m_default_acceleration_x;  // domyślne przyspieszenie
    double m_default_acceleration_y;

    bool m_is_dead;           // czy jednostka jest martwa

protected:
    ES::EntityState m_state;  // stan, w którym znajduje się jednostka
    SpritePtr m_left;         // animacja, kiedy jednostka idzie w lewo
    SpritePtr m_right;        // animacja, kiedy jednostka idzie w prawo
    SpritePtr m_stop;         // animacja, kiedy jednostka stoi

    double m_x;               // położenie jednostki na osi odciętych
    double m_y;               // położenie jednostki na osi rzędnych
    double m_vx;              // prędkość w poziomie
    double m_vy;              // prędkość w pionie
    double m_ax;              // przyspieszenie w poziomie
    double m_ay;              // przyspieszenie w pionie
    bool m_is_on_ground;      // czy postać jest na podłożu
    bool m_can_go_left;       // czy postać może iść w lewo
    bool m_can_go_right;      // czy postać może iść w prawo
};

typedef boost::shared_ptr<Entity> EntityPtr;

#endif
