#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include "gates.h"
#include "fault_list.h"
#include <string>
#include <vector>

char file_name[100]="../files/s349f_2.txt";        // circuit_file_name
char fault_list_file[100]="../files/input_fault_list.txt";
char input_test_vector_file[100]="../files/input_test_vector_file.txt";

int main()
{
    int mode_all_fault=1; // mode_all_fault=1 to consider all faults;  mode_all_fault=0 to consider only selected faults from given list
    int test_vector_all=1; // test_vector_all=1 to consider random test vector; test_vector_all=0 to consider only given test vectors
    int i,j,k,node,value,fault_all_known=0,total_detected_faults=0;
    int number_test_vectors=100;
    Circuit_init correct_circuit;
    fault_list_init FF;
    correct_circuit.input_calc(file_name);
    int* all_detected_faults;
    FILE *fault_results;
    FILE *fault_coverage;
    char *input;
    char **input_vectors;
    char *temp_scan=(char *)malloc((total_inputs+1) * sizeof(char));
    std::vector<int> known_fault_nodes_list;
    std::vector<int> connected_fault_gates_list;
    if(!test_vector_all)
    {
        FILE *f=freopen(input_test_vector_file, "r",stdin);
        for(i=1;;i++){
            scanf("%s",temp_scan);
            if (feof(f)) break;
        }
        number_test_vectors=i;
        fclose(f);
        input_vectors =(char **) malloc(number_test_vectors * sizeof(char *));
        for (i = 0; i < number_test_vectors; i++)
            input_vectors[i] = (char *)malloc((total_inputs+1) * sizeof(char));


        f=freopen(input_test_vector_file, "r",stdin);
        for(i=0;i<number_test_vectors;i++){
            scanf("%s",input_vectors[i]);
            if(strlen(input_vectors[i])< (unsigned int) total_inputs)
            {
                number_test_vectors--;
            }
        }
        fclose(f);
    }


    for(k=0;k<number_test_vectors;k++){
        fault_all_known=0;

        if(test_vector_all)
            input=vector_generator();
        else
            input=input_vectors[k];

//        printf("vector number %d with input %s\n",k+1,input);

        correct_circuit=logic(file_name,input); //fault free values
        FF.fault_init_func(fault_list_file,mode_all_fault);
        inject_input_faults( correct_circuit, FF);//inject faults at input
        if(k==0){
            all_detected_faults=(int*)malloc(sizeof(int) * total_input_faults);
            for(i=0;i<total_input_faults;i++){
                all_detected_faults[i]=0;
            }
        }

        while(!fault_all_known){
            known_fault_nodes_list =known_fault_nodes(FF);
            connected_fault_gates_list=connected_gates(correct_circuit.Node_list,correct_circuit.Gate_list,known_fault_nodes_list);
            generate_fault_list( correct_circuit, FF,connected_fault_gates_list);

            for(i=0;i<max_node_number;i++){
                if(known_fault_nodes_list[i]!=1)  break;
            }
            if(i==max_node_number)  fault_all_known=1;
        }

        for(i=0;i<total_outputs;i++){
            for(j=1;j<=total_input_faults;j++){
                FF.node_fault[j][max_node_number]=FF.node_fault[j][max_node_number] | FF.node_fault[j][correct_circuit.output_list[i].node_number-1];
            }
        }
        total_detected_faults=0;
        for(j=1;j<=total_input_faults;j++){
            all_detected_faults[j-1]=FF.node_fault[j][max_node_number] | all_detected_faults[j-1];
            if(all_detected_faults[j-1])
                total_detected_faults++;
        }
        if(k==0){
            fault_coverage=freopen("../files/fault_coverage.txt", "w",stdout);
            printf("%s\n",file_name);
        }
        else
            fault_coverage=freopen("../files/fault_coverage.txt", "a",stdout);
        if(mode_all_fault)
            printf("number of vector %d   total faults %d    detected faults %d   fault coverage  %0.3lf\n",k+1,total_input_faults,total_detected_faults,(double)total_detected_faults/(double)total_input_faults);
        else if(k==0 && mode_all_fault==0)
            printf("change mode_all_fault variable to 1 to generate results of this file" );


        fclose(fault_coverage);

        if(k==0){
            fault_results=freopen("../files/detected_faults.txt", "w",stdout);
            printf("%s\n",file_name);
        }
        else
            fault_results=freopen("../files/detected_faults.txt", "a",stdout);
        printf("vector number %d with input %s\n",k+1,input);
        if(mode_all_fault){
            for(j=1;j<=total_input_faults;j++){
                if(FF.node_fault[j][max_node_number]){
                    printf("%d  Stuck at  %d\n",FF.fault_list[j-1].node_number,FF.fault_list[j-1].value );

                }
            }
        }
        else{
            FILE *fault_input=freopen(fault_list_file, "r",stdin);
            while(!feof(fault_input)){
                scanf("%d",&node);
                scanf("%d",&value);
                 for(j=1;j<=total_input_faults;j++){
                    if(FF.node_fault[j][max_node_number] && (FF.fault_list[j-1].node_number==node) && (FF.fault_list[j-1].value==value) ){
                        printf("%d  Stuck at  %d\n",FF.fault_list[j-1].node_number,FF.fault_list[j-1].value );

                    }
                }
            }
            fclose(fault_input);
        }
        printf("--------------------------\n");
        fclose(fault_results);
    //    freopen("CONOUT$", "w", stdout);

        free(correct_circuit.Node_list);
        free(correct_circuit.Gate_list);
        free(correct_circuit.input_list);
        free(correct_circuit.output_list);
        free(FF.fault_list);
        free(FF.node_fault);

   }
    free(all_detected_faults);
    if(!test_vector_all){
        free(temp_scan);
        for(i=0;i<number_test_vectors;i++){
            free(input_vectors[i]);
        }
    }
    return 0;
}
