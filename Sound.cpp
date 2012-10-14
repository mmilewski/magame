#include "StdAfx.h"

#include "Sound.h"

Sound::Sound() {
    int audio_rate = 44100;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;
 
    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
        std::cout << "Unable to initialize audio: " << Mix_GetError() << "\n";
        exit(1);
    }
}

void Sound::LoadSounds() {
    std::ifstream settings("data/sounds.txt");
    
    std::string type;
    std::string name;
    std::string filename;
    while (settings) {
        settings >> type >> name;
        std::getline(settings, filename);
        boost::erase_all(filename, " ");
        if (type == "music") {
            LoadMusic(name, filename);
        }
        else if (type == "sfx") {
            LoadSfx(name, filename);
        }
        else {
            std::cout << "Unknown sound type: '" << type << "'\n";
        }
    }
}

void Sound::LoadMusic(const std::string& name, const std::string& filename) {
    Mix_Music* m = Mix_LoadMUS(filename.c_str());
    if (m) {
        m_music.insert(std::make_pair(name, m));
    }
    else {
        std::cout << "Can't load music file " << filename << "\n";
    }
}

void Sound::LoadSfx(const std::string& name, const std::string& filename) {
    Mix_Chunk* c = Mix_LoadWAV(filename.c_str());
    if (c) {
        m_sfx.insert(std::make_pair(name, c));
    }
    else {
        std::cout << "Can't load sfx file " << filename << "\n";
    }
}

void Sound::PlayMusic(const std::string& name) {
    if (m_music.count(name)) {
       Mix_PlayMusic(m_music.at(name), -1);
    }
    else {
       std::cout << "Unknown music '" << name << "'\n";
    }
}

void Sound::PlaySfx(const std::string& name) {
    if (m_sfx.count(name)) {
        if (Mix_PlayChannel(-1, m_sfx[name], 0) == -1) {
            std::cout <<"Unable to play sfx: '" << name << "'\n";
        }
    }
    else {
        std::cout << "Unknown sfx '" << name << "'\n";
    }
}

void Sound::HaltMusic() {
    Mix_HaltMusic();
}
