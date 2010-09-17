#include "StdAfx.h"

#include "Entity.h"
#include "Engine.h"

Entity::Entity(double x, double y, double def_velocity_x, double def_velocity_y,
               double def_acceleration_x, double def_acceleration_y)
    : m_default_velocity_x(def_velocity_x),
      m_default_velocity_y(def_velocity_y),
      m_default_acceleration_x(def_acceleration_x),
      m_default_acceleration_y(def_acceleration_y),
      m_is_dead(false),
      m_state(ES::Stand),
      m_x(x),
      m_y(y),
      m_vx(0.0),
      m_vy(0),
      m_ax(def_acceleration_x),
      m_ay(def_acceleration_y),
      m_is_on_ground(true),
      m_can_go_left(true),
      m_can_go_right(true)
      {
    SetDefaultMovement();
}

void Entity::SetSprites(SpritePtr left, SpritePtr right, SpritePtr stop) {
    m_left = left;
    m_right = right;
    m_stop = stop;
}

bool Entity::IsAnyFieldBelowMe(double dt, LevelPtr level) const {
    size_t curr_x_tile, curr_y_tile;
    GetCurrentTile(&curr_x_tile, &curr_y_tile);
    for (int x = -1; x < 2; ++x) {
        if (level->Field(curr_x_tile + x, curr_y_tile + 1) == FT::None) {
            continue;
        }
        const Aabb field_aabb = level->GetFieldAabb(curr_x_tile + x, curr_y_tile + 1);
        if (GetNextVerticalAabb(dt).IsOver(field_aabb)) {
            return true;
        }
    }
    return false;
}

bool Entity::IsAnyFieldAboveMe(double dt, LevelPtr level) const {
    size_t curr_x_tile, curr_y_tile;
    GetCurrentTile(&curr_x_tile, &curr_y_tile);
    for (int x = -1; x < 2; ++x) {
        if (level->Field(curr_x_tile + x, curr_y_tile - 1) == FT::None) {
            continue;
        }
        const Aabb field_aabb = level->GetFieldAabb(curr_x_tile + x, curr_y_tile - 1);
        if (GetNextVerticalAabb(dt).IsUnder(field_aabb)) {
            return true;
        }
    }
    return false;
}

bool Entity::IsAnyFieldOnLeft(double dt, LevelPtr level) const {
    size_t curr_x_tile, curr_y_tile;
    GetCurrentTile(&curr_x_tile, &curr_y_tile);
    for (int y = -1; y < 2; ++y) {
        if (level->Field(curr_x_tile - 1, curr_y_tile + y) == FT::None) {
            continue;
        }
        const Aabb field_aabb = level->GetFieldAabb(curr_x_tile - 1, curr_y_tile + y);
        if (GetNextHorizontalAabb(dt).IsOnRightOf(field_aabb)) {
            return true;
        }
    }
    return false;
}

bool Entity::IsAnyFieldOnRight(double dt, LevelPtr level) const {
    size_t curr_x_tile, curr_y_tile;
    GetCurrentTile(&curr_x_tile, &curr_y_tile);
    for (int y = -1; y < 2; ++y) {
        if (level->Field(curr_x_tile + 1, curr_y_tile + y) == FT::None) {
            continue;
        }
        const Aabb field_aabb = level->GetFieldAabb(curr_x_tile + 1, curr_y_tile + y);
        if (GetNextHorizontalAabb(dt).IsOnLeftOf(field_aabb)) {
            return true;
        }
    }
    return false;
}

bool Entity::DoFieldsEndOnLeft(double dt, LevelPtr level) const {
    size_t curr_x_tile, curr_y_tile;
    GetCurrentTile(&curr_x_tile, &curr_y_tile);
    // czy pole o jeden w lewo i w doł jest puste (None)
    // oraz czy jednostka jest przy krawędzi (IsOnLeftOf)...
    if (level->Field(curr_x_tile - 1, curr_y_tile + 1) == FT::None
        && level->GetFieldAabb(curr_x_tile - 1, curr_y_tile).IsOnLeftOf(GetNextHorizontalAabb(dt))) {
        return true;
    }
    return false;
}

bool Entity::DoFieldsEndOnRight(double dt, LevelPtr level) const {
    size_t curr_x_tile, curr_y_tile;
    GetCurrentTile(&curr_x_tile, &curr_y_tile);
    if (level->Field(curr_x_tile + 1, curr_y_tile + 1) == FT::None
        && level->GetFieldAabb(curr_x_tile + 1, curr_y_tile).IsOnRightOf(GetNextHorizontalAabb(dt))) {
        return true;
    }
    return false;
}

void Entity::CheckCollisionsWithLevel(double dt, LevelPtr level) {
    // czy jednostka koliduje z czymś od góry
    if(IsAnyFieldAboveMe(dt, level)) {
        Fall();  // rozpocznij spadanie
    }

    // czy jednostka koliduje z czymś od dołu
    if(IsAnyFieldBelowMe(dt, level)) {
        EntityOnGround();   // zatrzymaj na podłożu
    }

    // czy jednostka koliduje z czymś po lewej stronie
    if(IsAnyFieldOnLeft(dt, level)) {
        NegateXVelocity();  // zawróć
    }

    // czy jednostka koliduje z czymś po prawej stronie
    if(IsAnyFieldOnRight(dt, level)) {
        NegateXVelocity();  // zawróć
    }
}

void Entity::Update(double dt, LevelPtr level) {
    // ustaw domyślny ruch i sprawdź czy co w świecie piszczy
    SetDefaultMovement();
    CheckCollisionsWithLevel(dt, level);

    // wylicz nową prędkość oraz połóżenie na osi OY
    if (!m_is_on_ground) {
        m_y = GetNextYPosition(dt);
        m_vy += m_ay * dt;
    }

    // jeżeli poniżej pierwszego kafla, to nie spadaj niżej.
    // Na razie ustalamy poziom na y=1, aby jednostka nie uciekała za ekran
    if (m_y < 0) {
        m_y = 0;
        EntityOnGround();
    }

    // wylicz pozycję gracza w poziomie (oś OX).
    double next_x = GetNextXPosition(dt);
    if (next_x < m_x && m_can_go_left) {
        m_x = next_x;
    } else if (next_x > m_x && m_can_go_right) {
        m_x = next_x;
    }

    // nie można wyjść poza mapę
    if (m_x < 1) {
        m_x = 1; // nie można wyjść za początek mapy
    }

    // ustal stan ruchu gracza na podstawie prędkości
    if (fabs(m_vx) < 0.00001) {
        m_state = ES::Stand;
        m_vx = 0;
    } else if (m_vx > 0.0) {
        m_state = ES::GoRight;
    } else {
        m_state = ES::GoLeft;
    }

    // uaktualnij animację
    switch (m_state) {
    case ES::Stand:
        m_stop->Update(dt);
        break;
    case ES::GoLeft:
        m_left->Update(dt);
        break;
    case ES::GoRight:
        m_right->Update(dt);
        break;
    }
}

void Entity::Draw() const {
    const double tile_width = Engine::Get().GetRenderer()->GetTileWidth();
    const double tile_height = Engine::Get().GetRenderer()->GetTileHeight();

    // wylicz pozycję gracza na ekranie
    const double pos_x = m_x * tile_width;
    const double pos_y = m_y * tile_height;

//    std::cout << "Entity Draw: " << pos_x << " " << pos_y << std::endl;
//    const size_t screen_tiles_count = Engine::Get().GetRenderer()->GetHorizontalTilesOnScreenCount();
//    const size_t half_screen_tiles_count = screen_tiles_count/2-1;
//    double pos_x = 0.45;
//    const double left_edge = m_max_x_pos - half_screen_tiles_count;
//    if (m_x >= left_edge && m_x < m_max_x_pos) {
//        // jeżeli gracz jest między ostatnio widocznym fragmentem a
//        // maksymalną pozycją na której do tej pory był, to porusza
//        // się swobodnie (nie jest przypięty do środka planszy)
//        pos_x = (m_x - left_edge) * tile_width;
//    }
//
//    if (m_x > m_level_width - half_screen_tiles_count - 2) {
//        // jeżeli gracz jest przy prawej krawędzi mapy (mapa nie
//        // powinna się poruszać)
//        pos_x = (m_x - screen_tiles_count) * tile_width;
//    }


//     std::cout << "[Entity::Draw] " << pos_x << " " << pos_y << std::endl;
    switch (m_state) {
    case ES::Stand:
        m_stop->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case ES::GoLeft:
        m_left->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case ES::GoRight:
        m_right->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    }

//    const size_t v_tiles_count = Engine::Get().GetRenderer()->GetVerticalTilesOnScreenCount();
//    const size_t curr_y_tile = v_tiles_count - GetAabb().GetMinY();
//    const size_t curr_x_tile = GetX() + .5;
//    for (int y = -1; y < 2; ++y) {
//        Engine::Get().GetRenderer()->DrawAabb(MM_level->GetFieldAabb(curr_x_tile + 1, curr_y_tile + y));
//        Engine::Get().GetRenderer()->DrawAabb(MM_level->GetFieldAabb(curr_x_tile + y, curr_y_tile + 1));
//    }



//    Engine::Get().GetRenderer()->DrawAabb(GetAabb());
}

void Entity::KilledByPlayer() {
    SetIsDead(true);
    // std::cout << "[Entity] Killed by player" << std::endl;
}

void Entity::KilledWithBullet() {
    SetIsDead(true);
    // std::cout << "[Entity] Killed by bullet" << std::endl;
}
