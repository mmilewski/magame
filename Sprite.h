#ifndef SPRITE_H_
#define SPRITE_H_
#include "StdAfx.h"

#include "SpriteConfig.h"


class Sprite {
public:
    explicit Sprite(const SpriteConfigData& data);

    void Update(double dt);
    void SetCurrentFrame(size_t frame_num);
    void DrawCurrentFrame(double x, double y, double width, double height);

private:
    SpriteConfigData m_data;
    size_t m_current_frame;           // numer aktualnej klatki
    double m_current_frame_duration;  // czas trwania aktualnej klatki
};

typedef boost::shared_ptr<Sprite> SpritePtr;

#endif /* SPRITE_H_ */
