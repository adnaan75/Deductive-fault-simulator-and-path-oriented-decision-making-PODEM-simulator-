SIM_SRC  = podem.cpp gate_models.cpp 
SIM_OBJS = $(SIM_SRC:.cpp=.o)

all: $(SIM_SRC) podem

%.o: %.cpp
	g++ -Wall -c -o $@ $<  

podem: $(SIM_OBJS) 
	g++ -Wall -o $@ $^

clean: 
	rm podem *.o
