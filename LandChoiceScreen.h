#ifndef __LANDCHOICESCREEN_H_INCLUDED__
#define __LANDCHOICESCREEN_H_INCLUDED__

#include "AppState.h"
#include "Sprite.h"

class LandChoiceScreen: public AppState {
public:
    LandChoiceScreen();
    virtual ~LandChoiceScreen();
    void Init();
    void Start();

    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    boost::shared_ptr<AppState> NextAppState() const;

private:

    int NextNode(int node) const;

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

    struct Point {
        Point(float x, float y) : x(x), y(y) {}
        float operator[](int idx) const { return (idx==0?x:y); }
        float& operator[](int idx) { return (idx==0?x:y); }
        float x, y;
    };

    typedef std::vector<bool> BoolVector;
    typedef std::vector<int> IntVector;

    std::vector<BoolVector> moves;
    std::vector<IntVector> connections;
    std::vector<Point> m_positions;     // pozycje punktów

    float face_pos_x;       // położenie bohatera na ekranie
    float face_pos_y;
    int from_node;          // numer węzła początkowego
    int to_node;            // numer węzła docelowego

    SpritePtr m_sprite;

    boost::shared_ptr<AppState> m_next_app_state;
};

#endif
