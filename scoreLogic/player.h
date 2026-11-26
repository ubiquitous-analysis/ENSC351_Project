#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H
#include <stack>
class Player{
    // class that tracks the score and score history and provides an interface to update/modify the score
    // all the functions are threadsafe
    public:
    int getScore();
    void addScore(int addedScore);
    // sets the score to 21, can be undone
    void setWin();
    // gets the last score added for this player and undoes the command. 
    void undoScore();
 
    private:
    int currentScore = 0;
    std::stack <int> scoreHistory;
};
#endif