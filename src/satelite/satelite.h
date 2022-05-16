#ifndef SATELITE_H
#define SATELITE_H
#include <string>
#include <vector>
#include <iostream>
#include "../task/task.h"

class Satelite{
	std::string name;
	std::vector<Task *>  tasks;
	int port;
	int pid;
	int sfd;
public:
	//Satelite();
	Satelite(std::string nombre, int puerto);
	std::string getName(void);
	int getPort(void);
	int addTask(Task *tarea);
	bool usingResource(int recurso);
	int run(void);
	int getId(void);
	int connect2sat(void);
	int stop(void);
	int execute(void);
	friend std::ostream& operator << (std::ostream &out, const Satelite &c);
};

std::ostream& operator << (std::ostream &out, const Satelite &s);
#endif // SATELITE.H
