#include "estacionterrena.h"
#include <iostream>
#include <algorithm>

using namespace std;

/*EstacionTerrena::EstacionTerrena(){
}*/

EstacionTerrena::EstacionTerrena(std::string nombre, std::vector<Task *> tareas, std::vector<Satelite *> satelites)
:name(nombre), tasks(tareas), satellites(satelites){
	cout<<"Estacion terrena: "<<name<<endl;
	cout<<"Tareas:"<<endl;
	for (auto &t:tasks){
		cout<<*t<<endl;
	}
	cout<<"Satelites:"<<endl;
	for (auto &s:satellites){
		cout<<*s<<endl;
	}
}

void EstacionTerrena::run(){
	for (auto &s:satellites){
		if(s->connect2sat()==0){
			cout<<"Conectado a ";
		} else {
			cout<<"Fallo conexion a ";
		}
		cout<<s->getName()<<" at port "<<s->getPort()<<endl;
	}
	return;
}

void EstacionTerrena::work(){
	std::sort(tasks.begin(), tasks.end());
	//satellites[0]->addTask(tasks[0]);
	int flag;
	for(size_t i=0;i<tasks.size(); ++i){
		for(size_t k=0;k<satellites.size(); k++){
			flag=0;
			for(auto j:tasks[i]->resources){
				if(satellites[k]->usingResource(j)){
					flag=1;
					break;
				}
			}
			if (flag==0) {
				satellites[k]->addTask(tasks[i]);
				break;
			}
		}
	}
	
	cout<<"Tareas por satelite:"<<endl;
	for (auto &s:satellites){
		cout<<*s<<endl;
		cout<<"Ejecutando..."<<endl;
		s->execute();
	}
	
	
}

void EstacionTerrena::stop(){
	for (auto &s:satellites){
		if(s->stop()==0){
			cout<<"Se detuvo conexion a ";
		} else {
			cout<<"Fallo detencion de conexion a ";
		}
		cout<<s->getName()<<" at port "<<s->getPort()<<endl;
	}
	return;
}
