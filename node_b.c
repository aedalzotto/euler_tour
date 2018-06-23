#include <api.h>
#include <stdbool.h>

#define THIS_NODE 2
#define NODE_N 3

// Number of neighbors
#define N_ADJ 2

// Edges weight           to: a  c
unsigned int weight[N_ADJ] = {0, 0};

unsigned int counterf = 0;
unsigned int node_i = 0;
unsigned int backtracks = 0;
Message msg;

void next_hop()
{
    msg.msg[1] = THIS_NODE; // Update source
    if(N_ADJ - counterf - backtracks > 1)
        msg.msg[3] = THIS_NODE; // Backtrack to this node

    if(N_ADJ - counterf - backtracks){ // Send Forward
        msg.msg[0] = 1; // Forward
        msg.msg[2]--;   // Tunue--
        for(int i = 0; i < N_ADJ; i++){
            if(!weight[i]){     // Find first unused edge
                weight[i] = ++counterf;
                switch(i){
                case 0:
                    Echo("Forwarding to A");
                    Send(&msg, node_a);
                    break;
                case 1:
                    Echo("Forwarding to C");
                    Send(&msg, node_c);
                    break;
                }
                break;
            }
        }
    } else { // Send Backtrack
        if(!msg.msg[2]){ // 0 available edges
            msg.msg[0] = 0;
            Echo("No more unused edges. Exiting.");
            Send(&msg, manager);    // Tell the manager to exit all nodes
            return;
        }
        msg.msg[0] = -1;    // Backtrack instruction
        char buffer[20] = "Backtracking to ";
        buffer[17] = 40+msg.msg[3];
        Echo(buffer);
        switch(msg.msg[3]){
        case 1:
            Send(&msg, node_a);
            break;
        case 2:
            Send(&msg, node_b);
            break;
        case 3:
            Send(&msg, node_c);
            break;
        } 
    }
}


int main()
{    
    Echo("Starting node B");

    while(true){
        //Try to read from all nodes + manager
        while(true) {
            for(int i = 0; i <= NODE_N; i++) {
                if(SystemCall(READPIPE, (unsigned int*)&msg, i, 0))
                    goto read_ok;
            }
        }

    read_ok:
        if(!msg.msg[0]){        // Exit this node
            Echo("Node B exiting");
            exit();
        } else if(msg.msg[0]>0){ // Forward to this node
            /**
             * Manual translate node number to array number
             * std::map from c++ would be useful with std::find
             */
            switch(msg.msg[1]){ // Source node
            case 0: // From main
                Echo("Received from main. Start at B.");
                break;
            case 1: // From node a
                Echo("Received from A.");
                weight[0] = ++counterf;
                break;
            case 3: // From node c
                Echo("Received from C.");
                weight[1] = ++counterf;
                break;
            }
            next_hop();
        } else { // Backtrack to this node
            Echo("Received backtrack request.");
            // Find backtracked edge
            int top = 0;
            for(int i = 0; i < N_ADJ; i++){
                if(weight[i]>weight[top]) // Last edge sent
                    top = i;
            }
            weight[top] = N_ADJ - backtracks;
            backtracks++;
            next_hop();
        }
    }

}
