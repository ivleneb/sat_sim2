#include <iostream>
#include <string>
#include <sys/wait.h>
#include "task/task.h"
#include "satelite/satelite.h"
#include "estacionterrena/estacionterrena.h"
#include <unistd.h>

int main(){
	Task t1("fotos", 10, {1, 5});
	Task t2("mantenimiento", 1, {1, 2});
	Task t3("pruebas", 1, {5, 6});
	Task t4("fsck", 0.1, {1, 6});
	std::string n1="Satelite_1";
	std::string n2="Satelite_2";
	Satelite sat1(n1, 5000);
	Satelite sat2(n2, 5001);
	if(sat1.run()!=0) {
		std::cout<<"Fallo sat1.run."<<std::endl;
		return -1;
	}
	if(sat2.run()!=0){
		std::cout<<"Fallo sat2.run."<<std::endl;
		return -2;
	}
	sleep(5);
	EstacionTerrena et("myEst", {&t2, &t4, &t3, &t1}, {&sat1, &sat2});
	et.run();
	et.work();
	sleep(10);
	et.stop();
	int s1, s2;
	waitpid(sat1.getId(), &s1, 0);
	waitpid(sat2.getId(), &s2, 0);
	std::cout<<"Fin."<<std::endl;
}
