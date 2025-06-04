#
# Copyright 2021 Alysson Ribeiro da Silva - Federal University of Minas Gerais
#
# Permission is hereby granted, free of charge, to any person obtaining a copy 
# of this software and associated documentation files (the "Software"), to deal 
# in the Software without restriction, including without limitation the rights 
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
# of the Software, and to permit persons to whom the Software is furnished to do 
# so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all 
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

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
VPL_SRC = $(filter-out $(SRC_FOLDER)main_experimental.cc $(SRC_FOLDER)main_calibracao.cc $(SRC_FOLDER)main_comparar.cc, $(wildcard $(SRC_FOLDER)*.cc))
VPL_OBJ = $(patsubst $(SRC_FOLDER)%.cc, $(OBJ_FOLDER)%.o, $(VPL_SRC))

# Programa experimental
EXP_SRC = $(filter-out $(SRC_FOLDER)main_vpl.cc $(SRC_FOLDER)main_calibracao.cc $(SRC_FOLDER)main_comparar.cc, $(wildcard $(SRC_FOLDER)*.cc))
EXP_OBJ = $(patsubst $(SRC_FOLDER)%.cc, $(OBJ_FOLDER)%.o, $(EXP_SRC))

# Programa de calibração (novo)
CALIB_SRC = $(filter-out $(SRC_FOLDER)main_vpl.cc $(SRC_FOLDER)main_experimental.cc $(SRC_FOLDER)main_comparar.cc, $(wildcard $(SRC_FOLDER)*.cc))
CALIB_OBJ = $(patsubst $(SRC_FOLDER)%.cc, $(OBJ_FOLDER)%.o, $(CALIB_SRC))

# Programa de comparacao
COMP_SRC = $(filter-out $(SRC_FOLDER)main_vpl.cc $(SRC_FOLDER)main_experimental.cc $(SRC_FOLDER)main_calibracao.cc, $(wildcard $(SRC_FOLDER)*.cc))
COMP_OBJ = $(patsubst $(SRC_FOLDER)%.cc, $(OBJ_FOLDER)%.o, $(COMP_SRC))

# Regra genérica para compilar objetos
$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cc
	$(CC) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER)

# --- Alvos finais ---
# Alvo padrão (VPL)
all: $(VPL_OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN_FOLDER)tp1.out $(VPL_OBJ)

# Alvo experimental
experimental: $(EXP_OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN_FOLDER)experimental $(EXP_OBJ)

# Novo alvo para calibração
calibracao: $(CALIB_OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN_FOLDER)calibracao $(CALIB_OBJ)

comparacao: $(COMP_OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN_FOLDER)comparacao $(COMP_OBJ)
# Limpeza
clean:
	@rm -rf $(OBJ_FOLDER)* $(BIN_FOLDER)*

.PHONY: all experimental calibracao clean