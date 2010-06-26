#ifndef __PLAYER_BULLET_CREATOR_H_INCLUDED__
#define __PLAYER_BULLET_CREATOR_H_INCLUDED__
#include "StdAfx.h"

#include "Creator.h"
#include "PlayerBulletEntity.h"
#include "Game.h"


class PlayerBulletCreator : public Creator {
public:
    explicit PlayerBulletCreator(double x, double y, double vx, double vy)
        : m_x(x), m_y(y), m_vx(vx), m_vy(vy) {
    }

    void Create(Game& game) {
        // stwórz jednostkę i nadaj jej odpowiednią prędkość (zgodnie ze zwrotem postaci gracza)
        EntityPtr entity = Engine::Get().GetEntityFactory()->CreateEntity(ET::PlayerBullet, m_x, m_y);
        double x_vel = m_vx < 0 ? -entity->GetDefaultXVelocity() : entity->GetDefaultXVelocity();
        double y_vel = entity->GetDefaultYVelocity();
//        std::cout << "[PlayerBulletCreator] " << m_vx << "  " << x_vel << " " << y_vel << std::endl;
        entity->SetVelocity(x_vel + m_vx, y_vel + m_vy);

        // dodaj pocisk do gry oraz odegraj dźwięk 'laser'
        if (game.CanAddEntity(entity)) {
            game.AddEntity(entity);
            Engine::Get().GetSound()->PlaySfx("laser");
        }
    }

private:
    double m_x, m_y;    // położenie
    double m_vx, m_vy;  // prędkość
};

#endif
