#ifndef TASK_H
#define TASK_H
#include <string>
#include <vector>
#include <iostream>

class Task{
	std::string name;
	float payoff;
public:
	std::vector<int> resources;
	Task();
	Task(std::string nombre, float pago=0, std::vector<int> resources= std::vector<int>());
	void setName(std::string nombre);
	std::string getName(void);
	void setPayoff(float pago);
	float getPayoff(void);
	bool hasResource(int);
	bool operator<(const Task&  t2);
	friend std::ostream& operator << (std::ostream &out, const Task &t);
};
std::ostream& operator << (std::ostream &out, const Task &t);
#endif // TASK_H
