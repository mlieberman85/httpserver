#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	if(argc != 2) {
		perror("usage: showip hostname\n");
		//printf("usage: showip hostname\n");
		return 1;
	}
	int status;
	struct addrinfo hints, *res, *p;
	//struct addrinfo *servinfo;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE;
	char ipstr[INET6_ADDRSTRLEN];


	if((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0){
		//perror("getaddrinfo error: %s\n", gai_strerror(status));
		perror(gai_strerror(status));
		return 2;
	}

	printf("IP addresses for %s:\n\n", argv[1]);

	for(p = res; p != NULL; p = p->ai_next){
		void *addr;
		char *ipver;

		if(p->ai_family == AF_INET) {
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}
		else if(p->ai_family == AF_INET6) {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		else {
			perror("An IP address was not returned.\n");
			return 3;
		}

		inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
		printf(" %s: %s\n", ipver, ipstr);
	}


	freeaddrinfo(res);


	return 0;
}