CC = gcc

SRC_PATH = src
INC_PATH = include
OBJ_PATH = obj
BIN_PATH = bin

LIBS = -lm
CCOPT = 
DAMAS_EXEC = damas

RM_CMD = rm -rf
MKDIR_CMD = mkdir -p

OBJS = $(OBJ_PATH)/$(DAMAS_EXEC).o

all: clean directories $(BIN_PATH)/$(DAMAS_EXEC)

clean: 
	$(RM_CMD) $(OBJ_PATH) $(BIN_PATH)

directories:
	$(MKDIR_CMD) $(OBJ_PATH) $(BIN_PATH)

$(BIN_PATH)/$(DAMAS_EXEC): directories $(OBJS)
	$(CC) -I$(INC_PATH) -o $@ $(OBJS) $(LIBS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) -c -I$(INC_PATH) $< -o $@
