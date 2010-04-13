#ifndef __HALL_OF_FAME_H__
#define __HALL_OF_FAME_H__
#include "StdAfx.h"

#include "AppState.h"


class HallOfFame : public AppState  {
private:
    struct Entry {
        std::string name;
        size_t points;
    };

public:
    explicit HallOfFame();

    void Draw();
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

#endif /* __HALL_OF_FAME_H__ */
