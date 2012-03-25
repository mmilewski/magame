#ifndef MAGAME_SAVEPOINT_H_INCLUDED
#define	MAGAME_SAVEPOINT_H_INCLUDED
#include "StdAfx.h"

#include "Entity.h"

class SavePoint : public Entity {
public:
    explicit SavePoint(double x, double y);

    virtual ET::EntityType GetType() const { return ET::SavePoint; }

    virtual void Update(double dt, LevelPtr level);
    virtual void Draw() const;
    void SetSprites(SpritePtr active, SpritePtr inactive);

    void Activate();
    bool IsActive() const     { return m_is_active; }
    bool IsInactive() const   { return !IsActive(); }

private:
    bool m_is_active;                   // Czy punkt jest aktywny (gra zapisana)
    SpritePtr m_sprite_active;          // Animacja aktywnego punktu
    SpritePtr m_sprite_inactive;        // Animacja nieaktywego punktu
};

typedef boost::shared_ptr<SavePoint> SavePointPtr;

#endif
