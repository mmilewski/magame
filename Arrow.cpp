#include "Misc.h"

ArrowTrigger::ArrowTrigger(double x, double y, Orientation orientation)
    : Entity(x, y, 0, 0),
      m_seconds_since_last_shot(999.0),
      m_cooldown(3.0),
      m_orientation(orientation) {
}

void Arrow::Update(double dt, LevelPtr level) {
    // wylicz pozycję w poziomie (oś OX).
    double next_x = GetNextXPosition(dt);
    if (next_x < m_x && m_can_go_left) {
        m_x = next_x;
    } else if (next_x > m_x && m_can_go_right) {
        m_x = next_x;
    }

    CheckCollisionsWithLevel(dt, level);    
    ES::EntityState state = SetStateFromVelocity(GetXVelocity());
    if (state == ES::Stand) {
        const double eps = 0.0001;
        if (m_last_known_state == ES::GoLeft) {
            SetXVelocity(-eps);
        } else if (m_last_known_state == ES::GoRight) {
            SetXVelocity(eps);
        }
        SetStateFromVelocity(GetXVelocity());
    } else {
        m_last_known_state = state;
    }
    UpdateSpriteFromState(dt);
}

void Arrow::CheckCollisionsWithLevel(double dt, LevelPtr level) {
    if (IsAnyFieldOnLeft(dt, level)) {
        StopMovement();
    }

    if (IsAnyFieldOnRight(dt, level)) {
        StopMovement();
    }
}
