#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define P_SIZE sizeof(struct psubasta)
//#define clear() printf("\033[H\033[J")

struct psubasta
{
	uint16_t oferta;
	uint16_t mejor_oferta;
};

int main()
{
	const int PORT = 4444;	

	int sd;
	char buffer[P_SIZE];
	struct psubasta *subasta;
	socklen_t lon;
	struct sockaddr_in servidor;
	struct sockaddr_in cliente;

	int valor_ganador;
	int timer;
	int variable;

	sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;
	//setea el tiempo de espera del socket hasta que se recibe una oferta
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(PORT);
	servidor.sin_addr.s_addr = INADDR_ANY;

	lon = sizeof(struct sockaddr_in);

	if (bind(sd, (struct sockaddr *)&servidor, lon) < 0)
	{
		perror("Error en bind");
		exit(-1);
	}
	
	valor_ganador = 0;
	timer = 10; // contador para simular los 10 segundos de vigencia de la subasta
	while (timer != 0)
	{
	
		printf("Quién ofrece más de %d ...recibiendo ofertas\n",valor_ganador);
		
		//en variable se obtiene un valor distinto de -1, si llega una oferta
		variable = recvfrom(sd, buffer, P_SIZE, 0, (struct sockaddr *)&cliente, &lon); 
        
		if (variable != -1)
		{
			
			subasta = (struct psubasta *)buffer;

			if ( ntohs (subasta->oferta ) > valor_ganador)
			{
				valor_ganador = ntohs (subasta->oferta);
				subasta->mejor_oferta = valor_ganador;
				timer = 10;
			}

			subasta->mejor_oferta = valor_ganador;

			printf("Última oferta recibida %d\n", ntohs(subasta->oferta));

			sendto(sd, buffer, P_SIZE, 0, (struct sockaddr *)&cliente, lon);
		}
		
		timer = timer - 1;
		printf("Tiempo restante %d\n", timer);
		sleep(1);
		//clear();
		system("clear");
	}

	printf("Finalizó la subasta, con la siguiente oferta ganadora %d\n", subasta->mejor_oferta);
	
	close(sd);

	return (0);
}