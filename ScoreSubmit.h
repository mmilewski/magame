#ifndef MAGAME_SCORE_SUBMIT_H_INCLUDED
#define MAGAME_SCORE_SUBMIT_H_INCLUDED
#include "StdAfx.h"

#include "AppState.h"
#include "gui/InputBox.hpp"

class ScoreSubmit : public AppState, public boost::enable_shared_from_this<ScoreSubmit> {
private:
    struct Entry {
        std::string name;
        size_t points;
    };

public:
    explicit ScoreSubmit(size_t points);

    void ProcessEvents(const SDL_Event& event);
    void Draw() const;
    bool Update(double dt);

    void Init();
    void Start();

    AppStatePtr NextAppState() const;

private:
    void StoreInFile();

    boost::shared_ptr<gui::InputBox> m_name_input;
    size_t m_points;
};

typedef boost::shared_ptr<ScoreSubmit> ScoreSubmitPtr; 

#endif /* MAGAME_SCORE_SUBMIT_H_INCLUDED */
