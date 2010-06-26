#ifndef __SCORE_SUBMIT_H__
#define __SCORE_SUBMIT_H__
#include "StdAfx.h"

#include "AppState.h"


class ScoreSubmit : public AppState, public boost::enable_shared_from_this<ScoreSubmit> {
private:
    struct Entry {
        std::string name;
        size_t points;
    };

public:
    explicit ScoreSubmit(size_t points);

    void ProcessEvents(const SDL_Event& event);
    void Draw();
    bool Update(double dt);

    void Init();
    void Start();

    bool IsDone() const { return m_is_done; }

    AppStatePtr NextAppState() const;

private:
    std::pair<double, double> LetterPosition(char ch);
    void StoreInFile();

private:
    bool m_is_done;
    std::string m_player_nickname;
    size_t m_next_letter;
    size_t m_points;
    char m_highlighted_char;
    
};

typedef boost::shared_ptr<ScoreSubmit> ScoreSubmitPtr; 

#endif /* __SCORE_SUBMIT_H__ */
