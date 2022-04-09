OBJ_dir = obj
BIN_dir = bin
SRC_dir = src
INC_DIR = include

CC = gcc

CFLAGS = -Wall -I $(INC_DIR)
LIBS = -lm

OBJECTS = main.o memory.o process.o client.o driver.o restaurant.o

main.o = memory.h
memory.o = memory.h
process.o = memory.h main.h process.h
client.o = memory.h main.h client.h 
driver.o =  memory.h main.h driver.h
restaurant.o = memory.h main.h restaurant.h


magnaeats: $(OBJECTS)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTS)) -o bin/magnaeats $(LIBS)

%.o: $(SRC_dir)/%.c $($@)
	$(CC) $(CFLAGS) $(INC_DIR) -o $(OBJ_dir)/$@ -c $<

clean:
	rm -f $(OBJ_dir)/*.o
	rm -f $(BIN_dir)/magnaeats