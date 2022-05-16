#ifndef ESTACIONTERRENA_H
#define ESTACIONTERRENA_H
#include <vector>
#include "../satelite/satelite.h"
#include "../task/task.h"

class EstacionTerrena{
	std::string name;
	std::vector<Task *> tasks;
	std::vector<Satelite *> satellites;
public:
	//EstacionTerrena();
	EstacionTerrena(std::string nombre, std::vector<Task *> tareas, std::vector<Satelite *> satelites);
	void addTask(Task tarea);
	void addSatelite(Satelite satelite);
	void run(void);
	void stop(void);
	void work(void);
	//const& std::vector<Task *> get
};
#endif // ESTACIONTERRENA_H
