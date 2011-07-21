#include "StdAfx.h"

#include "Game.h"
#include "Engine.h"
#include "EntityFactory.h"
#include "Text.h"
#include "HallOfFame.h"
#include "ScoreSubmit.h"
#include "LevelChoiceScreen.h"
#include "Creator.h"


Game::~Game() {
    Engine::Get().GetSound()->HaltMusic();
}

void Game::ProcessEvents(const SDL_Event& event) {
    if (IsDone()) {
        return;
    }

    // przyjrzyj zdarzenia
    if (event.type == SDL_QUIT) {
        SetDone();
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        m_next_app_state = m_level_choice_screen;
        if (m_next_app_state) {
            m_next_app_state->Init();
        }
        SetDone();
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d) {
        m_player->Run();
    } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_d) {
        m_player->StopRunning();
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s) {
        if (m_player->CanShoot()) {
            m_player->FireBullet();
        }
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
        m_player->Jump();
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) {
        m_player->GoLeft();
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {
        m_player->GoRight();
    } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LEFT) {
        m_player->StopLeft();
    } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RIGHT) {
        m_player->StopRight();
    }
}

void Game::Start() {
    Engine::Get().GetSound()->PlayMusic("game");
}

void Game::Init() {
    Engine& engine = Engine::Get();

    if (!m_level) {
        // ładowanie poziomu i sprite'ów planszy
        m_level.reset(new Level());
        m_level->LoadFromFile("data/" + m_level_name + ".lvl");
        if (!m_level->GetLoaded()) {
            m_level_name = "1";
            m_level->LoadFromFile("data/" + m_level_name + ".lvl");
        }
        // załaduj jednostki do poziomu
        m_level->LoadEntitiesFromFile("data/" + m_level_name + ".ents");
    }
    m_entities_to_create = m_level->GetAllEntitiesToCreate();

    m_level_view.StoreSprite(FT::PlatformTopLeft,        Sprite::GetByName("PlatformTopLeft"));
    m_level_view.StoreSprite(FT::PlatformLeft,           Sprite::GetByName("PlatformLeft"));
    m_level_view.StoreSprite(FT::PlatformMid,            Sprite::GetByName("PlatformMid"));
    m_level_view.StoreSprite(FT::PlatformTop,            Sprite::GetByName("PlatformTop"));
    m_level_view.StoreSprite(FT::PlatformLeftTopRight,   Sprite::GetByName("PlatformLeftTopRight"));
    m_level_view.StoreSprite(FT::PlatformLeftRight,      Sprite::GetByName("PlatformLeftRight"));
    m_level_view.StoreSprite(FT::PlatformTopRight,       Sprite::GetByName("PlatformTopRight"));
    m_level_view.StoreSprite(FT::PlatformRight,          Sprite::GetByName("PlatformRight"));

    m_level_view.StoreSprite(FT::EndOfLevel,             Sprite::GetByName("EndOfLevel"));

    m_level_view.StoreSprite(FT::NcPlatformTopLeft,      Sprite::GetByName("NcPlatformTopLeft"));
    m_level_view.StoreSprite(FT::NcPlatformLeft,         Sprite::GetByName("NcPlatformLeft"));
    m_level_view.StoreSprite(FT::NcPlatformMid,          Sprite::GetByName("NcPlatformMid"));
    m_level_view.StoreSprite(FT::NcPlatformTop,          Sprite::GetByName("NcPlatformTop"));
    m_level_view.StoreSprite(FT::NcPlatformLeftTopRight, Sprite::GetByName("NcPlatformLeftTopRight"));
    m_level_view.StoreSprite(FT::NcPlatformLeftRight,    Sprite::GetByName("NcPlatformLeftRight"));
    m_level_view.StoreSprite(FT::NcPlatformTopRight,     Sprite::GetByName("NcPlatformTopRight"));
    m_level_view.StoreSprite(FT::NcPlatformRight,        Sprite::GetByName("NcPlatformRight"));


    // utwórz postać gracza
    const LevelEntityData player_data = m_level->GetPlayerData();
    if (!m_player) {
        if (player_data.name == "player") {
            m_player.reset(new Player(player_data.x, player_data.y, m_level->GetWidth(), 
                                      3, 0));
            m_player->SetSprites(SpritePtr(new Sprite(engine.GetSpriteConfig()->Get("player_left"))),
                                 SpritePtr(new Sprite(engine.GetSpriteConfig()->Get("player_right"))),
                                 SpritePtr(new Sprite(engine.GetSpriteConfig()->Get("player_stop"))) );
        } else {
            std::cerr << "brak informacji o postaci gracza w pliku z poziomem" << std::endl;
        }
    }
    else {
        m_stored_player_pos_x = 9;
        m_player->NewLevelReset(m_level);

    }
}

void Game::CheckPlayerEntitiesCollisions(double dt) {
    // poziomy i pionowy aabb gracza w następnym 'kroku'
    Aabb player_box_x = m_player->GetNextHorizontalAabb(dt);
    Aabb player_box_y = m_player->GetNextVerticalAabb(dt);

    for (std::vector<EntityPtr>::iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
        EntityPtr entity = *it;
        const ET::EntityType entity_type = entity->GetType();

        const int score_for_bonus = 40;   // punkty za wzięcie bonusu
        const int score_for_orb = 130;    // punkty za wzięcie kuli z punktami

        if (entity_type == ET::PlayerBullet) {
            // postać nie koliduje ze swoimi pociskami
            continue;
        } else if (entity_type == ET::SingleShot) {
            // gracz wziął bonus "zwykłe strzelanie"
            if (m_player->GetAabb().Collides(entity->GetAabb())) {
                m_player->AddScores(score_for_bonus);
                m_player->EnableShooting();
                entity->SetIsDead(true);
            }
            continue;
        } else if (entity_type == ET::TwinShot) {
            // gracz wziął bonus "podwójne strzelanie"
            if (m_player->GetAabb().Collides(entity->GetAabb())) {
                m_player->AddScores(score_for_bonus);
                m_player->EnableTwinShot();
                entity->SetIsDead(true);
            }
            continue;
        } else if (entity_type == ET::HigherJump) {
            // gracz wziął bonus "wyższe skakanie"
            if (m_player->GetAabb().Collides(entity->GetAabb())) {
                m_player->AddScores(score_for_bonus);
                m_player->IncreseJumpHeightBonus(3);
                entity->SetIsDead(true);
            }
            continue;
        } else if (entity_type == ET::Orb) {
            // gracz wziął kulę (oznaczającą dodatkowe punkty)
            if (m_player->GetAabb().Collides(entity->GetAabb())) {
                m_player->AddScores(score_for_orb);
                entity->SetIsDead(true);
            }
            continue;
        }

        // nieśmiertelna postać nie koliduje z innymi jednostkami,
        // ale może zbierać np. upgrade'y (patrz wyżej)
        if (m_player->IsImmortal()) {
            continue;
        }

        entity->SetDefaultMovement();
        Aabb entity_box = entity->GetAabb();

        // sprawdź czy wystąpiła kolizja. Jeżeli tak to gracz, zdecyduje o losie
        // swoim i jednostki. Zauważmy, że jeżeli wystąpi kolizja poniżej gracza
        // (naskoczenie na jednostkę), to pozostałe nie będą sprawdzane.
        if (player_box_y.IsOver(entity_box)) {
            // naskoczenie na jednostkę
            m_player->CollisionUnderPlayer(entity);
            player_box_y = m_player->GetNextVerticalAabb(dt);
        }
        else if (player_box_x.IsOnLeftOf(entity_box)) {
            m_player->ForbidGoingRight();
            m_player->CollisionOnRight(entity);
            player_box_x = m_player->GetNextHorizontalAabb(dt);
        }
        else if (player_box_x.IsOnRightOf(entity_box)) {
            m_player->ForbidGoingLeft();
            m_player->CollisionOnLeft(entity);
            player_box_x = m_player->GetNextHorizontalAabb(dt);
        }
        else if (player_box_y.IsUnder(entity_box)) {
            m_player->Fall();
            m_player->CollisionOverPlayer(entity);
            player_box_y = m_player->GetNextVerticalAabb(dt);
        }
    }
}

void Game::CheckCollisionOfOnePair(EntityPtr fst_entity, ET::EntityType fst_type,
                                   EntityPtr snd_entity, ET::EntityType snd_type,
                                   double dt) {
    if (fst_entity->GetNextAabb(dt).Collides(snd_entity->GetNextAabb(dt)) == false) {
        return;
    }

    // makro SWAP_IF pozwala zamienić znaczenie argumentów fst_* z snd_*
    // w ten sposób, że sprawdzając kolizję otrzymujemy wskażniki zawsze
    // w dobrej kolejności, tzn. tak, aby każdą parę obsługiwać jeden raz
#define SWAP_IF( type_a, type_b )  \
    if (fst_type == type_a && snd_type == type_b) {  \
        std::swap(fst_entity, snd_entity);   \
        std::swap(fst_type, snd_type);   \
    }

    SWAP_IF( ET::PlayerBullet, ET::Mush );

    // w tym miejscu wiemy, że jeżeli nastąpiła kolizja Mush z PlayerBullet,
    // to jednostka Mush będzie pod fst_entity a PlayerBullet pod snd_entity

    if (fst_type == ET::Mush && snd_type == ET::PlayerBullet) {
        snd_entity->SetIsDead(true);
        m_player->AddScores(fst_entity->GetScoresWhenKilled());
        fst_entity->KilledWithBullet();
    }

    if (fst_type == ET::Mush && snd_type == ET::Mush) {
        fst_entity->NegateXVelocity();
        snd_entity->NegateXVelocity();
    }

#undef SWAP_IF
}

void Game::CheckEntityEntityCollisions(double dt) {
    // sprawdzenie każdej pary (każdej jednokrotnie) - O(n^2)
    std::vector<EntityPtr>::iterator outer, inner;
    EntityPtr out_entity, inn_entity;
    ET::EntityType out_type, inn_type;
    for (outer = m_entities.begin(); outer != m_entities.end(); ++outer) {
        out_entity = *outer;
        out_type = out_entity->GetType();
        if (out_entity->IsDead()) {
            continue;
        }
        inner = outer;
        ++inner;
        for (; inner != m_entities.end(); ++inner) {
            inn_entity = *inner;
            inn_type = inn_entity->GetType();
            if (inn_entity->IsDead()) {
                continue;
            }
            CheckCollisionOfOnePair(inn_entity, inn_type, out_entity, out_type, dt);
        }
    }
}

void Game::ExecuteCreators() {
    std::list<CreatorPtr> creators;

    // pobierz kreatory od gracza
    std::list<CreatorPtr> plr_c = m_player->GetCreators();
    creators.splice(creators.end(), plr_c);
    m_player->DropAllCreators();

    // pobierz kreatory z jednostek
    for (std::vector<EntityPtr>::iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
        EntityPtr e = *it;
        if (!e->IsDead()) {
            std::list<CreatorPtr> ent_c = e->GetCreators();
            creators.splice(creators.end(), ent_c);
            e->DropAllCreators();
        }
    }

    // uruchom wszystkie kreatory
    for (std::list<CreatorPtr>::iterator it = creators.begin(); it != creators.end(); ++it) {
        (*it)->Create(*this);
    }
}

void Game::SweepAndAddEntities(double /* dt */) {
    // oznacz jednostki, które są za lewą krawędzią ekranu jako martwe
    const double distance_of_deletion = Engine::Get().GetRenderer()->GetHorizontalTilesOnScreenCount();
    for (std::vector<EntityPtr>::iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
        EntityPtr e = *it;
        if (e->GetX() + distance_of_deletion < m_player->GetX()) {
            e->SetIsDead(true);
        }
    }

    // usuń martwe jednostki - O(n^2). Można w O(n), ale trzeba napisać 
    // funktor - google(erase remove idiom).
    for (size_t i = 0; i < m_entities.size(); ++i) {
        if (m_entities.at(i)->IsDead()) {
            for (size_t c = i; c < m_entities.size() - 1; ++c) {
                m_entities.at(c) = m_entities.at(c + 1);
            }
            m_entities.resize(m_entities.size() - 1);
        }
    }

    // dodaj kolejne jednostki z listy do gry
    const double distance_of_creation = Engine::Get().GetRenderer()->GetHorizontalTilesOnScreenCount();
    while (m_entities_to_create.empty() == false) {
        if (m_entities_to_create.front().x - m_player->GetX() < distance_of_creation) {
            LevelEntityData data = m_entities_to_create.front();
            m_entities_to_create.pop_front();
            EntityPtr e = Engine::Get().GetEntityFactory()->CreateEntity(data.name, data.x, data.y);
            m_entities.push_back(e);
        } else {
            break;
        }
    }
}

void Game::BindLevelChoiceScreen(const boost::shared_ptr<LevelChoiceScreen>& screen) {
    m_level_choice_screen = screen;
}

bool Game::Update(double dt) {
    // czy gracz zakończył aktualny poziom?
    if (m_player->HasCompletedCurrentLevel()) {
        if (m_level_choice_screen) {
            m_level_choice_screen->SetPlayer(m_player);
            m_next_app_state = m_level_choice_screen;
        } else {
            m_next_app_state.reset();
        }

        SetDone();
        return IsDone();
    }
    else if (m_player->GetLifesCount() < 1) {
        size_t score = m_player->GetScores();
        if (score != 0) {
            m_next_app_state.reset(new ScoreSubmit(score));
        }
        else {
            m_next_app_state.reset(new HallOfFame());
        }
        SetDone();
    }

    // zbierz nowe obiekty z już istniejących
    ExecuteCreators();

    // ustaw domyślny ruch graczowi
    m_player->SetDefaultMovement();

    // sprawdź kolizje gracz-jednostka oraz jednostka-jednostka
    CheckPlayerEntitiesCollisions(dt);
    CheckEntityEntityCollisions(dt);

    // uaktualnij obiekt reprezentujący gracza
    m_player->Update(dt, m_level);

    // uaktualnij stan jednostek
    for (std::vector<EntityPtr>::iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
        EntityPtr e = *it;
        if (e->IsAlive()) {
            e->Update(dt, m_level);
        }
    }

    // usuń niepotrzebne jednostki i dodaj nowe
    SweepAndAddEntities(dt);

    // zaaktualizuj stan mapy kaflowej (np. animację kafli)
    m_level_view.Update(dt);

    return !IsDone();
}

void Game::Draw() {
    if (IsClearBeforeDraw()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
    }

    // narysuj mapę
    if (m_player->MoveMap()) {
        m_stored_player_pos_x = m_player->GetX();
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    {
        glPushAttrib(GL_COLOR_BUFFER_BIT);
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            double player_x = -(m_stored_player_pos_x * Engine::Get().GetRenderer()->GetTileWidth() - 0.45);
            glTranslated(player_x, 0, 0);
            glMatrixMode(GL_MODELVIEW);
    
            m_level_view.SetLevel(m_level, m_stored_player_pos_x);
            m_level_view.Draw(m_stored_player_pos_x);

            // narysuj postać gracza
            m_player->Draw();

            // narysuj pozostałe obiekty
            for (std::vector<EntityPtr>::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
                const EntityPtr e = *it;
                if (e->IsAlive()) {
                    e->Draw();
                }
            }
        }
        glPopAttrib();
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    if (IsSwapAfterDraw()) {
        SDL_GL_SwapBuffers();
    }
}
