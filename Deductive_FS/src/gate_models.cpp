#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include "gates.h"

int max_node_number=0,total_gates,total_inputs,total_outputs;


Node GATE::Gate_func(){

       switch(type) {
          case INV :
             out.value=!in1.value;
             break;
          case BUF :
              out.value=in1.value;
              break;
          case NOR :
             out.value=!(in1.value|in2.value);
             break;
          case NAND :
             out.value=!(in1.value & in2.value);
             break;
          case AND :
             out.value=in1.value & in2.value;
             break;
          case OR :
             out.value=in1.value | in2.value;
             break;
       }
       return out;

    }




int gate_convert(char *ch){

    if (!strcmp(ch,"INV")) return INV;
    else if (!strcmp(ch,"BUF")) return BUF;
    else if (!strcmp(ch,"NOR")) return NOR;
    else if (!strcmp(ch,"NAND")) return NAND;
    else if (!strcmp(ch,"AND")) return AND;
    else if (!strcmp(ch,"OR")) return OR;
    else if (!strcmp(ch,"INPUT")) return INPUT;
    else if (!strcmp(ch,"OUTPUT")) return OUTPUT;

    return -1;
}

void Circuit_init::init_func(char *file_name, char *input){
    int i,j,a,b;
    char s[10];
    FILE *f=freopen(file_name, "r",stdin);

    for(i=1;;i++){
        a=-1; b=0;
        scanf("%s",s);
        for (j=1;;j++){
            b=a;
            scanf("%d",&a);
            if(b==a) break;
            if(a>max_node_number) max_node_number=a;
        }
        if (strcmp(s,"INPUT") == 0) total_inputs=j-2;
        if (strcmp(s,"OUTPUT") == 0) total_outputs=j-2;
        if (feof(f)) break;
    }
    total_gates=i-2;
    Node_list = (Node*)malloc(sizeof(Node) * max_node_number);
    Gate_list = (GATE*)malloc(sizeof(GATE) * total_gates);
    input_list = (Node*)malloc(sizeof(Node) * total_inputs);
    output_list = (Node*)malloc(sizeof(Node) * total_outputs);
    for (i=1;i <= max_node_number ;i++)
    {
        Node_list[i-1].node_number=i;
        Node_list[i-1].value=-1;
    }

    fclose(f);
    FILE *f2=freopen(file_name, "r",stdin);

    for(i=0;i<total_gates;i++){
        scanf("%s",s);
        Gate_list[i].type=gate_convert(s);

        if (Gate_list[i].type==INV || Gate_list[i].type==BUF)
            scanf("%d %d",&Gate_list[i].in1.node_number,&Gate_list[i].out.node_number);
        else
            scanf("%d %d %d",&Gate_list[i].in1.node_number,&Gate_list[i].in2.node_number,&Gate_list[i].out.node_number);
    }

    scanf("%s",s);
    for(i=0;i<total_inputs;i++){
        scanf("%d",&input_list[i].node_number);
        input_list[i].value=(int)(input[i])-48;
        Node_list[input_list[i].node_number-1].value=(int)(input[i])-48;
    }

    scanf("%d %s",&a,s);
    for(i=0;i<total_outputs;i++){
        scanf("%d",&output_list[i].node_number);
    }

    for(i=0;i<total_gates;i++){
        Gate_list[i].in1.value=Node_list[Gate_list[i].in1.node_number-1].value;
        if(!(Gate_list[i].type==INV || Gate_list[i].type==BUF))
            Gate_list[i].in2.value=Node_list[Gate_list[i].in2.node_number-1].value;
    }
    fclose(f2);
}

void Circuit_init::input_calc(char *file_name){
    int i,j,a,b;
    char s[10];
    FILE *f=freopen(file_name, "r",stdin);

    for(i=1;;i++){
        a=-1; b=0;
        scanf("%s",s);
        for (j=1;;j++){
            b=a;
            scanf("%d",&a);
            if(b==a) break;
            if(a>max_node_number) max_node_number=a;
        }
        if (strcmp(s,"INPUT") == 0) total_inputs=j-2;
        if (strcmp(s,"OUTPUT") == 0) total_outputs=j-2;
        if (feof(f)) break;
    }
    fclose(f);
}


std::vector<int> known_nodes(Node Node_list[]){
    int i;
    std::vector<int> known_nodes_list( max_node_number );

    for(i=0;i<max_node_number;i++){
        known_nodes_list[i]=0;
        if(Node_list[i].value >=0)
            known_nodes_list[i]=1;
    }
    return known_nodes_list;
}

std::vector<int> connected_gates(Node Node_list[],GATE Gate_list[],std::vector<int> known_nodes_list){

    std::vector<int> connected_gates_list( total_gates );
    int i,j;
    for(j=0;j<total_gates;j++){
        connected_gates_list[j]=0;
    }

    for(i=0;i<max_node_number;i++){
        if(known_nodes_list[i]){
            for(j=0;j<total_gates;j++){
                if((Gate_list[j].type==INV || Gate_list[j].type==BUF) && Gate_list[j].in1.node_number==Node_list[i].node_number){
                        connected_gates_list[j]=1;
                }
                else if(Gate_list[j].in1.node_number==Node_list[i].node_number || Gate_list[j].in2.node_number==Node_list[i].node_number )
                    connected_gates_list[j]=1;
            }
        }
    }

    for(j=0;j<total_gates;j++){
            if(connected_gates_list[j] && !(Gate_list[j].type==INV || Gate_list[j].type==BUF)){
                if(!(known_nodes_list[Gate_list[j].in1.node_number-1] && known_nodes_list[Gate_list[j].in2.node_number-1]))
                    connected_gates_list[j]=0;
            }

    }
    return connected_gates_list;

}
