#ifndef ENGINE_H_
#define ENGINE_H_
#include "StdAfx.h"

#include "video/SpriteConfig.h"
#include "video/Renderer.h"
#include "entity/EntityFactory.h"
#include "Sound.h"
#include "video/Window.h"

class Engine {
public:
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

#endif
