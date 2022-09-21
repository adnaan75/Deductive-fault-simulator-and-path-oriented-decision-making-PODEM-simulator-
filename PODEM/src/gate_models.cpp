#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include "gates.h"

int max_node_number=0,total_gates,total_inputs,total_outputs;
int D=2, D_bar=3, xx=4;

Node GATE::Gate_func(){

       switch(type) {
          case INV :
             if(in1.value==0 || in1.value ==1)
                out.value=!in1.value;
             else if(in1.value==D)
                out.value=D_bar;
             else if(in1.value==D_bar)
                out.value=D;
             else if(in1.value==xx)
                out.value=xx;
             break;
          case BUF :
              out.value=in1.value;
              break;
          case NOR :
             if(in1.value==1 || in2.value==1 || ((in1.value==D && in2.value==D_bar) || (in1.value==D_bar && in2.value==D)))
                out.value=0;
             else if(in1.value==xx || in2.value==xx)
                out.value=xx;
             else if((in1.value==D && in2.value==0) || (in1.value==0 && in2.value==D) || (in1.value==D && in2.value==D) )
                out.value=D_bar;
             else if((in1.value==D_bar && in2.value==0) || (in1.value==0 && in2.value==D_bar) || (in1.value==D_bar && in2.value==D_bar) )
                out.value=D;
             else
                out.value=!(in1.value | in2.value);
             break;
          case NAND :
             if(in1.value==0 || in2.value==0 || ((in1.value==D && in2.value==D_bar) || (in1.value==D_bar && in2.value==D)))
                out.value=1;
             else if(in1.value==xx || in2.value==xx)
                out.value=xx;
             else if((in1.value==D && in2.value==1) || (in1.value==1 && in2.value==D) || (in1.value==D && in2.value==D) )
                out.value=D_bar;
             else if((in1.value==D_bar && in2.value==1) || (in1.value==1 && in2.value==D_bar) || (in1.value==D_bar && in2.value==D_bar) )
                out.value=D;
             else
                out.value=!(in1.value & in2.value);
             break;
          case AND :
             if(in1.value==0 || in2.value==0 || ((in1.value==D && in2.value==D_bar) || (in1.value==D_bar && in2.value==D)))
                out.value=0;
             else if(in1.value==xx || in2.value==xx)
                out.value=xx;
             else if((in1.value==D && in2.value==1) || (in1.value==1 && in2.value==D) || (in1.value==D && in2.value==D) )
                out.value=D;
             else if((in1.value==D_bar && in2.value==1) || (in1.value==1 && in2.value==D_bar) || (in1.value==D_bar && in2.value==D_bar) )
                out.value=D_bar;
             else
                out.value=in1.value & in2.value;
             break;
          case OR :
             if(in1.value==1 || in2.value==1 || ((in1.value==D && in2.value==D_bar) || (in1.value==D_bar && in2.value==D)))
                out.value=1;
             else if(in1.value==xx || in2.value==xx)
                out.value=xx;
             else if((in1.value==D && in2.value==0) || (in1.value==0 && in2.value==D) || (in1.value==D && in2.value==D) )
                out.value=D;
             else if((in1.value==D_bar && in2.value==0) || (in1.value==0 && in2.value==D_bar) || (in1.value==D_bar && in2.value==D_bar) )
                out.value=D_bar;
             else
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



void Circuit_init::xxinit_func(char *file_name){
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
    std::vector<int> D_f(total_gates);
    D_frontier=D_f;
    for (i=1;i <= max_node_number ;i++)
    {
        Node_list[i-1].node_number=i;
        Node_list[i-1].value=xx;
    }
    for(i=0;i<total_gates;i++)
    {
        D_frontier[i]=0;
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
    for(i=0;i<max_node_number;i++){
        Node_list[i].value=xx;
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

int output_check(Node Node_list[],Node output_list[])
{
    int i,flag=0;
    for(i=0;i<total_outputs;i++)
    {
        if(Node_list[output_list[i].node_number-1].value==D || Node_list[output_list[i].node_number-1].value==D_bar)
        {
            flag=1;
        }
    }
    if(flag==1)
        return 1;
    else
        return 0;
}

void Circuit_init::build_D_frontier()
{
    int i;
    for (i=0;i<total_gates;i++)
    {
        D_frontier[i]=0;
        if(Gate_list[i].type!=INV && Gate_list[i].type!=BUF)
        {
            if((Gate_list[i].in1.value==D ||  Gate_list[i].in1.value==D_bar) && Gate_list[i].out.value==xx)
            {

                D_frontier[i]=1;
            }
            else if((Gate_list[i].in2.value==D ||  Gate_list[i].in2.value==D_bar) && Gate_list[i].out.value==xx)
            {
                D_frontier[i]=1;
            }
        }
    }

}

void Circuit_init::implications(int node_number, int node_value,int fault_node, int fault_value)
{
    int i,j,flag,all_known=0;
    //Circuit_init circuit;
    std::vector<int> known_nodes_list;
    std::vector<int> connected_gates_list;
    for(i=0;i<total_inputs;i++)
    {
        if(input_list[i].node_number==node_number)
        {
            input_list[i].value=node_value;
            Node_list[node_number-1].value=node_value;
        }
        if(input_list[i].node_number==fault_node)
        {
            if(input_list[i].value==0 && fault_value==1)
            {
                input_list[i].value=D_bar;
                Node_list[node_number-1].value=D_bar;
            }
            else if(input_list[i].value==1 && fault_value==0)
            {
                input_list[i].value=D;
                Node_list[node_number-1].value=D;
            }
        }
    }

    for(i=0;i<max_node_number;i++)
    {
        flag=0;
        for(j=0;j<total_inputs;j++)
        {
            if (Node_list[i].node_number==input_list[j].node_number)
                flag=1;
        }
        if(flag==0)
        {
            Node_list[i].value=-1;
        }
    }
    //freopen("../files/result.txt", "w",stdout);

    while(!all_known){

        known_nodes_list =known_nodes(Node_list);
        connected_gates_list=connected_gates(Node_list,Gate_list,known_nodes_list);

        //Gate output evaluation
        for(i=0;i<total_gates;i++){
            if(connected_gates_list[i]){
                Gate_list[i].out=Gate_list[i].Gate_func();
                if(Gate_list[i].out.node_number==fault_node && Gate_list[i].out.value!=-1)
                {
                    if(Gate_list[i].out.value==0 && fault_value==1)
                    {
                        Gate_list[i].out.value=D_bar;
                    }

                    else if(Gate_list[i].out.value==1 && fault_value==0)
                    {
                        Gate_list[i].out.value=D;
                    }

                }
                Node_list[Gate_list[i].out.node_number-1].value=Gate_list[i].out.value;
            }
        }

        //update gate inputs
        for(i=0;i<total_gates;i++){
            Gate_list[i].in1.value=Node_list[Gate_list[i].in1.node_number-1].value;
            if(!(Gate_list[i].type==INV || Gate_list[i].type==BUF))
                Gate_list[i].in2.value=Node_list[Gate_list[i].in2.node_number-1].value;
        }

        //checking whether all node values are known
        for(i=0;i<max_node_number;i++){
            if(Node_list[i].value<0)
            {
                break;
            }
            if(i==max_node_number-1)
                all_known=1;
        }

    }

}
int Circuit_init::test_not_possible(Node fault_given)
{
    int i,flag=0,flag_D=0;

    for (i=0;i<total_gates;i++)
    {

        if (Gate_list[i].out.node_number==fault_given.node_number && (Gate_list[i].out.value!=D && Gate_list[i].out.value!=D_bar) )
        {

            if(Gate_list[i].type!=BUF && Gate_list[i].type!=INV)
            {

                if(Gate_list[i].in1.value!=xx && Gate_list[i].in2.value!=xx)
                {

                    flag=1;
                }
            }
            else
            {

                if(Gate_list[i].in1.value!=xx && (Gate_list[i].out.value!=D && Gate_list[i].out.value!=D_bar))
                {
                    flag=1;
                }

            }

        }

        if(D_frontier[i]==1)
        {

            flag_D=1;
        }

    }

    if(flag_D!=1)
    {
        for(i=0;i<max_node_number;i++)
        {
            if((Node_list[i].value==D || Node_list[i].value==D_bar))
                flag=1;
        }
        if(Node_list[fault_given.node_number-1].value!=xx)
        {
            flag=1;
        }
    }
    if(flag)
        return 1;
    else
        return 0;

}

Node Circuit_init::Objective(Node fault_given)
{
    Node objective;
    int i;

    if (Node_list[fault_given.node_number-1].value==xx)
    {
        objective.node_number=fault_given.node_number;
        objective.value=!fault_given.value;
        return objective;
    }

    for(i=0;i<total_gates;i++)
    {
        if(D_frontier[i]==1)
        {
            if(Gate_list[i].in1.value==xx)
            {
                objective.node_number=Gate_list[i].in1.node_number;
                if(Gate_list[i].type==AND || Gate_list[i].type==NAND)
                {
                    objective.value=1;
                }
                else if(Gate_list[i].type==OR || Gate_list[i].type==NOR)
                {
                    objective.value=0;
                }
            }
            else if(Gate_list[i].in2.value==xx)
            {
                objective.node_number=Gate_list[i].in2.node_number;
                if(Gate_list[i].type==AND || Gate_list[i].type==NAND)
                {
                    objective.value=1;
                }
                else if(Gate_list[i].type==OR || Gate_list[i].type==NOR)
                {
                    objective.value=0;
                }
            }

            return objective;
        }
    }
    return objective;
}

Node Circuit_init::Backtrace(Node objective)
{
    Node input_imply;
    int k=objective.node_number;
    int flag=0,i,inversion=0;
    for(i=0;i<total_inputs;i++)
    {
        if(input_list[i].node_number==k)
            flag=1;
    }
    while(flag==0)
    {
        for(i=0;i<total_gates;i++)
        {
            if(Gate_list[i].out.node_number==k)
            {
                if(Gate_list[i].type==NAND || Gate_list[i].type==NOR || Gate_list[i].type==INV)
                {
                    inversion=inversion+1;
                }
                if(Gate_list[i].in1.value==xx)
                {
                    k=Gate_list[i].in1.node_number;
                }
                else if(Gate_list[i].in2.value==xx)
                {
                    k=Gate_list[i].in2.node_number;
                }

            }
        }
        for(i=0;i<total_inputs;i++)
        {
            if(input_list[i].node_number==k)
                flag=1;
        }

    }
    input_imply.node_number=k;
    if((inversion+objective.value)%2==0)
        input_imply.value=0;
    else
        input_imply.value=1;

    return input_imply;

}

int Circuit_init::PODEM()
{
    Node objective;
    Node input_imply;
    

    if(output_check(Node_list,output_list))
        return 1;

    if(test_not_possible(fault_location))
        return 0;

    objective=Objective(fault_location);

    input_imply=Backtrace(objective);
    
    implications(input_imply.node_number, input_imply.value,fault_location.node_number, fault_location.value);


    build_D_frontier();


    if (PODEM()==1)
        return 1;
    implications(input_imply.node_number, !input_imply.value,fault_location.node_number, fault_location.value);
    

    build_D_frontier();

    if (PODEM()==1)
        return 1;
    implications(input_imply.node_number, xx,fault_location.node_number, fault_location.value);
    return 0;
}
