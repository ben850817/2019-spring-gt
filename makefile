TARGET  = postman.out
SRC_DIR = ./src/
INC_DIR = ./inc/
INC_FAKEMN = /ift/home/m107yhliu/fake-mininet
LIB_FAKEMN = /ift/home/m107yhliu/fake-mininet/lib

CC = g++ -std=c++11
CFLAGS += -O3 -I$(INC_DIR)
SRC     = $(wildcard $(SRC_DIR)*.cpp)
OBJ     = $(patsubst %.cpp, %.o, $(SRC))



$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -L $(INC_FAKEMN) -lfakemn

%.o : %.cpp 

	$(CC) $(CFLAGS) -c $< -o $@ -I $(LIB_FAKEMN)

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ) 
