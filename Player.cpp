#include "StdAfx.h"

#include "PlayerBulletCreator.h"
#include "Utils.h"
#include "Engine.h"
#include "Player.h"
#include "Text.h"

Player::Player(double x, double y, size_t level_width, size_t lifes = DefaultLifesCount, size_t score = 0)
    : Entity(x, y, DefaultXVelocity, DefaultYVelocity, DefaultXAcceleration, DefaultYAcceleration),
      m_state(PS::Stand),
      m_running_factor(1.0),
      m_jump_allowed(true),
      m_level_width(level_width),
      m_total_scores(score),
      m_is_immortal(false),
      m_lifes(lifes),
      m_twin_shot_enabled(false),
      m_is_level_completed(false),
      m_max_x_pos(std::max(x, 9.0)) {
    SetDefaultMovement();
}

void Player::Jump(double y_velocity) {
    // wykonaj skok o ile jest taka możliwość
    if (m_jump_allowed) {
        m_jump_allowed = false;
        m_is_on_ground = false;
        // początkowa prędkość i przyspieszenie
        SetYVelocity(y_velocity);
        SetYAcceleration(DefaultYAcceleration);
        Engine::Get().GetSound()->PlaySfx("jump");
    }
}

void Player::CheckCollisionsWithLevel(double dt, LevelPtr level) {
    size_t x_tile, y_tile;
    GetCurrentTile(&x_tile, &y_tile);

    // czy gracz wszedł w portal i należy mu się wygrana :)
    for (int i=-1; i<2; i++) {
        for (int j=-1; j<2; j++) {
            if (level->GetFieldAabb(x_tile + i, y_tile + j).Collides(GetNextAabb(dt))
                && level->Field(x_tile + i, y_tile + j) == FT::EndOfLevel) {
                LevelCompleted();
            }
        }
    }

    // czy postać koliduje z czymś od góry
    if (IsAnyFieldAboveMe(dt, level)) {
        Fall();
    }

    // czy postać koliduje z czymś od dołu
    if (IsAnyFieldBelowMe(dt, level)) {
        PlayerOnGround();
    }

    // czy postać koliduje z czymś po lewej stronie
    if (IsAnyFieldOnLeft(dt, level)) {
        ForbidGoingLeft();
    }

    // czy postać koliduje z czymś po prawej stronie
    if (IsAnyFieldOnRight(dt, level)) {
        ForbidGoingRight();
    }
}

void Player::Update(double dt, LevelPtr level) {
    CheckCollisionsWithLevel(dt, level);

    // wylicz nową prędkość oraz połóżenie na osi OY
    if (!m_is_on_ground) {
        m_y = GetNextYPosition(dt);
        m_vy += m_ay * dt;
        m_vy = std::max(m_vy, -22.0);    // maksymalna prędkość w dół
    }

    // jeżeli poniżej pierwszego kafla, to odbieramy życie.
    if (m_y < .5) {
        LooseLife();
    }

    // uaktualnij informacje o nieśmiertelności
    const double immortality_time = 3; // 3 sekundy
    if (m_immortal_duration > immortality_time) {
        m_is_immortal = false;
        m_immortal_duration = 0;
    } else {
        m_immortal_duration += dt;
    }

    // wylicz pozycję gracza w poziomie (oś OX).
    double next_x = GetNextXPosition(dt);
    if (next_x < m_x && m_can_go_left) {
        m_x = next_x;
    } else if (next_x > m_x && m_can_go_right) {
        m_x = next_x;
    }

    // nie można wyjść poza mapę
    if (m_x < 0) {
        m_x = 0; // nie można wyjść za początek mapy
    } else if (m_x > m_level_width - 1) {
        m_x = m_level_width - 1; // nie można wyjść za ostatni kafel mapy
    }

    // nie możemy cofnąć się do miejsc, których już nie widzimy
    if (m_x > m_max_x_pos) {
        m_max_x_pos = m_x;
    }
    const size_t half_screen_tiles_count = (Engine::Get().GetRenderer()->GetHorizontalTilesOnScreenCount()-1)/2;
    if (m_x < m_max_x_pos - half_screen_tiles_count) {
        m_x = m_max_x_pos - half_screen_tiles_count; // można się wrócić tylko do tych części mapy, które się już widziało
    } 
    if (m_x > m_level_width - half_screen_tiles_count - 2) {
        m_max_x_pos = m_level_width - half_screen_tiles_count - 2;
    }

    // ustal stan ruchu gracza na podstawie prędkości
    if (fabs(m_vx) < 0.001 && (m_state != PS::TurnLeft || m_state != PS::TurnRight)) {
        // nie zmieniamy stanu
    } else if (m_vx > 0.0) {
        m_state = PS::GoRight;
    } else {
        m_state = PS::GoLeft;
    }

    // uaktualnij animację
    switch (m_state) {
    case PS::Stand:
        m_stop->Update(dt);
        break;
    case PS::GoLeft:
        m_left->Update(dt);
        break;
    case PS::GoRight:
        m_right->Update(dt);
        break;
    case PS::TurnLeft:
    case PS::TurnRight:
        // animacja ma jedną klatkę, więc niczego nie aktualizujemy
        break;
    }
}

void Player::Draw() const {
    // wypisz informację o liczbie punktów zdobytych przez gracza
    Text scores_text;
    scores_text.SetSize(0.05, 0.05);
    scores_text.DrawText("Punkty " + IntToStr(GetScores()), .01, .9);
    scores_text.DrawText("Zycia " + IntToStr(GetLifesCount()), .65, .9);

    // jeżeli bohater jest nieśmiertelny, to miga (rysuj - nie rysuj)
    if (IsImmortal() && int(m_immortal_duration * 10) % 2 == 1) {
        return;
    }

    // pobierz szerokość i wysokość kafla na ekranie
    RendererPtr renderer = Engine::Get().GetRenderer();
    const double tile_width = renderer->GetTileWidth();
    const double tile_height = renderer->GetTileHeight();

    // wylicz pozycję gracza na ekranie
    const double pos_x = m_x * tile_width;
    const double pos_y = m_y * tile_height;

    switch (m_state) {
    case PS::Stand:
        m_stop->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case PS::GoLeft:
        m_left->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case PS::GoRight:
        m_right->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case PS::TurnLeft:
        m_left->SetCurrentFrame(0);
        m_left->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case PS::TurnRight:
        m_right->SetCurrentFrame(0);
        m_right->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    }
}

void Player::CollisionOnRight(EntityPtr /* entity */) {
    LooseLife();
}

void Player::CollisionOnLeft(EntityPtr /* entity */) {
    LooseLife();
}

void Player::CollisionOverPlayer(EntityPtr /* entity */) {
    LooseLife();
}

void Player::CollisionUnderPlayer(EntityPtr entity) {
    AllowToJump();
    Jump(GetDefaultYVelocity() + 6);
    AddScores(entity->GetScoresWhenKilled() * 2);
    entity->KilledByPlayer();
}

void Player::FireBullet() {
    // GetX() oraz GetY() zwracają położenie lewego dolnego
    // narożnika postaci. W zależności od prędkości i stanu
    // postaci dodajemy pocisk po odpowiedniej stronie.

    double x, xvel;
    const double eps = 0.0001;  // jakakolwiek prędkość
    if (m_state == PS::TurnLeft) {
        x = GetX() - .3;
        xvel = -eps;
    } else if (m_state == PS::TurnRight) {
        x = GetX() + .7;
        xvel = eps;
    } else {
        x = GetXVelocity() < 0 ? GetX() - .3 : GetX() + .7;
        xvel = GetXVelocity();
    }

    const double y = GetY() + .5;
    AddCreator(CreatorPtr(new PlayerBulletCreator(x, y, xvel, 0)));
    if (IsTwinShotEnabled()) {
        double twin_xvel = xvel + 2 * (xvel/std::fabs(xvel)); // zwiększamy prędkość w zależności od zwrotu wektora prędkości
        AddCreator(CreatorPtr(new PlayerBulletCreator(x, y + .5, twin_xvel, 0)));
    }
}

void Player::LooseLife() {
    // utrata jednego życia
    m_lifes--;

    // nieśmiertelność przez pewien czas
    m_is_immortal = true;
    m_immortal_duration = 0;

    // ustaw graczowi nową pozycję (respawn)
    SetPosition(2, 2);
}

void Player::LevelCompleted() {
    m_is_level_completed = true;
}
