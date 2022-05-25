#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <signal.h> // SIGINT
#include <string.h> // bzero
#include <time.h>

char log_buffer[256]={0};
char name_buffer[100]={0};
#define LOG(str) strncpy(log_buffer, str, sizeof(log_buffer)), log_buffer[255]='\0', printf("[%s]: %s.\n", name_buffer, log_buffer)
#define LOG2(str, n) sprintf(log_buffer, str, n), printf("[%s]: %s.\n", name_buffer, log_buffer)
#define LOG3(str, n, m) sprintf(log_buffer, str, n, m), printf("[%s]: %s.\n", name_buffer, log_buffer)
#define LOG4(str, n, m, o) sprintf(log_buffer, str, n, m, o), printf("[%s]: %s.\n", name_buffer, log_buffer)

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
		printf("[SAT]: BAD call. Must be '%s name port'.\n", argv[0]);
		return -1;
	}
	strncpy(name_buffer, argv[1], sizeof(name_buffer));
	name_buffer[99]='\0';

	LOG2("Iniciando en puerto %s", argv[2]);
	int portno = 0;
	if((portno=atoi(argv[2]))==0){
		LOG("BAD value for port");
		return -2;
	}
	//printf("Puerto: %d\n", portno);
	int server_fd;

	if((server_fd=socket(AF_INET, SOCK_STREAM, 0))==0){
		LOG("Socket FAIL");
		return -3;
	} //else printf("SAT %s: Socket inicializado.\n", argv[1]);

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family= AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; // localhost // htonl(INADDR_ANY)
	address.sin_port = htons(portno);

	// socket to address
	if(bind(server_fd, (struct sockaddr *) &address, sizeof(address))<0){
		LOG("Bind FAIL");
		return -4;
	} //else printf("SAT %s: Bind ok.\n", argv[1]);

	// socket listening
	if(listen(server_fd, 3)<0){
		LOG("Listen FAIL");
		return -5;
	} else LOG("Listening ...");

	int new_socket;
	int addrlen = sizeof(address);
	if((new_socket=accept(server_fd, (struct sockaddr *) &address, (socklen_t*)  &addrlen))<0){
		LOG("Accept FAIL");
		return -6;
	} else LOG("Socket paired");

	char buffer[1024]={0};
	int valread;
	signal(SIGINT, intHandler);
	//printf("Running...\n");
	while(keepRunning){
		struct tsk tarea;
		if((valread=read(new_socket, &tarea, sizeof(tarea)))>0){
			LOG2("Len: %d", valread);
			int j;
			char res_buf[100]={0};
			int n=0;
			for(j=0; j<tarea.num_res; j++) n=sprintf(res_buf+n, "%d,", tarea.resources[j]);
			LOG4("%s %.1f [%s]", tarea.name, tarea.pyf, res_buf);
			LOG("Procesando ..");
			bzero(buffer, 1024);
			int rand_res=rand()%10;
			if(rand_res==5) sprintf(buffer, "%s", "FAIL");
			else sprintf(buffer, "%s", "SUCCESS");
			LOG("Enviando respuesta ..");
			if(write(new_socket, buffer, 200)!=200){
				LOG("Fallo envio");
			}
		} else {
			LOG("Error on read");
			break;
		}
	}
	LOG("Cerrando conexión..");
	if(close(new_socket)!=0) LOG("Socket pair to client fail to close");
	if(close(server_fd)!=0) LOG("Socket server  fail to close");
	LOG("END");
	return 0;
}
