#include "satelite.h"
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>

using namespace std;
//Satelite::Satelite(){}
static char path[]="/home/app/satellogic2/bin/satEmu";

struct tsk{
	char name[50];
	float pyf;
	int num_res;
	int resources[10];
};

Satelite::Satelite(std::string nombre, int puerto)
: name(nombre), port(puerto){
}

string Satelite::getName(void){
	return name;
}

int Satelite::getPort(void){
	return port;
}

int Satelite::addTask(Task *tarea){
	tasks.push_back(tarea);
	return 0;
}

bool Satelite::usingResource(int recurso){
	for(auto& task:tasks){
		if(task->hasResource(recurso)) return true;
	}
	return false;
}

int Satelite::connect2sat(void){
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		return -1;
	}
	server = gethostbyname("localhost");
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		return -2;
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);
	if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
		perror("ERROR connecting");
		return -3;
	}
	sfd=sockfd;
	return 0;
}

int Satelite::stop(){
	if(close(sfd)!=0){ 
		cout<<"Fail to close socket ("<<name<<")."<<endl;
		return -1;
	} else{
		cout<<"Socket closed ("<<name<<")."<<endl;
		return 0;
	}
}

int Satelite::run(){
	int id=-1;
	if((id=fork())<0){
		cout<<"Fallo lanzamiento de satelite."<<endl;
		return -1;
	}
	else if(id==0){
		// child
		char portStr[10]={0};
		sprintf(portStr,"%d", port);
		int e=execl(path, "satEmu", name.c_str(), portStr,NULL);
		return e;
	} else {
		// parent
		pid=id;
		return 0;
	}
}

int Satelite::getId(){
	return pid;
}

int Satelite::execute(void){
	for(auto& t: tasks){
		struct tsk tarea;
		memset(&tarea, 0, sizeof(struct tsk));
		sprintf(tarea.name, "%s", t->getName().c_str());
		tarea.pyf=t->getPayoff();
		int c=0;
		for(auto i:t->resources){
			tarea.resources[c]=i;
			c++;
		}
		tarea.num_res=c;
		int n = write(sfd,&tarea,sizeof(struct tsk));
		if (n < 0) {
		  cout<<"ERROR writing to socket"<<endl;
		  continue;
		}
		cout<<"Esperando respuesta..."<<endl;
		char buffer[200]={0};
		if((n=read(sfd, buffer, 200))>0){
			buffer[199]='\0';
			cout<<"Recibi '"<<buffer<<"'"<<endl;
		} else {
			cout<<"Fallo recpecion de respuesta."<<endl;
		}
	}
	return 0;
}

std::ostream& operator << (std::ostream &out, const Satelite &s){
	out<<s.name<<", puerto="<<s.port<<", tareas=[";
	for(auto& t:s.tasks){
		out<<t->getName()<<",";
	}
	out<<"]";
	return out;
}
