#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<fcntl.h>
#include <pthread.h>
#include <regex.h>

#define MAXCONNECTIONS 10
#define BYTES 2048

void server(char *);
void *client_response(void *a);

pthread_t connections[MAXCONNECTIONS];
int sockfd, receive;
int portNumber;
struct sockaddr_in my_addr;
int client;

int main(int argc, char* argv[]) {
	
	if (argc != 2) {
		printf("Incorrect command supplied\n");
		exit(1);
	}

	portNumber = atoi(argv[1]);

	if (portNumber < 1024 || portNumber > 60000) {
		printf("Invalid port number\n");
		exit(1);
	}
	
	char port[12];
	snprintf(port, 12, "%d", portNumber);
	
	printf("Starting server at localhost:%d\n", portNumber);
	server(port);
	
	
	socklen_t addrlen;
	addrlen = sizeof(my_addr);

	while(1) {
		int ret;
		for (client = 0; client < MAXCONNECTIONS; client++) {
			ret = pthread_create(&connections[client], NULL, client_response, (void *)(intptr_t)client);
			connections[client] = accept(sockfd, (struct sockaddr*) &my_addr, &addrlen);
			pthread_join(connections[client], NULL);
		}
	}
	return 0;
}

void server(char *port) {
	int s;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	memset(&hints, 0, sizeof(struct addrinfo));
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	s = getaddrinfo(NULL, port, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(1);
	}
	
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sockfd = socket(rp->ai_family, rp->ai_socktype, 0);
		
		if (sockfd == -1) {
			continue;
		}
		
		if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) {
			break;
		}
	}
	
	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}
	
	freeaddrinfo(result);
	
	if (listen(sockfd, MAXCONNECTIONS) != 0) {
		perror("listen");
		exit(1);
	}
}

void *client_response(void *a) {
	char buffer[BYTES], writeto[BYTES], cwd[1024];
	char *httpget;
	char *httpgettype;
	char *end = " \r\n";
	
	const char *web = "/web/index.html";
	const char *image1 = "/web/images/me.jpg";
	const char *image2 = "/web/images/computer.png";

	const char *HTTP_200_STRING = "OK\n";
	const char *HTTP_404_STRING = "Not Found\n"; 
	const char *HTTP_501_STRING = "Not Implemented\n";

	const char *chtml = "Content-Type: text/html\r\n";
	const char *ccss = "Content-Type: text/css\r\n";
	const char *cjpg = "Content-Type: image/jpeg\r\n"; 
	const char *cpng = "Content-Type: image/png\r\n";
	const char *cgif = "Content-Type: image/gif\r\n"; 
	const char *cplain = "Content-Type: text/plain\r\n";

	const char *HTTP_404_CONTENT = "<html><head><title>>404 Not Found</title></head><body><h1>404 Not Found</h1></body></html>";
	const char *HTTP_501_CONTENT = "<html><head><title>501 Not Implemented</title></head><body><h1>501 Not Implemented</h1>The server either does not recognise the request method, or it lacksthe ability to fulfill the request.</body></html>";

	getcwd(cwd, sizeof(cwd));

	int fd;
	size_t nread;
	int n = (intptr_t)a;
	
	receive = recv(connections[n], buffer, BYTES, 0);
	
	if (receive == -1) {
		perror("recv");
		exit(1);
	}
	
	else if (receive == 0) {
		fprintf(stderr, "Client disconnected\n");
		exit(1);
	}
	
	else {
		printf("%s\n", buffer);
		httpget = strtok(buffer, end);
		if (strncmp(httpget, "GET / ", 6) == 0) {
			httpgettype = strtok(NULL, end);
			if (strncmp(httpgettype, "HTTP/1.1", 8) != 0) {
				write(connections[n], HTTP_404_STRING, strlen(HTTP_404_STRING));
			}
			else {
				if ((fd = open("./web/index.html", O_RDONLY)) != -1) {
					send(connections[n], HTTP_200_STRING, strlen(HTTP_200_STRING), 0);
					while ((nread = read(fd, writeto, BYTES)) > 0) {
						write(connections[n], writeto, nread);
					}
				}
				else {
					write(connections[n], HTTP_404_CONTENT, strlen(HTTP_404_CONTENT));
				}
			}
		}
	}
	close(connections[n]);
	pthread_exit(NULL);
}