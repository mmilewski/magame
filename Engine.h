#ifndef ENGINE_H_
#define ENGINE_H_
#include "SpriteConfig.h"
#include "Renderer.h"
#include "EntityFactory.h"
#include "Sound.h"
#include "Window.h"

class Engine {
public:
    static Engine& Get() {
        static Engine engine;
        return engine;
    }

    void Load() {
        m_sprite_config.reset(new SpriteConfig::SpriteConfig());
        m_renderer.reset(new Renderer::Renderer());
        m_entity_factory.reset(new EntityFactory::EntityFactory());
        m_sound.reset(new Sound::Sound());
        m_window.reset(new Window::Window());
    }

    EntityFactoryPtr EntityFactory() { return m_entity_factory; }
    SpriteConfigPtr SpriteConfig()   { return m_sprite_config; }
    RendererPtr Renderer()           { return m_renderer; }
    SoundPtr Sound() { return m_sound; }
    WindowPtr Window() { return m_window; }

private:
    EntityFactoryPtr m_entity_factory;
    SpriteConfigPtr m_sprite_config;
    RendererPtr m_renderer;
    SoundPtr m_sound;
    WindowPtr m_window;
};

#endif
