// @TITLE : Android UDP send receive
// @AUTHOR : Ricardo Barbedo and Teng FAN

#include <errno.h>                 // biblio des erreurs
#include <sys/socket.h>            // biblio pour les socket
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <stdio.h>                 // biblio pour les E/S.
#include <stdlib.h> 
#include <time.h>

void delay(int i)    /*Pause l'application pour i milliseconds*/
{
    clock_t start,end;
    start=clock();
    while(((end=clock())-start)<=i*CLOCKS_PER_SEC/1000); //par millisecond
}

int main( int argc, char *argv[]) {
	
	int s;                         // socket d'ecoute du port
	char send_paquet[3];             // paquet envoye
	send_paquet[0] = 'o';
	send_paquet[1] = 'u';
	send_paquet[2] = 'i';
	char recv_paquet[3];             // paquet recu
    //Les erreurs
    int err_send;
    int err_recv;

	struct sockaddr_in adServeur;    // adresse au format internet
  
	unsigned int ladd=sizeof(adServeur);  // longueur de l'adresse de reception
	int port;						// port de connexion
	char* addrIP = argv[1];
    
	if (argc !=3 ) {
		printf ("Erreur: l'information ne suffit pas! \n"); exit (-1);
	}
	else  
	{
		sscanf(argv[2], "%d", &port);
	}
	
	in_addr_t addrIPbin = inet_addr(addrIP);
	
	/* On ouvre la socket  Internet en mode connecte. 6 est le numéro de TCP , cf. le fichier /etc/protocols*/
	s = socket(AF_INET, SOCK_DGRAM, 17);
	perror("socket");

	adServeur.sin_family = AF_INET;           // adresses de type internet
	adServeur.sin_port = htons (port);        // numero de port du serveur
	adServeur.sin_addr.s_addr = addrIPbin;   // accepte les connexions de n'importe quelle interface ethernet, wifi...
	/*
	//Timeout
	fd_set fd;
	struct timeval tv;
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
		perror("Error");
	}
	*/
	
	//er = connect(s, (struct sockaddr *) &adServeur, sizeof (struct sockaddr_in));
    //do
	do{
		/*
		FD_ZERO(&fd);
		FD_SET(s, &fd);
		tv.tv_sec = 2;
		tv.tv_usec = 0;//200000; //200ms
		if (select(0, &fd, NULL, NULL, &tv) > 0) {
			err_send = sendto(s, send_paquet, 3, 0,(struct sockaddr *) &adServeur, ladd);
		}
		*/
		err_send = sendto(s, send_paquet, 3, 0,(struct sockaddr *) &adServeur, ladd);
		perror("sendto");
		err_recv = recvfrom(s, recv_paquet, 3, 0, NULL,NULL);
		perror("recvfrom");
		printf("Receive package length %d = %c%c%c\n",err_recv,recv_paquet[0],recv_paquet[1],recv_paquet[2]);
		delay(5);
	}
    while ((strncmp ((char *)send_paquet, "bye", 3)));               // 
    printf ("Vous avez deconnecte.\n");
    close (s);							// fermeture de la socket d'écoute
    return 0;
}
