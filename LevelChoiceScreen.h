#ifndef __LEVELCHOICESCREEN_H_INCLUDED__
#define __LEVELCHOICESCREEN_H_INCLUDED__

#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "AppState.h"
#include "Sprite.h"

// zapowiedź klasy (forward declaration)
class Player;
typedef boost::shared_ptr<Player> PlayerPtr;


class LevelChoiceScreen: public AppState {
public:
    explicit LevelChoiceScreen(PlayerPtr player);
    virtual ~LevelChoiceScreen();
    void Init();
    void Start();

    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    boost::shared_ptr<AppState> NextAppState() const;

private:

    // następny węzeł (dowolny, z którym jest połączenie). Lub node, jeżeli nie ma połączeń
    int NextNode(size_t node) const;

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

    // rysuje drogę z punktu from do punktu to
    void DrawRoad(size_t from, size_t to) const;

    struct Point {
        Point(float x, float y) : x(x), y(y) {}
        float operator[](int idx) const { return (idx==0?x:y); }
        float& operator[](int idx) { return (idx==0?x:y); }
        float x, y;
    };

    typedef std::vector<bool> BoolVector;
    typedef std::vector<int> IntVector;

    std::vector<IntVector> m_connections;              // połączenia między węzłami
    std::vector<Point> m_positions;                    // pozycje punktów
    std::map<int, std::string> m_node_to_level_name;   // mapowanie węzeł -> nazwa poziomu

    Point m_face_pos;               // położenie bohatera na ekranie
    int m_current_from_node;        // numer węzła początkowego
    int m_current_to_node;          // numer węzła docelowego

    SpritePtr m_sprite;             // klatki z polami (droga, węzły, postać)
    float m_tile_width;             // szerokość kafla na ekranie
    float m_tile_height;            // wysokość kafla na ekranie

    PlayerPtr m_player;             // wskaźnik na gracza, który przekażemy do instancji gry
    boost::shared_ptr<AppState> m_next_app_state;
};

#endif
