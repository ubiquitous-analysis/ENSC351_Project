#ifndef UDP_SCOREBOARD_H
#define UDP_SCOREBOARD_H

// creates a socket with destination addr, must be called before 
// scoreboard_send, messages go to localhost because server and
// c app are on the same board
void scoreboard_init(void);

// send score for red and blue teams in one UDP packet
// format of messages is "red,blue"
void scoreboard_send(int red, int blue);

// close socket
void scoreboard_cleanup(void);

#endif