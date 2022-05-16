BIN=../bin/
LIST=$(BIN)/satEmu $(BIN)/main

all:	$(LIST)

$(BIN)/satEmu:	sateliteemulator/sateliteemulator.c
	gcc -Wall -o $(BIN)/satEmu sateliteemulator/sateliteemulator.c

$(BIN)/main: estacionterrena/estacionterrena.cpp satelite/satelite.cpp task/task.cpp
	g++ -Wall -o $(BIN)/main main.cpp  estacionterrena/estacionterrena.cpp satelite/satelite.cpp task/task.cpp

