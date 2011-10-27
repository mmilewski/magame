#include "Misc.h"

void Arrow::Update(double dt, LevelPtr level) {
    CalculateNextXPosition(dt);
    CheckCollisionsWithLevel(dt, level);
    RememberLastKnownState();
    SetStateFromVelocity(GetXVelocity());
    UpdateSpriteFromState(dt);
}

void Arrow::RememberLastKnownState() {
    // zapamiętaj stan inny niż ES::Stand
    ES::EntityState state = SetStateFromVelocity(GetXVelocity());
    if (state == ES::Stand) {
        const double eps = 0.0001;
        if (m_last_known_state == ES::GoLeft) {
            SetXVelocity(-eps);
        } else if (m_last_known_state == ES::GoRight) {
            SetXVelocity(eps);
        }
    } else {
        m_last_known_state = state;
    }
}

void Arrow::CheckCollisionsWithLevel(double dt, LevelPtr level) {
    if (IsAnyFieldOnLeft(dt, level)) {
        StopMovement();
    }

    if (IsAnyFieldOnRight(dt, level)) {
        StopMovement();
    }
}
