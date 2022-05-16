#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h> // 
#include <netinet/in.h>
#include <stdlib.h>
#include <signal.h> // SIGINT
#include <string.h> // bzero
#include <time.h>

struct tsk{
	char name[50];
	float pyf;
	int num_res;
	int resources[10];
};

static volatile int keepRunning=1;

void intHandler(int dummy){
	keepRunning=0;
}

int main(int argc, char **argv){
	srand(time(NULL));
	if (argc!=3){
		printf("SAT: BAD call. Must be '%s name port'.\n", argv[0]);
		return -1;
	}
	printf("SAT %s: Iniciando en puerto %s\n", argv[1], argv[2]);
	int portno = 0;
	if((portno=atoi(argv[2]))==0){
		printf("SAT %s: BAD value for port.\n", argv[1]);
		return -2;
	}
	//printf("Puerto: %d\n", portno);
	int server_fd;
	
	if((server_fd=socket(AF_INET, SOCK_STREAM, 0))==0){
		printf("SAT %s: Socket FAIL.\n", argv[1]);
		return -3;
	} //else printf("SAT %s: Socket inicializado.\n", argv[1]);
	
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family= AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; // localhost // htonl(INADDR_ANY)
	address.sin_port = htons(portno);
	
	// socket to address
	if(bind(server_fd, (struct sockaddr *) &address, sizeof(address))<0){
		printf("SAT %s: Bind FAIL.\n", argv[1]);
		return -4;
	} //else printf("SAT %s: Bind ok.\n", argv[1]);
	
	// socket listening
	if(listen(server_fd, 3)<0){
		printf("SAT %s: Listen FAIL.\n", argv[1]);
		return -5;
	} else printf("SAT %s: Listening ...\n", argv[1]);
	
	int new_socket;
	int addrlen = sizeof(address);
	if((new_socket=accept(server_fd, (struct sockaddr *) &address, (socklen_t*)  &addrlen))<0){
		printf("SAT %s: Accept FAIL.\n", argv[1]);
		return -6;
	} else printf("SAT %s: Socket paired.\n", argv[1]);
	
	char buffer[1024]={0};
	int valread;
	signal(SIGINT, intHandler);
	//printf("Running...\n");
	while(keepRunning){
		struct tsk tarea;
		if((valread=read(new_socket, &tarea, sizeof(tarea)))>0){
			printf("SAT %s: Len: %d.\n", argv[1], valread);
			printf("SAT %s: %s %.1f [", argv[1], tarea.name, tarea.pyf);
			int j;
			for(j=0; j<tarea.num_res; j++) printf("%d,", tarea.resources[j]);
			printf("]\n");
			printf("SAT %s: procesando ... \n", argv[1]);
			bzero(buffer, 1024);
			int rand_res=rand()%10;
			if(rand_res==5) sprintf(buffer, "%s", "FAIL");
			else sprintf(buffer, "%s", "SUCCESS");
			if(write(new_socket, buffer, 200)!=200){
				printf("SAT %s: Fallo envio.\n", argv[1]);
			}
		} else {
			printf("SAT %s: Error on read.\n", argv[1]);
			break;
		}
	}
	printf("SAT %s: Cerrando conexión...\n", argv[1]);
	if(close(new_socket)!=0) printf("SAT %s: Socket pair to cliente fail to close.\n", argv[1]);
	if(close(server_fd)!=0) printf("SAT %s: Socket server  fail to close.\n", argv[1]);
	printf("SAT %s: END.\n", argv[1]);
	return 0;
}
