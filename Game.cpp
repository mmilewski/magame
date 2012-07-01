#include "StdAfx.h"

#include "Engine.h"
#include "video/Text.h"
#include "video/Sprite.h"
#include "entity/EntityFactory.h"
#include "entity/Creator.h"
#include "entity/SavePoint.h"
#include "entity/Misc.h"
#include "TransitionEffect.h"
#include "HallOfFame.h"
#include "ScoreSubmit.h"
#include "LevelChoiceScreen.h"
#include "Game.h"


Game::~Game() {
    Engine::Get().GetSound()->HaltMusic();
}

void Game::ProcessEvents(const SDL_Event& event) {
    if (IsDone()) {
        return;
    }

#define WAS_PRESSED(thekey) event.type == SDL_KEYDOWN && event.key.keysym.sym == thekey
#define WAS_RELEASED(thekey) event.type == SDL_KEYUP && event.key.keysym.sym == thekey

    // przyjrzyj zdarzenia
    if (event.type == SDL_QUIT) {
        SetDone();
        return;
    } else if (WAS_PRESSED(SDLK_ESCAPE)) {
        m_next_app_state = m_level_choice_screen;
        if (m_next_app_state) {
            m_next_app_state->Init();
        }
        SetDone();
        return;
    }

    if (WAS_PRESSED(SDLK_d)) {
        m_player->Run();
    }
    else if (WAS_RELEASED(SDLK_d)) {
        m_player->StopRunning();
    }
    else if (WAS_PRESSED(SDLK_s)) {
        if (m_player->CanShoot()) {
            m_player->FireBullet();
        }
    }

    if (WAS_PRESSED(SDLK_UP)) {
        m_player->Jump();
    }
    else if (WAS_PRESSED(SDLK_LEFT)) {
        m_player->GoLeft();
    }
    else if (WAS_PRESSED(SDLK_RIGHT)) {
        m_player->GoRight();
    }
    else if (WAS_RELEASED(SDLK_LEFT)) {
        m_player->StopLeft();
    }
    else if (WAS_RELEASED(SDLK_RIGHT)) {
        m_player->StopRight();
    }
#undef WAS_PRESSED
#undef WAS_RELEASED
}

void Game::Start() {
    Engine::Get().GetSound()->PlayMusic("game");
    if (m_should_load_when_active_again) {
        m_player->RespawnFrom(m_saved_player);
        m_stored_player_pos_x = m_saved_stored_player_pos_x;
        m_should_load_when_active_again = false;
    } else {
        SaveGame(m_player);
    }
    SetDone(false);
    m_next_app_state.reset();
}

void Game::Init() {
    if (m_should_load_when_active_again) {
        return;
    }

    if (!m_level) {
        // ładowanie poziomu i sprite'ów planszy
        m_level.reset(new Level());
        m_level->LoadFromFile("data/" + m_level_name + ".lvl");
        if (!m_level->Loaded()) {
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
            m_player.reset(new Player(player_data.x, player_data.y, m_level->GetWidth(), /*lifes*/3, /*score*/0));
            SpriteConfigPtr spriteConfig = Engine::Get().GetSpriteConfig();
            m_player->SetSprites(SpritePtr(new Sprite(spriteConfig->Get("player_left"))),
                                 SpritePtr(new Sprite(spriteConfig->Get("player_right"))),
                                 SpritePtr(new Sprite(spriteConfig->Get("player_stop"))) );
        } else {
            std::cerr << "brak informacji o postaci gracza w pliku z poziomem" << std::endl;
        }
    }
    else {
        m_stored_player_pos_x = 9;
        m_player->NewLevelReset(m_level);

    }
}

void Game::HandleCollisionPlayerWithSolidEntity(PlayerPtr player, EntityPtr entity, double dt) {
    Aabb entity_box = entity->GetNextAabb(dt);
    Aabb player_box = player->GetNextAabb(dt);
    Aabb player_curr_box = player->GetAabb();
    if (player_box.IsUnder(entity_box)) {
        player->Fall();
    }
    if (player_box.IsOver(entity_box)) {
        player->PlayerOnGround();
    }
    if (player_curr_box.IsOnLeftOf(entity_box)) {
        player->ForbidGoingRight();
    }
    if (player_curr_box.IsOnRightOf(entity_box)) {
        player->ForbidGoingLeft();
    }
}

void Game::CheckPlayerEntitiesCollisions(double dt) {
    // poziomy i pionowy aabb gracza w następnym 'kroku'
    Aabb player_box_x = m_player->GetNextHorizontalAabb(dt);
    Aabb player_box_y = m_player->GetNextVerticalAabb(dt);

    BOOST_FOREACH(EntityPtr entity, m_entities) {
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
                m_player->IncreaseJumpHeightBonus(3);
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
        } else if (entity_type == ET::SavePoint) {
            // gracz zapisał grę
            if (m_player->GetAabb().Collides(entity->GetAabb())) {
                SavePointPtr sp = boost::dynamic_pointer_cast<SavePoint>(entity);
                if (sp && sp->IsInactive()) {
                    SaveGame(m_player);
                    sp->Activate();
                }
            }
            continue;
        } else if (entity_type == ET::Thorns && !m_player->IsImmortal()) {
            // gracz wpadł na kolce
            if (m_player->GetAabb().Collides(entity->GetAabb())) {
                m_player->LooseLife();
            }
            continue;
        } else if (entity_type == ET::Arrow && !m_player->IsImmortal()) {
            // gracz został ugodzony strzałą
            if (m_player->GetAabb().Collides(entity->GetAabb())) {
                std::cout << "kolizja gracz <-> arrow" << std::endl;
                m_player->LooseLife();
            }
            continue;
        } else if (entity_type == ET::ArrowTrigger) {
            continue;
        } else if (entity_type == ET::Column) {
            if (m_player->GetNextAabb(dt).Collides(entity->GetAabb())) {
                HandleCollisionPlayerWithSolidEntity(m_player, entity, dt);
            }
        }

        if (entity_type == ET::Mush && !m_player->IsImmortal()) {
            // sprawdź czy wystąpiła kolizja. Jeżeli tak to gracz, zdecyduje o 
            // losie swoim i jednostki. Zauważmy, że jeżeli wystąpi kolizja 
            // poniżej gracza (naskoczenie na jednostkę), to pozostałe 
            // nie będą sprawdzane.
            Aabb entity_box = entity->GetAabb();
            if (player_box_y.IsOver(entity_box)) {
                // naskoczenie na jednostkę
                m_player->CollisionUnderPlayer(entity);
                player_box_y = m_player->GetNextVerticalAabb(dt);
            } else if (player_box_x.IsOnLeftOf(entity_box)) {
                m_player->ForbidGoingRight();
                m_player->CollisionOnRight(entity);
                player_box_x = m_player->GetNextHorizontalAabb(dt);
            } else if (player_box_x.IsOnRightOf(entity_box)) {
                m_player->ForbidGoingLeft();
                m_player->CollisionOnLeft(entity);
                player_box_x = m_player->GetNextHorizontalAabb(dt);
            } else if (player_box_y.IsUnder(entity_box)) {
                m_player->Fall();
                m_player->CollisionOverPlayer(entity);
                player_box_y = m_player->GetNextVerticalAabb(dt);
            }
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
    SWAP_IF( ET::PlayerBullet, ET::Column );
    SWAP_IF( ET::Arrow, ET::Column );
    SWAP_IF( ET::Arrow, ET::Mush );
    SWAP_IF( ET::Arrow, ET::PlayerBullet );
    SWAP_IF( ET::Column, ET::Mush );

    // w tym miejscu wiemy, że jeżeli nastąpiła kolizja Mush z PlayerBullet,
    // to jednostka Mush będzie pod fst_entity a PlayerBullet pod snd_entity

    if (fst_type == ET::Mush && snd_type == ET::PlayerBullet) {
        snd_entity->SetIsDead(true);
        m_player->AddScores(fst_entity->GetScoresWhenKilled());
        fst_entity->KilledWithBullet();
    }
    
    if (fst_type == ET::Column && snd_type == ET::PlayerBullet) {
        snd_entity->NegateXVelocity();
    }

    if (fst_type == ET::Column && snd_type == ET::Arrow) {
        snd_entity->StopMovement();
    }

    if (fst_type == ET::Mush && snd_type == ET::Arrow) {
        fst_entity->SetIsDead(true);
        snd_entity->SetIsDead(true);
    }
    
    if (fst_type == ET::Mush && snd_type == ET::Column) {
        fst_entity->NegateXVelocity();
    }

    if (fst_type == ET::PlayerBullet && snd_type == ET::Arrow) {
        fst_entity->SetIsDead(true);
        snd_entity->SetIsDead(true);
        m_player->AddScores(snd_entity->GetScoresWhenKilled());
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
    BOOST_FOREACH(EntityPtr entity, m_entities) {
        if (!entity->IsDead()) {
            std::list<CreatorPtr> ent_c = entity->GetCreators();
            creators.splice(creators.end(), ent_c);
            entity->DropAllCreators();
        }
    }

    // uruchom wszystkie kreatory
    BOOST_FOREACH(CreatorPtr cr, creators) {
        cr->Create(*this);
    }
}

void Game::SweepAndAddEntities(double /* dt */) {
    // usuń martwe jednostki
    auto isEntityDead = [](EntityPtr e) { return e->IsDead(); };
    m_entities.erase(boost::remove_if(m_entities, isEntityDead), m_entities.end());

    // dodaj kolejne jednostki z listy do gry
    m_entities_to_create.sort(LevelEntityData::OrderByX);
    const double distance_of_creation = Engine::Get().GetRenderer()->GetHorizontalTilesOnScreenCount();
    auto isTooFar = [&](const LevelEntityData& data) { return data.x - m_player->GetX() > distance_of_creation; };
    auto firstEntityTooFar = boost::find_if(m_entities_to_create, isTooFar);
    std::for_each(m_entities_to_create.begin(),
                  firstEntityTooFar,
                  [&](const LevelEntityData& data)->void {
                      EntityPtr e = Engine::Get().GetEntityFactory()->CreateEntity(data);
                      m_entities.push_back(e);
                  });
    m_entities_to_create.erase(m_entities_to_create.begin(), firstEntityTooFar);
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
    if (m_player->ShouldBeRespawned()) {
        LoadGame();
        SetDone();
    }

    // uaktualnij stan jednostek
    BOOST_FOREACH(auto& entity, m_entities) {
        if (entity->IsAlive()) {
            entity->Update(dt, m_level);
        }
    }

    // usuń niepotrzebne jednostki i dodaj nowe
    SweepAndAddEntities(dt);

    // zaaktualizuj stan mapy kaflowej (np. animację kafli)
    m_level_view.Update(dt);

    // przesuń mapę (???)
    if (m_player->MoveMap()) {
        m_stored_player_pos_x = m_player->GetX();
    }

    return !IsDone();
}

void Game::Draw() const {
    if (IsClearBeforeDraw()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
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
            BOOST_FOREACH(EntityPtr entity, m_entities) {
                if (entity->IsAlive()) {
                    entity->Draw();
                }
            }
        }
        glPopAttrib();
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

//    Aabb player_box = m_player->GetAabb();
//    Engine::Get().GetRenderer()->DrawAabb(player_box);

    if (IsSwapAfterDraw()) {
        SDL_GL_SwapBuffers();
    }
}

void Game::SaveGame(PlayerPtr /* player */) {
    std::cout << "Zapisywanie gry...\n";
    m_saved_player.reset(new Player(*(m_player.get())));
    m_saved_stored_player_pos_x = m_stored_player_pos_x;
    std::cout << "Gra zapisana" << std::endl;
}

void Game::LoadGame() {
    std::cout << "Wznawianie gry" << std::endl;
    m_should_load_when_active_again = true;

    GamePtr game = shared_from_this();
    tefPtr fadeout = TransitionEffect::PrepareFadeOut(game).to(game).duration(1).delay(.2, .2).Build();
    m_next_app_state = fadeout;
}
