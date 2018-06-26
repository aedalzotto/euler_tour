#include <stdbool.h>
#include <stdlib.h>

#define NODE_M 0
#define NODE_A 1
#define NODE_B 2
#define NODE_C 3
#define NODE_D 4
#define NODE_E 5

/* Graph model */
/* THE GRAPH MUST BE CONNECTED */
#define TOTAL_EDGES 6
#define TOTAL_TASKS 6
const int P[TOTAL_TASKS] = {manager, node_a, node_b, node_c, node_d, node_e};

// Node degree. First always 0
const int deg[TOTAL_TASKS] = {0, 2, 2, 4, 2, 2};

const char *start_msg[TOTAL_TASKS] = {"Start M",
                                    "Start A",
                                    "Start B",
                                    "Start C",
                                    "Start D",
                                    "Start E"};
const char *exit_msg[TOTAL_TASKS] = {"Exit M",
                                    "Exit A",
                                    "Exit B",
                                    "Exit C",
                                    "Exit D",
                                    "Exit E"};
const char *rcv_msg[TOTAL_TASKS] = {"Rcv M",
                                "Rcv A",
                                "Rcv B",
                                "Rcv C",
                                "Rcv D",
                                "Rcv E"};
const char *bkt_msg[TOTAL_TASKS] = {"Bkt M",
                                    "Bkt A",
                                    "Bkt B",
                                    "Bkt C",
                                    "Bkt D",
                                    "Bkt E"};
const char *fwd_msg[TOTAL_TASKS] = {"Fwd M",
                                    "Fwd A",
                                    "Fwd B",
                                    "Fwd C",
                                    "Fwd D",
                                    "Fwd E"};

                                          // m, a, b, c, d, e
const int ntoa[TOTAL_TASKS][TOTAL_TASKS] = {{0, 0, 0, 0, 0, 0}, //m
                                            {0, 0, 0, 1, 0, 0}, //a
                                            {0, 0, 0, 1, 0, 0}, //b
                                            {0, 0, 1, 0, 2, 3}, //c
                                            {0, 0, 0, 0, 0, 1}, //d
                                            {0, 0, 0, 0, 1, 0}};//e

                                    //array  0, 1, 2, 3, 4, 5   node
const int atop[TOTAL_TASKS][TOTAL_TASKS] = {{0, 0, 0, 0, 0, 0}, //m
                                            {2, 3, 0, 0, 0, 0}, //a
                                            {1, 3, 0, 0, 0, 0}, //b
                                            {1, 2, 4, 5, 0, 0}, //c
                                            {3, 5, 0, 0, 0, 0}, //d
                                            {3, 4, 0, 0, 0, 0}};//e


// DONT CHANGE
#define LENGHT 4
#define MSG_OP msg.msg[0]
#define MSG_SRC msg.msg[1]
#define TUNUE msg.msg[2]
#define BK_ADDR msg.msg[3]

#define FORWARD 1
#define BACKTRACK -1
#define EXIT 0

bool read_all(int self, Message *msg)
{
    for(int i = 0; i < TOTAL_TASKS; i++){
        if(i == self)
            continue;
        
        if(SystemCall(READPIPE, (unsigned int*)msg, P[i], 0))
            return true;
    }
    return false;
}
