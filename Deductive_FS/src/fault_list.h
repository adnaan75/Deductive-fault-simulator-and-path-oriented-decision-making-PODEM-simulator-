#ifndef FAULT_LIST_H_INCLUDED
#define FAULT_LIST_H_INCLUDED
#include "gates.h"
extern int total_input_faults;
typedef struct fault {
    int node_number;
    int value;

}fault;


class fault_list_init {
public:
    fault* fault_list;
    int **node_fault;
    void fault_init_func(char *file_name, int mode);

};
void inject_input_faults(Circuit_init correct_circuit,fault_list_init FF);
std::vector<int> known_fault_nodes(fault_list_init FF);
std::vector<int> connected_fault_gates(Node Node_list[],GATE Gate_list[],std::vector<int>);
void generate_fault_list(Circuit_init correct_circuit,fault_list_init FF,std::vector<int>);
char* vector_generator(void);
#endif // FAULT_LIST_H_INCLUDED
