#ifndef MAGAME_ENGINE_H_INCLUDED
#define MAGAME_ENGINE_H_INCLUDED
#include "StdAfx.h"

#include "video/SpriteConfig.h"
#include "video/Renderer.h"
#include "game/entity/EntityFactory.h"
#include "Sound.h"
#include "video/Window.h"

class Engine {
public:
    explicit Engine()
        : m_entity_factory(),
          m_sprite_config(),
          m_renderer(),
          m_sound(),
          m_window() {

    }

    static Engine& Get() {
        static Engine engine;
        return engine;
    }

    void Load() {
        m_sprite_config.reset(new SpriteConfig());
        m_renderer.reset(new Renderer());
        m_entity_factory.reset(new EntityFactory());
        m_sound.reset(new Sound());
        m_window.reset(new Window());
    }

    EntityFactoryPtr GetEntityFactory() { return m_entity_factory; }
    SpriteConfigPtr GetSpriteConfig()   { return m_sprite_config; }
    RendererPtr GetRenderer()           { return m_renderer; }
    SoundPtr GetSound()                 { return m_sound; }
    WindowPtr GetWindow()               { return m_window; }

private:
    EntityFactoryPtr m_entity_factory;
    SpriteConfigPtr m_sprite_config;
    RendererPtr m_renderer;
    SoundPtr m_sound;
    WindowPtr m_window;
};

#endif /* MAGAME_ENGINE_H_INCLUDED */
