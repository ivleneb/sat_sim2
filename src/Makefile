BIN=../bin
LIST=$(BIN)/satEmu $(BIN)/main

all:	$(LIST)

$(BIN)/satEmu:	sateliteemulator/sateliteemulator.c
	mkdir -p $(BIN)
	gcc -Wall -o $(BIN)/satEmu sateliteemulator/sateliteemulator.c

$(BIN)/main: estacionterrena/estacionterrena.cpp satelite/satelite.cpp task/task.cpp
	mkdir -p $(BIN)
	g++ -Wall -I./task -I./satelite -I./estacionterrena -o $(BIN)/main main.cpp  estacionterrena/estacionterrena.cpp satelite/satelite.cpp task/task.cpp


