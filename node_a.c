#include <api.h>
#include <stdbool.h>

#define THIS_NODE 1
#define NODE_N 3

// Number of neighbors
#define N_ADJ 2

// Edges weight           to: b  c
unsigned int weight[N_ADJ] = {0, 0};

unsigned int counterf = 0;
unsigned int node_i = 0;
unsigned int backtracks = 0;
Message msg;

void next_hop()
{
    msg.msg[1] = THIS_NODE;
    if(N_ADJ - counterf - backtracks > 1)
        msg.msg[3] = THIS_NODE;

    if(N_ADJ - counterf - backtracks){ // Send Forward
        msg.msg[0] = 1;
        msg.msg[2]--;
        for(int i = 0; i < N_ADJ; i++){
            if(!weight[i]){
                weight[i] = ++counterf;
                switch(i){
                case 0:
                    Echo("Forwarding to B");
                    Send(&msg, node_b);
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
            Send(&msg, manager);
            return;
        }
        msg.msg[0] = -1;
        char buffer[18] = "Backtracking to ";
        buffer[16] = 0x40+msg.msg[3];
        buffer[17] = 0;
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
    Echo("Starting node A");
    while(true){
        while(true) {
            for(int i = 0; i <= NODE_N; i++) {
                if(SystemCall(READPIPE, (unsigned int*)&msg, i, 0))
                    goto read_ok;
            }
        }

    read_ok:
        if(!msg.msg[0]){        // Exit
            Echo("Node A exiting");
            exit();
        } else if(msg.msg[0]>0){ // Received Forward
            // Node to vector. A std::map would be nice.
            switch(msg.msg[1]){
            case 0: // From main
                Echo("Received from main. Start at A.");
                break;
            case 2: // From node b
                Echo("Received from B.");
                weight[0] = ++counterf;
                break;
            case 3: // From node c
                Echo("Received from C.");
                weight[1] = ++counterf;
                break;
            }
            next_hop();
        } else { // Backtrack
            // Find backtracked edge
            Echo("Received backtrack request.");
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
