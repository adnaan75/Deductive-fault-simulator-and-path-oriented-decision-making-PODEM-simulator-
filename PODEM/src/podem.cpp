#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include "gates.h"

#include <string>
#include <vector>

char file_name[100]="../files/s349f_2.txt";
char fault_list_file[100]="../files/input_fault_list_podem.txt";


int main()
{
    Node faultx;
    int result,i,j,total_input_faults,initial=0;

    Circuit_init circuit;
    circuit.input_calc(file_name);
    FILE *generetaed_tests;
    FILE *f=freopen(fault_list_file, "r",stdin);
    for(i=1;;i++)
    {
        scanf("%d",&faultx.node_number);
        scanf("%d",&faultx.value);
        if (feof(f)) break;
    }
    total_input_faults=i;
    fclose(f);
    Node* fault_list = (Node*)malloc(sizeof(Node) * total_input_faults);

    f=freopen(fault_list_file, "r",stdin);
    for(i=0;i<total_input_faults;i++)
    {
        scanf("%d",&fault_list[i].node_number);
        scanf("%d",&fault_list[i].value);
    }



    for(j=0;j<total_input_faults;j++)
    {
        faultx.node_number=fault_list[j].node_number;
        faultx.value=fault_list[j].value;

        circuit.xxinit_func(file_name);

        circuit.fault_location.node_number=faultx.node_number;
        circuit.fault_location.value=faultx.value;

        result=circuit.PODEM();

        for(i=0;i<total_inputs;i++)
        {
            circuit.input_list[i].value=circuit.Node_list[circuit.input_list[i].node_number-1].value;
        }

        if(initial==0)
        {
            generetaed_tests=freopen("../files/generated_tests.txt", "w",stdout);
            initial=1;
        }
        else
        {
            generetaed_tests=freopen("../files/generated_tests.txt", "a",stdout);
        }
        printf("Test for Node %d stuck at %d:   ",faultx.node_number,faultx.value);
        if(result==0)
            printf("FAILURE\n");
        else
        {
            for(i=0;i<total_inputs;i++)
            {
                if (circuit.input_list[i].value!=D && circuit.input_list[i].value!=D_bar && circuit.input_list[i].value!=xx)
                {
                    printf("%d",circuit.input_list[i].value);
                }
                else if(circuit.input_list[i].value==xx)
                {
                    printf("x");
                }
                else if(circuit.input_list[i].value==D)
                {
                    printf("1");
                }
                else if(circuit.input_list[i].value==D_bar)
                {
                    printf("0");
                }

            }
            printf("\n");
        }
        fclose(generetaed_tests);
//        freopen("CONOUT$", "w", stdout);
        printf("generated test for Node %d stuck at %d\n",faultx.node_number,faultx.value);
    }

    fclose(f);



    free(circuit.Node_list);
    free(circuit.Gate_list);
    free(circuit.input_list);
    free(circuit.output_list);
    free(fault_list);

    printf("\nfinished\n");
    return 0;
}
