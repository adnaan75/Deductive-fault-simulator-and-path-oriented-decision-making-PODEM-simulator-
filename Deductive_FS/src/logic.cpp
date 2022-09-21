#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include "gates.h"
#include <string>
#include <vector>


Circuit_init logic(char *file_name, char *input)
{
    int i,all_known=0;
    Circuit_init circuit;
    std::vector<int> known_nodes_list;
    std::vector<int> connected_gates_list;
    circuit.init_func(file_name,input);
    //freopen("../files/result.txt", "w",stdout);

    while(!all_known){

    known_nodes_list =known_nodes(circuit.Node_list);
    connected_gates_list=connected_gates(circuit.Node_list,circuit.Gate_list,known_nodes_list);

    //Gate output evaluation
    for(i=0;i<total_gates;i++){
        if(connected_gates_list[i]){
            circuit.Gate_list[i].out=circuit.Gate_list[i].Gate_func();
            circuit.Node_list[circuit.Gate_list[i].out.node_number-1].value=circuit.Gate_list[i].out.value;
        }
    }

    //update gate inputs
    for(i=0;i<total_gates;i++){
        circuit.Gate_list[i].in1.value=circuit.Node_list[circuit.Gate_list[i].in1.node_number-1].value;
        if(!(circuit.Gate_list[i].type==INV || circuit.Gate_list[i].type==BUF))
            circuit.Gate_list[i].in2.value=circuit.Node_list[circuit.Gate_list[i].in2.node_number-1].value;
    }

    //checking whether all node values are known
    for(i=0;i<max_node_number;i++){
        if(circuit.Node_list[i].value<0)
        {
            break;
        }
        if(i==max_node_number-1)
            all_known=1;
    }

}

//writing outputs
/*
    for(i=0;i<total_outputs;i++){
        circuit.output_list[i].value=circuit.Node_list[circuit.output_list[i].node_number-1].value;
        printf("%d",circuit.output_list[i].value);
    }
*/
return circuit;
}

