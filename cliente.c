#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define P_SIZE sizeof(struct psubasta) 

struct psubasta {
	uint16_t oferta;
	uint16_t mejor_oferta;
};

int main(int argc, char * argv[] ) {

	const int PORT = 4444;

	int sd;
	char buffer[P_SIZE];
	char teclado[1024];
	struct psubasta *subasta;
	socklen_t lon;
	struct hostent *h;
	struct sockaddr_in servidor;

    if (argc < 2) {
    	printf("Debe ejecutar %s (nombre de host)\n", argv[0]);
   		exit (-1);
    }

	sd = socket ( PF_INET , SOCK_DGRAM , IPPROTO_UDP );

	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(PORT);
	if ( h = gethostbyname ( argv [1] ) ) {
    	memcpy ( &servidor.sin_addr , h->h_addr , h->h_length );
    } else {
        herror("DNS error");
        exit (-1);
    }

	subasta = (struct psubasta *) buffer;
	
	while ( htons (subasta->oferta ) != 0) {

		printf ("Ingrese su oferta ");
		fgets(teclado, sizeof(teclado) , stdin);
		teclado[strlen(teclado) - 1 ] = '\0';
		subasta->oferta = htons ( atoi (teclado) );
        //subasta->oferta = 0;

		lon = sizeof (struct sockaddr_in);
		printf("enviando al servidor...\n");
		sendto (sd, buffer, P_SIZE, 0, (struct sockaddr *) &servidor, lon); 

		recvfrom (sd, buffer, P_SIZE, 0, (struct sockaddr *) &servidor, &lon);
		
		printf ("La oferta ganadora hasta el momento es de %d \n", (subasta->mejor_oferta));
	};	
	
	close (sd);

	return(0);

}
