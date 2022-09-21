SIM_SRC  = deductive.cpp fault_list.cpp gate_models.cpp logic.cpp
SIM_OBJS = $(SIM_SRC:.cpp=.o)

all: $(SIM_SRC) deductive

%.o: %.cpp
	g++ -Wall -c -o $@ $<  

deductive: $(SIM_OBJS) 
	g++ -Wall -o $@ $^

clean: 
	rm deductive *.o 
