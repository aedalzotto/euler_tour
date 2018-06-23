#include <api.h>
#include <stdbool.h>
#include "euler.h"


/* Node Model */
#define THIS_NODE NODE_A
// Number of neighbors
#define N_ADJ 2
// Edges weight           to: b  c
unsigned int weight[N_ADJ] = {0, 0};


//DONT CHANGE
unsigned int counterf = 0;
unsigned int node_i = 0;
unsigned int backtracks = 0;

Message msg;

void next_hop()
{
    MSG_SRC = THIS_NODE;
    if(N_ADJ - counterf - backtracks > 1)
        BK_ADDR = THIS_NODE;

    if(N_ADJ - counterf - backtracks){ // Send Forward
        MSG_OP = FORWARD;
        TUNUE--;
        for(int i = 0; i < N_ADJ; i++){
            if(!weight[i]){
                weight[i] = ++counterf;
                Echo(fwd_msg[atop[THIS_NODE][i]]);
                Send(&msg, P[atop[THIS_NODE][i]]);
                break;
            }
        }
    } else { // Send Backtrack
        MSG_OP = BACKTRACK;
        Echo(bkt_msg[BK_ADDR]);
        Send(&msg, P[BK_ADDR]);
    }
}


int main()
{    
    Echo(start_msg[THIS_NODE]);
    while(true){
        while(!read_all(THIS_NODE, &msg));

        if(!MSG_OP){        // Exit
            Echo(exit_msg[THIS_NODE]);
            exit();
        } else if(!TUNUE){
            MSG_OP = EXIT;
            Echo("No more unused edges. Exiting.");
            Send(&msg, P[0]);
        } else if(MSG_OP == FORWARD){
            Echo(rcv_msg[MSG_SRC]);
            if(MSG_SRC)
                weight[ntoa[THIS_NODE][MSG_SRC]] = ++counterf;
            next_hop();
        } else { // Backtrack
            // Find backtracked edge
            Echo(bkt_msg[MSG_SRC]);
            int top = 0;
            for(int i = 0; i < N_ADJ; i++){
                if(weight[i]>weight[top])
                    top = i;
            }
            weight[top] = N_ADJ - backtracks;
            backtracks++;
            next_hop();
        }
    }
}
