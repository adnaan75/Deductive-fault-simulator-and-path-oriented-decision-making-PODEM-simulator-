#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include "fault_list.h"
#include "gates.h"
#include <time.h>

int total_input_faults;

void fault_list_init::fault_init_func(char *fault_list_file, int mode)
{
    int i,j,node,value;

    FILE *fw=freopen("../files/all_fault_list.txt", "w",stdout);

    for(i=1;i<=max_node_number;i++){
        printf("%d  %d\n",i,0);
        printf("%d  %d\n",i,1);
    }
    total_input_faults=max_node_number*2;

    fclose(fw);
    fault_list = (fault*)malloc(sizeof(fault) * total_input_faults);
    FILE *fr=freopen("../files/all_fault_list.txt", "r",stdin);
    for(i=0;i<total_input_faults;i++)
    {
        scanf("%d",&node);
        scanf("%d",&value);
        fault_list[i].node_number=node;
        fault_list[i].value=value;
    }
    node_fault =(int **)malloc((total_input_faults+1)*sizeof(int *));
    for(i=0;i<total_input_faults+1;i++){
        node_fault[i]=(int *)malloc((max_node_number+1)*sizeof(int));
    }

    fclose(fr);

//    freopen("CONOUT$", "w", stdout);

    for(i=0;i<total_input_faults+1;i++){
        for(j=0;j<max_node_number+1;j++){
            node_fault[i][j]=0;
            if(i==0)
                node_fault[i][j]=-1;
        }
    }
}

void inject_input_faults(Circuit_init correct_circuit,fault_list_init FF){
    int i,j;
    for(i=0;i<total_inputs;i++){
        for(j=1;j<total_input_faults+1;j++){
            if(correct_circuit.input_list[i].node_number==FF.fault_list[j-1].node_number && (correct_circuit.input_list[i].value!=FF.fault_list[j-1].value)){
                FF.node_fault[j][correct_circuit.input_list[i].node_number-1]=1;
                FF.node_fault[0][correct_circuit.input_list[i].node_number-1]=1;

            }
        }
    }
}

std::vector<int> known_fault_nodes(fault_list_init FF){
    int i;
    std::vector<int> known_fault_nodes_list( max_node_number );

    for(i=0;i<max_node_number;i++){
        known_fault_nodes_list[i]=0;
        if(FF.node_fault[0][i] ==1)
            known_fault_nodes_list[i]=1;
    }
    return known_fault_nodes_list;
}

std::vector<int> connected_fault_gates(Node Node_list[],GATE Gate_list[],std::vector<int> known_fault_nodes_list){

    std::vector<int> connected_fault_gates_list( total_gates );
    int i,j;
    for(j=0;j<total_gates;j++){
        connected_fault_gates_list[j]=0;
    }

    for(i=0;i<max_node_number;i++){
        if(known_fault_nodes_list[i]){
            for(j=0;j<total_gates;j++){
                if((Gate_list[j].type==INV || Gate_list[j].type==BUF) && Gate_list[j].in1.node_number==Node_list[i].node_number){
                        connected_fault_gates_list[j]=1;
                }
                else if(Gate_list[j].in1.node_number==Node_list[i].node_number || Gate_list[j].in2.node_number==Node_list[i].node_number )
                    connected_fault_gates_list[j]=1;
            }
        }
    }

    for(j=0;j<total_gates;j++){
            if(connected_fault_gates_list[j] && !(Gate_list[j].type==INV || Gate_list[j].type==BUF)){
                if(!(known_fault_nodes_list[Gate_list[j].in1.node_number-1] && known_fault_nodes_list[Gate_list[j].in2.node_number-1]))
                    connected_fault_gates_list[j]=0;
            }

    }
    return connected_fault_gates_list;

}

void generate_fault_list(Circuit_init correct_circuit,fault_list_init FF,std::vector<int> connected_fault_gates_list){

    int i,j;
    for(i=0;i<total_gates;i++){
        if(connected_fault_gates_list[i]){
            if(correct_circuit.Gate_list[i].type==INV || correct_circuit.Gate_list[i].type==BUF ){
                for(j=0;j<total_input_faults;j++){
                    FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=FF.node_fault[j+1][correct_circuit.Gate_list[i].in1.node_number-1];
                    if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (FF.fault_list[j].value!=correct_circuit.Gate_list[i].out.value))
                        FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                }
                FF.node_fault[0][correct_circuit.Gate_list[i].out.node_number-1]=1;
            }
            if(correct_circuit.Gate_list[i].type==AND || correct_circuit.Gate_list[i].type==NAND ){
                if(correct_circuit.Gate_list[i].in1.value!=0 && correct_circuit.Gate_list[i].in2.value!=0){
                    for(j=0;j<total_input_faults;j++){
                        FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=FF.node_fault[j+1][correct_circuit.Gate_list[i].in1.node_number-1] | FF.node_fault[j+1][correct_circuit.Gate_list[i].in2.node_number-1];
                        if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==AND) && correct_circuit.Gate_list[i].out.value==1 && FF.fault_list[j].value==0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                        else if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==NAND) && correct_circuit.Gate_list[i].out.value==0 && FF.fault_list[j].value==1)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                    }
                    FF.node_fault[0][correct_circuit.Gate_list[i].out.node_number-1]=1;
                }
                else if(correct_circuit.Gate_list[i].in1.value==0 && correct_circuit.Gate_list[i].in2.value==0){
                    for(j=0;j<total_input_faults;j++){
                        FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=FF.node_fault[j+1][correct_circuit.Gate_list[i].in1.node_number-1] & FF.node_fault[j+1][correct_circuit.Gate_list[i].in2.node_number-1];
                        if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==AND) && correct_circuit.Gate_list[i].out.value==0 && FF.fault_list[j].value==1)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                        else if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==NAND) && correct_circuit.Gate_list[i].out.value==1 && FF.fault_list[j].value==0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                    }
                    FF.node_fault[0][correct_circuit.Gate_list[i].out.node_number-1]=1;
                }
                else if(correct_circuit.Gate_list[i].in1.value==0 && correct_circuit.Gate_list[i].in2.value==1){
                    for(j=0;j<total_input_faults;j++){
                        FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=FF.node_fault[j+1][correct_circuit.Gate_list[i].in1.node_number-1] - FF.node_fault[j+1][correct_circuit.Gate_list[i].in2.node_number-1];
                        if(FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]<0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=0;
                        if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==AND) && correct_circuit.Gate_list[i].out.value==0 && FF.fault_list[j].value==1)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                        else if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==NAND) && correct_circuit.Gate_list[i].out.value==1 && FF.fault_list[j].value==0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                    }
                    FF.node_fault[0][correct_circuit.Gate_list[i].out.node_number-1]=1;
                }
                else if(correct_circuit.Gate_list[i].in1.value==1 && correct_circuit.Gate_list[i].in2.value==0){
                    for(j=0;j<total_input_faults;j++){
                        FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=FF.node_fault[j+1][correct_circuit.Gate_list[i].in2.node_number-1] - FF.node_fault[j+1][correct_circuit.Gate_list[i].in1.node_number-1];
                        if(FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]<0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=0;
                        if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==AND) && correct_circuit.Gate_list[i].out.value==0 && FF.fault_list[j].value==1)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                        else if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==NAND) && correct_circuit.Gate_list[i].out.value==1 && FF.fault_list[j].value==0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                    }
                    FF.node_fault[0][correct_circuit.Gate_list[i].out.node_number-1]=1;
                }

            }//

            if(correct_circuit.Gate_list[i].type==OR || correct_circuit.Gate_list[i].type==NOR ){
                if(correct_circuit.Gate_list[i].in1.value!=1 && correct_circuit.Gate_list[i].in2.value!=1){
                    for(j=0;j<total_input_faults;j++){
                        FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=FF.node_fault[j+1][correct_circuit.Gate_list[i].in1.node_number-1] | FF.node_fault[j+1][correct_circuit.Gate_list[i].in2.node_number-1];
                        if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==OR) && correct_circuit.Gate_list[i].out.value==0 && FF.fault_list[j].value==1)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                        else if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==NOR) && correct_circuit.Gate_list[i].out.value==1 && FF.fault_list[j].value==0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                    }
                    FF.node_fault[0][correct_circuit.Gate_list[i].out.node_number-1]=1;
                }
                else if(correct_circuit.Gate_list[i].in1.value==1 && correct_circuit.Gate_list[i].in2.value==1){
                    for(j=0;j<total_input_faults;j++){
                        FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=FF.node_fault[j+1][correct_circuit.Gate_list[i].in1.node_number-1] & FF.node_fault[j+1][correct_circuit.Gate_list[i].in2.node_number-1];
                        if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==OR) && correct_circuit.Gate_list[i].out.value==1 && FF.fault_list[j].value==0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                        else if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==NOR) && correct_circuit.Gate_list[i].out.value==0 && FF.fault_list[j].value==1)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                    }
                    FF.node_fault[0][correct_circuit.Gate_list[i].out.node_number-1]=1;
                }
                else if(correct_circuit.Gate_list[i].in1.value==1 && correct_circuit.Gate_list[i].in2.value==0){
                    for(j=0;j<total_input_faults;j++){
                        FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=FF.node_fault[j+1][correct_circuit.Gate_list[i].in1.node_number-1] - FF.node_fault[j+1][correct_circuit.Gate_list[i].in2.node_number-1];
                        if(FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]<0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=0;
                        if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==OR) && correct_circuit.Gate_list[i].out.value==1 && FF.fault_list[j].value==0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                        else if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==NOR) && correct_circuit.Gate_list[i].out.value==0 && FF.fault_list[j].value==1)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                    }
                    FF.node_fault[0][correct_circuit.Gate_list[i].out.node_number-1]=1;
                }
                else if(correct_circuit.Gate_list[i].in1.value==0 && correct_circuit.Gate_list[i].in2.value==1){
                    for(j=0;j<total_input_faults;j++){
                        FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=FF.node_fault[j+1][correct_circuit.Gate_list[i].in2.node_number-1] - FF.node_fault[j+1][correct_circuit.Gate_list[i].in1.node_number-1];
                        if(FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]<0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=0;
                        if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==OR) && correct_circuit.Gate_list[i].out.value==1 && FF.fault_list[j].value==0)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                        else if((FF.fault_list[j].node_number==correct_circuit.Gate_list[i].out.node_number) && (correct_circuit.Gate_list[i].type==NOR) && correct_circuit.Gate_list[i].out.value==0 && FF.fault_list[j].value==1)
                            FF.node_fault[j+1][correct_circuit.Gate_list[i].out.node_number-1]=1;
                    }
                    FF.node_fault[0][correct_circuit.Gate_list[i].out.node_number-1]=1;
                }

            }//

        }
    }




}
char* vector_generator(void){
    char* input;
    int i,r;
    input=(char*)malloc(sizeof(char) * total_inputs+1);
    //random vector generator
    for (i=0;i<total_inputs;i++){
        r=rand() & 1;
        if(r)
            input[i]='1';
        else
            input[i]='0';
    }
    input[total_inputs]='\0';
    return input;
}
