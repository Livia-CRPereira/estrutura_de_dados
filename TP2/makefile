# Compilador e flags
CC = g++
CXXFLAGS = -std=c++11 -g -Wall

# Pastas
INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./bin/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

# --- Alvos principais ---
# Programa principal (VPL)
VPL_SRC = $(wildcard $(SRC_FOLDER)*.cc)
VPL_OBJ = $(patsubst $(SRC_FOLDER)%.cc, $(OBJ_FOLDER)%.o, $(VPL_SRC))

# Regra genérica para compilar objetos
$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cc
	$(CC) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER)

# --- Alvos finais ---
# Alvo padrão (VPL)
all: $(VPL_OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN_FOLDER)tp2.out $(VPL_OBJ)

# Limpeza
clean:
	@rm -rf $(OBJ_FOLDER)* $(BIN_FOLDER)*

.PHONY: all