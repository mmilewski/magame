#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdexcept>  // logic_error
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
    void Jump(double y_velocity = DefaultYVelocity);
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
    Aabb GetBasicAabb() const { return Aabb(.1, 0, .7, .9);  }

    // obsługa kolizji z każdej strony
    void CollisionOnRight(EntityPtr entity);
    void CollisionOnLeft(EntityPtr entity);
    void CollisionOverPlayer(EntityPtr entity);
    void CollisionUnderPlayer(EntityPtr entity);

    void AddScores(int scores) { m_total_scores += scores; }
    int  GetScores() const     { return m_total_scores; }

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

    void EnableTwinShot()          { m_twin_shot_enabled = true; }
    void DisableTwinShot()         { m_twin_shot_enabled = false; }
    bool IsTwinShotEnabled() const { return m_twin_shot_enabled; }

    bool MoveMap() {
        const size_t screen_tiles_count = Engine::Get().GetRenderer()->GetHorizontalTilesOnScreenCount();
        const size_t half_screen_tiles_count = screen_tiles_count/2;
        return 
            GetX() >= m_max_x_pos
            && 
            GetX() < (m_level_width - 1) - half_screen_tiles_count;
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

private:
    enum { DefaultXVelocity = 4, DefaultYVelocity = 20,
           DefaultXAcceleration = 0, DefaultYAcceleration = -60,
           DefaultLifesCount = 3 };
    
    PS::PlayerState m_state;  // stan, w którym znajduje się postać
    double m_running_factor;  // współczynnik biegania. >1.0 => biegnie, <1.0 => spowolnienie
    bool m_jump_allowed;      // czy gracz może skakać (np. jest na podłożu)
    size_t m_level_width;     // szerokość poziomu (w kaflach)

    int m_total_scores;       // łączne zdobyte punkty
    bool m_is_immortal;       // czy jest nieśmiertelny
    double m_immortal_duration;  // czas przez który postać już jest nieśmiertelna
    int m_lifes;                 // liczba żyć posiadanych przez postać

    bool m_twin_shot_enabled;    // czy upgrade twin shot jest dostępny

    bool m_is_level_completed;   // czy aktualny poziom został zakończony

    double m_max_x_pos;
};

typedef boost::shared_ptr<Player> PlayerPtr;

#endif
