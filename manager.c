#include <api.h>
#include <stdbool.h>
#include "euler.h"

bool path_only = false;
unsigned int start=NODE_B, end=NODE_C;

/**
 * Verifica se o grafo é euleriano
 * O critério é:
 *  Circuit: todos nodos de grau par
 *  Path: dois nodos ímpares
 */
bool check_eulerian()
{
    int odd_count = 0;
    for(int i = 1; i < TOTAL_TASKS; i++){
        if(deg[i] % 2){   // Sum of odd nodes
            odd_count++;
            if(odd_count == 1){  // Has starting node
                start = i;
                path_only = true;
            } else if(odd_count == 2) // Has ending node
                end = i;
            else                    // Impossible tour
                return false;
        }
    }

    if(odd_count){
        Echo("Only path is possible.");
    } else {
        Echo("Circuit is possible.");
    }

    return true;
}

int main()
{
    if(!check_eulerian()){
        Echo("The graph is not Eulerian. Quiting.");
        exit();
    }

    Message msg;
    msg.length = LENGHT;
    MSG_OP = FORWARD;
    MSG_SRC = NODE_M;
    TUNUE = TOTAL_EDGES;
    BK_ADDR = NODE_M;

    Echo(start_msg[start]);
    Send(&msg, P[start]);

    while(!read_all(NODE_M, &msg));

    Echo(exit_msg[MSG_SRC]);
    MSG_OP = EXIT; //Exit
    MSG_SRC = NODE_M; //From manager
    Echo("Exiting nodes.");
    for(int i = 1; i < TOTAL_TASKS; i++){
        Send(&msg, P[i]);
    }
    
    Echo("Exiting main");
    exit();
}
