# Nome do compilador
CC = gcc

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

# Nome do executável
TARGET = $(BIN_DIR)/pa1.out

# Flags do compilador
CFLAGS = -Wall -I$(INC_DIR)

# Obtém todos os arquivos .c em src/
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Gera os nomes dos arquivos objeto correspondentes em obj/
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Regra principal
all: $(TARGET)

# Compilação do executável
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Compilação dos arquivos .c para .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Criação dos diretórios, se não existirem
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Limpeza dos arquivos gerados
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# Limpeza completa (inclui diretórios gerados)
cleanall:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

