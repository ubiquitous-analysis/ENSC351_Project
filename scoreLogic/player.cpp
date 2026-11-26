
#include "player.h"
#include "stdio.h"
#include "pthread.h"
#include <stdio.h>

static pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;


int Player::getScore(){
    int scoreNow;
    pthread_mutex_lock(&myMutex);
    {
        scoreNow = currentScore;
    }
    pthread_mutex_unlock(&myMutex);
    return scoreNow;
}
void Player::addScore(int addedScore){
    pthread_mutex_lock(&myMutex);
    {
        printf("added score is : %d\n",addedScore);
        currentScore += addedScore;
        scoreHistory.push(addedScore);
    }
    pthread_mutex_unlock(&myMutex);
    
    return;
}
void Player::undoScore(){
    pthread_mutex_lock(&myMutex);
    {
        if(scoreHistory.size() == 0){
            printf("Score history is empty!\n");
            pthread_mutex_unlock(&myMutex);
            return;
        }
        int lastInput = scoreHistory.top();
        printf("top of the stack is: %d\n",lastInput);
        currentScore -= lastInput;
        scoreHistory.pop();
    }
    pthread_mutex_unlock(&myMutex);
    return;
}
void Player::setWin(){
    addScore(21-currentScore);
    return;
}
