#ifndef __GAME_H_INCLUDED__
#define __GAME_H_INCLUDED__
#include "StdAfx.h"


#include "Utils.h"
#include "Player.h"
#include "Level.h"
#include "SpriteGrid.h"
#include "Entity.h"
#include "AppState.h"

class LevelChoiceScreen;
class Level;
typedef boost::shared_ptr<Level> LevelPtr;

class Game : public AppState, public boost::enable_shared_from_this<Game> {
public:
    explicit Game(const std::string& level_name, PlayerPtr player) 
        : m_player(player),
          m_stored_player_pos_x(9.0),
          m_level_name(level_name)  {
    }
    explicit Game(LevelPtr level, PlayerPtr player) 
        : m_player(player),
          m_level(level),
          m_stored_player_pos_x(9.0),
          m_level_name(level->GetName())  {
    }

    ~Game();

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

    void BindLevelChoiceScreen(const boost::shared_ptr<LevelChoiceScreen>& screen);

private:
    void SweepAndAddEntities(double dt);
    void CheckPlayerEntitiesCollisions(double dt);
    void CheckEntityEntityCollisions(double dt);
    void CheckCollisionOfOnePair(EntityPtr fst_entity, ET::EntityType fst_type,
                                 EntityPtr snd_entity, ET::EntityType snd_type, double dt);

    void SaveGame(PlayerPtr player);

    PlayerPtr m_player;
    std::vector<EntityPtr> m_entities;                // jednostki w grze
    std::list<LevelEntityData> m_entities_to_create;  // opisy jednostek do stworzenia

    LevelPtr m_level;
    SpriteGrid m_level_view;
    double m_stored_player_pos_x;

    std::string m_level_name;
    AppStatePtr m_next_app_state;
    

    boost::shared_ptr<LevelChoiceScreen> m_level_choice_screen;
};

typedef boost::shared_ptr<Game> GamePtr;

#endif
