#ifndef MAGAME_PLAYER_H_INCLUDED
#define MAGAME_PLAYER_H_INCLUDED
#include "StdAfx.h"

#include "Entity.h"

class Player : public Entity {
public:
    explicit Player(double x, double y, size_t level_width, size_t lifes_count, size_t score);

    void Update(double dt, LevelPtr level);
    void Draw() const;
    void CheckCollisionsWithLevel(double dt, LevelPtr level);
    ET::EntityType GetType() const {
        throw std::logic_error("Gracz nie jest jednostką");
    }

    virtual void GoLeft()       { m_vx -= GetDefaultXVelocity(); m_state=PS::GoLeft; }
    virtual void GoRight()      { m_vx += GetDefaultXVelocity(); m_state=PS::GoRight; }
    virtual void StopLeft()     { m_vx += GetDefaultXVelocity(); TurnLeft(); }
    virtual void StopRight()    { m_vx -= GetDefaultXVelocity(); TurnRight(); }
    virtual void StopMovement() { m_vx = 0; m_state = PS::Stand; }
    void TurnLeft()             { m_state=PS::TurnLeft; }
    void TurnRight()            { m_state=PS::TurnRight; }
    void AllowToJump()              { m_jump_allowed = true; }
    void ForbidToJump()             { m_jump_allowed = false; }
    void Run()                      { m_running_factor = 2.0; }
    void StopRunning()              { m_running_factor = 1.0; }
    double GetRunningFactor() const { return m_running_factor; }
    void Jump(double extra_y_velocity=0);

    void SetDefaultMovement()  {
        m_is_on_ground = m_jump_allowed = false;
        m_can_go_right = m_can_go_left = true;
    }
    void PlayerOnGround() {
        m_is_on_ground = m_jump_allowed = true;
        SetYVelocity(0);
    }

    double GetNextXPosition(double dt) const { return GetX() + GetXVelocity() * dt * GetRunningFactor(); }

    // prostokąt otaczający jednostkę bez uwzględniania jej pozycji
    Aabb GetBasicAabb() const { return Aabb(.2, 0, .75, .9);  }

    // obsługa kolizji z każdej strony
    void CollisionOnRight(EntityPtr entity);
    void CollisionOnLeft(EntityPtr entity);
    void CollisionOverPlayer(EntityPtr entity);
    void CollisionUnderPlayer(EntityPtr entity);

    void AddScores(int scores) { m_total_scores += scores; }
    int  GetScores() const     { return m_total_scores - m_bullet_pay; }

    // wystrzel pocisk
    void FireBullet();

    // zwraca liczbę żyć bohatera
    int GetLifesCount() const { return m_lifes; }

    // gracz stracił życie - reakcja na zdarzenie
    void LooseLife();

    // akcja wywoływana. kiedy gracz zakończy poziom
    // (np. odegranie fanfarów, wyświetlenia napisu, ...
    void LevelCompleted();

    bool HasCompletedCurrentLevel() const {
        return m_is_level_completed;
    }

    bool IsImmortal() const { return m_is_immortal; }

    void EnableShooting()          { m_shooting_enabled = true; }
    void DisableShooting()         { m_shooting_enabled = false; }
    bool CanShoot() const          { return m_shooting_enabled; }
    bool CannotShoot() const       { return !CanShoot(); }

    void EnableTwinShot()          { EnableShooting(); m_twin_shot_enabled = true; }
    void DisableTwinShot()         { m_twin_shot_enabled = false; }
    bool IsTwinShotEnabled() const { return m_twin_shot_enabled; }

    void IncreaseJumpHeightBonus(int by)  { m_jump_height_bonus += by; }
    void DropJumpHeightBonus()            { m_jump_height_bonus = 0; }

    bool MoveMap() const {
        const size_t screen_tiles_count = Engine::Get().GetRenderer()->GetHorizontalTilesOnScreenCount();
        return 
            GetX() >= m_max_x_pos
            && 
            GetX() < (m_level_width - 1) - screen_tiles_count/2;
    }

    void NewLevelReset(LevelPtr level) {
        LevelEntityData player_data = level->GetPlayerData();
        SetDefaultMovement();
        SetPosition(player_data.x, player_data.y);
        SetVelocity(0, 0);
        m_state = PS::Stand;
        m_running_factor = 1;

        m_max_x_pos = 9;
        m_level_width = level->GetWidth();
        m_is_level_completed = false;
    }
    
    bool ShouldBeRespawned() const   { return m_should_be_respawned; }

    void RespawnFrom(boost::shared_ptr<Player> saved_player);

private:
    void PayForBullet() {
        m_bullet_pay += 15;
    }

private:
    enum { DefaultXVelocity = 4, DefaultYVelocity = 20,
           DefaultXAcceleration = 0, DefaultYAcceleration = -60,
           DefaultLifesCount = 3 };
    
    PS::PlayerState m_state;  // stan, w którym znajduje się postać
    double m_running_factor;  // współczynnik biegania. >1.0 => biegnie, <1.0 => spowolnienie
    bool m_jump_allowed;      // czy gracz może skakać (np. jest na podłożu)
    size_t m_level_width;     // szerokość poziomu (w kaflach)

    int m_total_scores;       // łączne zdobyte punkty
    int m_bullet_pay;         // opłata za wystrzelenie pocisków

    bool m_is_immortal;       // czy jest nieśmiertelny
    double m_immortal_duration;  // czas przez który postać już jest nieśmiertelna
    int m_lifes;                 // liczba żyć posiadanych przez postać

    bool m_shooting_enabled;     // czy gracz może strzelać
    bool m_twin_shot_enabled;    // czy upgrade twin shot jest dostępny
    int m_jump_height_bonus;     // bonus do prędkości pionowej podczas skoku

    bool m_is_level_completed;   // czy aktualny poziom został zakończony

    double m_max_x_pos;
    
    bool m_should_be_respawned;  // czy gracz powinien zostać przywrócony na zapisaną pozycję
};

typedef boost::shared_ptr<Player> PlayerPtr;

#endif /* MAGAME_PLAYER_H_INCLUDED */
