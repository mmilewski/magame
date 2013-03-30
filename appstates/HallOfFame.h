#ifndef MAGAME_HALL_OF_FAME_H_INCLUDED
#define MAGAME_HALL_OF_FAME_H_INCLUDED
#include "StdAfx.h"

#include "AppState.h"

class HallOfFame;
typedef boost::shared_ptr<HallOfFame> HallOfFamePtr;


class HallOfFame : public AppState, public boost::enable_shared_from_this<HallOfFame>  {
private:
    struct Entry {
        std::string name;
        size_t points;
    };

public:
    explicit HallOfFame();
    static HallOfFamePtr New();

    void Draw() const;
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    void Init();
    void Start();

    bool IsDone() const { return m_is_done; }

    AppStatePtr NextAppState() const;

private:
    void LoadFromFile();

private:
    bool m_is_done;
    std::vector<Entry> m_entries;
};

#endif /* MAGAME_HALL_OF_FAME_H_INCLUDED */
