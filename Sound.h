#ifndef __SOUND_H__
#define __SOUND_H__
#include "StdAfx.h"


class Sound {
public:
    explicit Sound();
    void LoadSounds();
    
    void PlayMusic(const std::string& name);
    void PlaySfx(const std::string& name);

    void HaltMusic();
private:
    void LoadMusic(const std::string& name, const std::string& filename);
    void LoadSfx(const std::string& name, const std::string& filename);

private:
    std::map<std::string, Mix_Chunk*> m_sfx;
    std::map<std::string, Mix_Music*> m_music;
};

typedef boost::shared_ptr<Sound> SoundPtr;

#endif /* __SOUND_H__ */
