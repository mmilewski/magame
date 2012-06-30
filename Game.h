#ifndef MAGAME_GAME_H_INCLUDED
#define MAGAME_GAME_H_INCLUDED
#include "StdAfx.h"

#include "common/Utils.h"
#include "entity/Player.h"
#include "Level.h"
#include "video/SpriteGrid.h"
#include "entity/Entity.h"
#include "AppState.h"

#include <LevelChoiceScreen.h>

class Level;
typedef boost::shared_ptr<Level> LevelPtr;

class Game : public AppState, public boost::enable_shared_from_this<Game> {
public:
    explicit Game(const std::string& level_name, PlayerPtr player) 
        : m_saved_player(),
          m_saved_stored_player_pos_x(1.0),
          m_should_load_when_active_again(false),
          m_player(player),
          m_entities(),
          m_entities_to_create(),
          m_level(),
          m_level_view(),
          m_stored_player_pos_x(9.0),
          m_level_name(level_name),
          m_next_app_state(),
          m_level_choice_screen() {
    }
    explicit Game(LevelPtr level, PlayerPtr player) 
        : m_saved_player(),
          m_saved_stored_player_pos_x(1.0),
          m_should_load_when_active_again(false),
          m_player(player),
          m_entities(),
          m_entities_to_create(),
          m_level(level),
          m_level_view(),
          m_stored_player_pos_x(9.0),
          m_level_name(level->GetName()),
          m_next_app_state(),
          m_level_choice_screen() {
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

    AppStatePtr NextAppState() const {
        return m_next_app_state;
    }

    void BindLevelChoiceScreen(const boost::shared_ptr<LevelChoiceScreen>& screen);

private:
    void SweepAndAddEntities(double dt);
    void HandleCollisionPlayerWithSolidEntity(PlayerPtr player, EntityPtr entity, double dt);
    void CheckPlayerEntitiesCollisions(double dt);
    void CheckEntityEntityCollisions(double dt);
    void CheckCollisionOfOnePair(EntityPtr fst_entity, ET::EntityType fst_type,
                                 EntityPtr snd_entity, ET::EntityType snd_type, double dt);

    void SaveGame(PlayerPtr player);
    void LoadGame();

    PlayerPtr m_saved_player;
    double m_saved_stored_player_pos_x;
    bool m_should_load_when_active_again;

    PlayerPtr m_player;
    std::vector<EntityPtr> m_entities;                // jednostki w grze
    std::list<LevelEntityData> m_entities_to_create;  // opisy jednostek do stworzenia

    LevelPtr m_level;
    SpriteGrid m_level_view;
    double m_stored_player_pos_x;

    std::string m_level_name;
    AppStatePtr m_next_app_state;
    
    LevelChoiceScreenPtr m_level_choice_screen;
};

typedef boost::shared_ptr<Game> GamePtr;

#endif
