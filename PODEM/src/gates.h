#ifndef GATES_H_INCLUDED
#define GATES_H_INCLUDED
#include <vector>
extern int max_node_number,total_gates,total_inputs,total_outputs,D,D_bar,xx;
extern char input[1000];
typedef enum Gate_Type_ENUM {
    INV,
    BUF,
    NOR,
    NAND,
    AND,
    OR,
    INPUT,
    OUTPUT,
    NUM_GATE_TYPES
} Gate_Type;

typedef struct Node {
    int node_number;
    int value;
    //int* Node_fault_list ;
}Node;


class GATE {

public:
    Node in1,in2,out;
    int type;

    Node Gate_func();

};

class Circuit_init {

public:

    Node* Node_list;
    Node* input_list;
    Node* output_list;
    GATE* Gate_list;
    Node fault_location;
    std::vector<int> D_frontier;
    void xxinit_func(char *file_name);
    void input_calc(char *file_name);
    void build_D_frontier(void);
    int test_not_possible(Node fault_given);

    Node Objective(Node fault_given);
    Node Backtrace(Node objective);
    void implications(int node_number, int node_value, int fault_node, int fault_value);
    int PODEM(void);
};



int gate_convert(char *ch);
int output_check(Node Node_list[],Node output_list[]);
std::vector<int> known_nodes(Node Node_list[]);
std::vector<int> connected_gates(Node Node_list[],GATE Gate_list[],std::vector<int>);


#endif // GATES_H_INCLUDED
