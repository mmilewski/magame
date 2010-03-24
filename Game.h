#ifndef __GAME_H_INCLUDED__
#define __GAME_H_INCLUDED__
#include "StdAfx.h"

#include <boost/shared_ptr.hpp>
#include <SDL/SDL.h>
#include <vector>
#include <iostream>

#include "Utils.h"
#include "Player.h"
#include "Level.h"
#include "SpriteGrid.h"
#include "Entity.h"
#include "AppState.h"

class Game : public AppState {
public:
    explicit Game(const std::string& level_name, 
                  size_t player_lifes = 2, 
                  size_t player_total_score = 0) 
        : m_stored_player_pos_x(9.0),
          m_level_name(level_name),
          m_player_lifes(player_lifes),
          m_player_total_score(player_total_score) {

        SetNextLevelName();
    }

    void SetNextLevelName() {
        int n = StrToInt(m_level_name);
        m_next_level_name = IntToStr(n+1);
    }

    void Start();
    void Init();
    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    void ExecuteCreators();
    bool CanAddEntity(EntityPtr entity) const {
        size_t curr_tile_x, curr_tile_y;
        entity->GetCurrentTile(&curr_tile_x, &curr_tile_y);
        if (m_level->Field(curr_tile_x, curr_tile_y) != FT::None) {
            return false;
        }
        return true;
    }
    void AddEntity(EntityPtr entity) {
        m_entities.push_back(entity);
    }

    LevelPtr GetLevel() const { return m_level; }

    AppStatePtr NextAppState() const {
        return m_next_app_state;
    }

    
private:
    void SeepAndAddEntities(double dt);
    void CheckPlayerEntitiesCollisions(double dt);
    void CheckEntityEntityCollisions(double dt);
    void CheckCollisionOfOnePair(EntityPtr fst_entity, ET::EntityType fst_type,
                                 EntityPtr snd_entity, ET::EntityType snd_type, double dt);

    PlayerPtr m_player;
    std::vector<EntityPtr> m_entities;                // jednostki w grze
    std::list<LevelEntityData> m_entities_to_create;  // opisy jednostek do stworzenia

    LevelPtr m_level;
    SpriteGrid m_level_view;
    double m_stored_player_pos_x;

    std::string m_level_name;
    AppStatePtr m_next_app_state;

    size_t m_player_lifes;
    size_t m_player_total_score;
    
    std::string m_next_level_name;
};

typedef boost::shared_ptr<Game> GamePtr;

#endif
