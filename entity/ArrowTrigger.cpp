#include "StdAfx.h"

#include "Misc.h"
#include "Game.h"

ArrowTrigger::ArrowTrigger(double x, double y, Orientation orientation)
    : Entity(x, y, 0, 0),
      m_seconds_since_last_shot(999.0),
      m_cooldown(3.0),
      m_orientation(orientation) {
}

void ArrowTrigger::Update(double dt, LevelPtr /* level */) {
    m_seconds_since_last_shot += dt;
    if (m_seconds_since_last_shot >= m_cooldown) {
        TriggerArrow();
        m_seconds_since_last_shot = 0.0;
    }

    SetStateFromVelocity( (m_orientation==Left) ? -1 : 1 );
    UpdateSpriteFromState(dt);
}

void ArrowTrigger::TriggerArrow() {
    double vel_x = 3;
    if (m_orientation == Left) {
        vel_x *= -1;
    }
    AddCreator(CreatorPtr(new ArrowCreator(GetX(), GetY(), vel_x, 0)));
}


// *************************************************************  ArrowCreator

ArrowTrigger::ArrowCreator::ArrowCreator(double x, double y, double vx, double vy)
    : m_x(x), m_y(y), m_vx(vx), m_vy(vy) {

}

void ArrowTrigger::ArrowCreator::Create(Game& game) {
    EntityFactoryPtr factory = Engine::Get().GetEntityFactory();
    EntityPtr entity = factory->CreateEntity(ET::Arrow, m_x, m_y);
    entity->SetVelocity(m_vx, m_vy);

    if (game.CanAddEntity(entity)) {
        game.AddEntity(entity);
    }
}
