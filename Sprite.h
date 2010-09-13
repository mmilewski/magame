#ifndef SPRITE_H_
#define SPRITE_H_
#include "StdAfx.h"

#include "SpriteConfig.h"

class Sprite;
typedef boost::shared_ptr<Sprite> SpritePtr;

class Sprite {
public:
    explicit Sprite(const SpriteConfigData& data);

    void Update(double dt);
    void SetCurrentFrame(size_t frame_num);
    void DrawCurrentFrame(double x, double y, double width, double height);

    /* Ustawia rozmiary sprite'a powyżej których będzie on powtarzany -- rysowany ponownie.
       Wywołanie:
         sprite->SetRepeat(.5, 1.0);
         sprite->DrawCurrentFrame(x, y, 2.0, 3.5);
       Oznacza, że w poziomie sprite zostanie powtórzony czterokrotnie, a w pionie 3,5-krotnie. Każdy z narysowanych
       sprite'ów (być może poza ostatnim) będzie miał wymiary 0.5 X 1.0

       Przypuszczalnie najczęściej będzie wykorzystywane następujące wywołanie:
         sprite->SetRepeat(tile_width, tile_height);
         sprite->DrawCurrentFrame(x, y, any_width, any_height);
       Zostanie wówczas narysowany prostokąt o wymiarach any_width X any_height wypełniony sprite'mi standartowej wielkości jednego kafla.

       Argumenty width oraz height powinny być ściśle większe od 0. Zachowanie dla pozostałych wartości jest nieokreślone.
     */
    void SetRepeat(double width, double height);

    /* Po wywołaniu tej metody, sprite będzie rozciągany, a nie powtarzany. */
    void ResetRepeat();

public:
    static SpritePtr GetByName(const std::string& name);
    
private:
    SpriteConfigData m_data;
    size_t m_current_frame;           // numer aktualnej klatki
    double m_current_frame_duration;  // czas trwania aktualnej klatki

    double m_width_repeat;            // szerokość, powyżej której sprite będzie ponownie rysowany
    double m_height_repeat;           // wysokość, powyżej której sprite będzie ponownie rysowany
};


#endif /* SPRITE_H_ */
