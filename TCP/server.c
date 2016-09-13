#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>

int main() {
	int sserver = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(9002),
		.sin_addr.s_addr = INADDR_ANY
	};

	bind(sserver, (struct sockaddr*) &server_addr, sizeof(server_addr));
	listen(sserver, 5);

	while (1) {
		int sclient = accept(sserver, NULL, NULL);

		send(sclient, "What's your name children?\n", 256, 0);

		char name[30];
		recv(sclient, &name, sizeof(name), 0);

		printf("@%s reach this server! \\o/\n", name);

		char message[1024] = "Congratulations ";
		strcat(message, name);
		strcat(message, "! You reach the server!!!");

		send(sclient, message, sizeof(message), 0);
	}

	close(sserver);

	return 0;
}

