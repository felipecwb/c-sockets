
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>

int main() {
	// socket
	int n_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(9002),
		.sin_addr.s_addr = INADDR_ANY
	};

	int conn_status = connect(n_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

	if (conn_status == -1) {
		printf("There was an error making a connection to the remote socket\n\n");
		return 1;
	}

	printf(" + Connecting to a local server in 9002 port!\n\n");

	char response[1024];
	recv(n_socket, &response, sizeof(response), 0);
	printf("%s", response);

	char name[30];
	scanf("%[^\n]", name);

	send(n_socket, name, sizeof(name), 0);
	recv(n_socket, &response, sizeof(response), 0);
	close(n_socket);

	printf("@server: %s\n\n", response);

	return 0;
}

