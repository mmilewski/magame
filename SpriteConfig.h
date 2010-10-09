#ifndef SPRITECONFIG_H_
#define SPRITECONFIG_H_
#include "StdAfx.h"

#include "Types.h"


struct SpriteConfigData {
    /**
     * frame_duration - czas trwania klatki (w sekundach)
     * left, bottom, width oraz height są współrzędnymi klatki w pikselach (na obrazku) oraz jej rozmiarami.
     * layer - poziom, na którym będzie wyświetlony sprite
     */
    explicit SpriteConfigData(DL::DisplayLayer layer, size_t frame_count, double frame_duration, double left,
                              double bottom, double width, double height, bool loop, bool dark) :
        layer(layer), frame_count(frame_count), frame_duration_time(frame_duration),
        left(left), bottom(bottom), width(width), height(height), loop(loop), dark(dark) {
    }

    DL::DisplayLayer layer;       // warstwa, na którym będzie rysowany sprite. Im bliżej 0, tym bliżej obserwatora
    size_t frame_count;           // liczba klatek w animacji
    double frame_duration_time;   // czas trwania klatki
    double left;   // położenie w poziomie pierwszej klatki animacji w obrazku (w px)
    double bottom; // położenie w pionie pierwszej klatki animacji w obrazku (w px)
    double width;  // szerokość klatki w pikselach
    double height; // wysokość klatki w pikselach
    bool loop;     // czy animacja ma być zapętlona?
    bool dark;     // czy spirte powinien być przyciemniony przy pomocy czarnej blendy?
};

class SpriteConfig {
public:
    explicit SpriteConfig();
    SpriteConfigData Get(const std::string& name) const;
    bool Contains(const std::string& name) const;

private:
    std::map<std::string, SpriteConfigData> m_data;
    void Insert(const std::string& name, const SpriteConfigData& data);
};

typedef boost::shared_ptr<SpriteConfig> SpriteConfigPtr;

#endif /* SPRITECONFIG_H_ */
