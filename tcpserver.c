#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(argc, argv)
{
	int s, fd , len;
	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
	int sin_size;
	char buf[BUFSIZ];

	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(8000);

	if((s= socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return 1;
	}
	if(bind(s, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0) {
		perror("bind");
		return 1;
	}

	listen(s, 5);

	sin_size = sizeof(struct sockaddr_in);
	if((fd = accept(s, (struct sockaddr *) &remote_addr, &sin_size)) < 0) {
		perror("accept");
		return 1;
	}
	printf("accepted client %s\n", inet_ntoa(remote_addr.sin_addr));
	len = send(fd, "Welcome to my server\n", 21, 0);
	while ((len = recv(fd, buf, BUFSIZ, 0))>0){
		buf[len] = '\0';
		printf("%s\n", buf);
		if(send(fd, buf, len, 0) < 0){
			perror("write");
			return 1;
		}
	}
	close(fd);
	close(s);
	return 0;
}