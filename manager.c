#include <api.h>
#include <stdbool.h>
#include "euler.h"

/**
 * ATENÇÃO!
 * O Usuário deve garantir que o grafo é conexo
 */

/* Definir numero de nodos */
#define NODE_N 3

/* Definir Grau de cada nodo *//* a  b  c */
unsigned int nodes_deg[NODE_N] = {2, 2, 2};

bool path_only = false;
unsigned int start, end;
unsigned int tunue = 0;

/**
 * Verifica se o grafo é euleriano
 * O critério é:
 *  Circuit: todos nodos de grau par
 *  Path: dois nodos ímpares
 */
bool check_eulerian()
{
    int odd_count = 0;
    for(int i = 0; i < NODE_N; i++){
        tunue += nodes_deg[i];  // Sum of edges
        if(nodes_deg[i] % 2){   // Sum of odd nodes
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
    msg.length = 4;
    msg.msg[0] = 1; //Forward
    msg.msg[1] = 0; //From manager
    msg.msg[2] = tunue; //Total Unused Edges
    msg.msg[3] = 0; //No backtrack in main

    char buffer[20] = "Start at ";
    buffer[9] = start+65;
    buffer[10] = 0;
    Echo(buffer);
    Send(&msg, P[start+1]);
    /*
    switch(start){
    case 0:
        Echo("Start at A");
        Send(&msg, node_a);
        break;
    case 1:
        Echo("Start at B");
        Send(&msg, node_b);
        break;
    case 2:
        Echo("Start at C");
        Send(&msg, node_c);
        break;
    }
    */

    while(true) {
        for(int i = 0; i < TOTAL_TASKS; i++) {
            if(SystemCall(READPIPE, (unsigned int*)&msg, P[i], 0))
                goto read_ok;
        }
    }

read_ok:
    Echo("Received exit");

    msg.msg[0] = 0; //Exit
    msg.msg[1] = 0; //From manager
    Echo("Exiting nodes.");
    for(int i = 1; i < TOTAL_TASKS; i++){
        Send(&msg, P[i]);
    }

    Echo("Exiting main");
    exit();
}
