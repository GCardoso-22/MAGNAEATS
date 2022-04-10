#SO-043
#Diogo Chambel - 53319
#Gonçalo Cardoso - 54415
#Pedro Correia - 54570


OBJ_dir = obj
BIN_dir = bin
SRC_dir = src
INC_DIR = include

CC = gcc

CFLAGS = -g -Wall -I $(INC_DIR)
LIBS = -lm -lrt -lpthread

OBJECTS = main.o memory.o process.o client.o driver.o restaurant.o

main.o = memory.h
memory.o = memory.h
process.o = memory.h main.h process.h
client.o = memory.h main.h client.h 
driver.o =  memory.h main.h driver.h
restaurant.o = memory.h main.h restaurant.h


magnaeats: $(OBJECTS)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTS)) -o $(BIN_dir)/magnaeats $(LIBS)

%.o: $(SRC_dir)/%.c $($@)
	$(CC) $(CFLAGS) -o $(OBJ_dir)/$@ -c $<

clear:
	rm -f $(OBJ_dir)/*.o
	rm -f $(BIN_dir)/magnaeats