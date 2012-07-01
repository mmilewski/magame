#ifndef MAGAME_LEVELCHOICESCREEN_H_INCLUDED
#define MAGAME_LEVELCHOICESCREEN_H_INCLUDED
#include "StdAfx.h"

#include "AppState.h"
#include "video/Sprite.h"

// zapowiedź klasy (forward declaration)
class Player;
typedef boost::shared_ptr<Player> PlayerPtr;

class LevelChoiceScreen;
typedef boost::shared_ptr<LevelChoiceScreen> LevelChoiceScreenPtr;

class LevelChoiceScreen: public AppState,
                         public boost::enable_shared_from_this<LevelChoiceScreen> {
public:
    explicit LevelChoiceScreen(PlayerPtr player);
    virtual ~LevelChoiceScreen();
    void Init();
    void Start();

    void Draw() const;
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    boost::shared_ptr<AppState> NextAppState() const;

    void SetPlayer(PlayerPtr player);

private:
    // zwraca nazwę poziomu dla wskazanego węzła
    std::string NodeToLevelName(int node);

    // Zmienia kierunek ruchu na 'idź w lewo' i zwraca true. Jeżeli zmiana nie była możliwa, zwraca false.
    bool GoLeft();

    // Zmienia kierunek ruchu na 'idź w prawo' i zwraca true. Jeżeli zmiana nie była możliwa, zwraca false.
    bool GoRight();

    // Zmienia kierunek ruchu na 'idź w górę' i zwraca true. Jeżeli zmiana nie była możliwa, zwraca false.
    bool GoUpward();

    // Zmienia kierunek ruchu na 'idź w dół' i zwraca true. Jeżeli zmiana nie była możliwa, zwraca false.
    bool GoDown();

    // Uruchamia konkretny poziom na podstawie węzła, w którym stoi postać. Jeżeli postać nie stoi, to nie robi nic.
    void RunLevelFromNode();

    void DrawRoad(size_t src_point_index, size_t tgt_point_index) const;
    void DrawAllRoads() const;
    void DrawAllNodes() const;
    void DrawFace() const;
    void DrawHud() const;


    typedef std::vector<int> IntVector;

    std::vector<IntVector> m_connections;              // połączenia między węzłami
    std::vector<Position> m_positions;                 // pozycje punktów
    std::map<int, std::string> m_node_to_level_name;   // mapowanie węzeł -> nazwa poziomu

    Position m_face_pos;            // położenie bohatera na ekranie
    int m_current_from_node;        // numer węzła początkowego
    int m_current_to_node;          // numer węzła docelowego

    SpritePtr m_horizontal_road_sprite;  // animacja drogi w poziomie
    SpritePtr m_vertical_road_sprite;    // animacja drogi w pionie
    SpritePtr m_entry_enabled_sprite;    // animacja aktywnego wejścia
    SpritePtr m_entry_disabled_sprite;   // animacja nieaktywnego wejścia
    SpritePtr m_face_sprite;             // animacja sprite'a bohatera

    Size m_tile_size;               // rozmiar kafla na ekranie

    PlayerPtr m_player;             // wskaźnik na gracza, który przekażemy do instancji gry
    AppStatePtr m_next_app_state;
};

#endif
