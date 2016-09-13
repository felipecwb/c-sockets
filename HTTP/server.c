#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>

void web_file(char *filename, char *body);
void http_header(char *response, char *http_header);
void http_find_file(const char *request, char *filename);

int main() {
	int sserver = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(8000),
		.sin_addr.s_addr = INADDR_ANY
	};

	bind(sserver, (struct sockaddr*) &server_addr, sizeof(server_addr));
	listen(sserver, 10);

	printf("+ Micro server running on local at 8000 port\n\n");

	int sclient;
	while (1) {
		char http_request[1024];
		char http_response[4096];

		sclient = accept(sserver, NULL, NULL);
		memset(http_request, 0, sizeof(http_request));
		recv(sclient, http_request, sizeof(http_request), 0);

		char filename[50];
		memset(filename, 0, sizeof(filename));
		http_find_file(http_request, filename);

		printf("+ FILE: %s\n", filename);

		char body[3072];
		memset(body,0, sizeof(body));
		web_file(filename, body);

		char header[1024];
		memset(header, 0, sizeof(header));
		http_header(body, header);

		memset(http_response, 0, sizeof(http_response));
		strcpy(http_response, header);
		strcat(http_response, body);

		send(sclient, http_response, sizeof(http_response), 0);
		close(sclient);
	}

	return 0;
}

void web_file(char *filename, char *body) {
	char fname[60] = "./web";
	strcat(fname, filename);

	FILE *fh = fopen(fname, "r");
	if (fh == NULL) {
		body[0] = '\0';
		return;
	}

	char buf[1024];
	while (! feof(fh)) {
		fgets(buf, 1024, fh);
		strcat(body, buf);
	}

	fclose(fh);
}

void http_header(char *response, char *http_header) {
	if (response[0] == '\0') {
		strcpy(http_header, "HTTP/1.1 404 Not Found\r\n");
		strcat(http_header, "Content-Type: text/plain; charset=UTF-8\r\n");
		strcat(http_header, "\n");

		strcpy(response, "File Not Found!\n\0");
		return;
	}

	strcpy(http_header, "HTTP/1.1 200 Ok\r\n");
	strcat(http_header, "\n");
}

void http_find_file(const char *request, char *filename) {
	if((strncmp(request, "GET", 3)) != 0) {
		filename = NULL;
	}

	sscanf(request, "GET %[^\? ] %*s", filename);
	if (strcmp(filename, "/") == 0 || filename == NULL) {
		strcpy(filename, "/index.html");
	}
}

