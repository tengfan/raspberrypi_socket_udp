// @TITLE : I2C -> UDP
// @AUTHOR : Ricardo Barbedo and Teng FAN

#include <errno.h>                 // biblio des erreurs
#include <sys/socket.h>            // biblio pour les socket
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <stdio.h>                 // biblio pour les E/S.
#include <stdlib.h> 
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

void delay(int i)    /*Pause l'application pour i seconds*/
{
    clock_t start,end;
    start=clock();
    while(((end=clock())-start)<=i*CLOCKS_PER_SEC/1000); //par millisecond
}

int main( int argc, char **argv) {
	/** UDP part */
	/** UDP Variable Declaration*/
	int s;                         // socket d'ecoute du port
	char paquet_rcv[3];             // paquet recu / envoye
    char paquet_snd[3];
	int err_send,err_recv,er;
	struct sockaddr_in adServeur, adClient;    // adresse au format internet
	unsigned int ladd=sizeof (adClient);  // longueur de l'adresse de reception
	int port;						// port de connexion
	char* addrIP = argv[1];
	
	if (argc !=3 ) {
		printf ("Erreur argument 2 : no port \n"); exit (-1);
	}
	
	if (argc < 2 ) {
		printf ("Erreur argument 1 : no address and no port \n"); exit (-1);
	}
	else  sscanf(argv[2], "%d", &port);
	
	in_addr_t addrIPbin = inet_addr(addrIP);
	
	/* On ouvre la socket  Internet en mode connecte. 6 est le num閞o de TCP , cf. le fichier /etc/protocols*/
	s = socket(AF_INET, SOCK_DGRAM, 17);
	perror("socket");
  
	adServeur.sin_family = AF_INET;           // adresses de type internet
	adServeur.sin_port = htons (port);        // numero de port du serveur
	adServeur.sin_addr.s_addr = addrIPbin;   // accepte les connexions de n'importe quelle interface ethernet, wifi...
	
	/** I2C Part */
	/** I2C Variable Declaration */
	int file;
	int adapter_nr = 1;
	char filename[20];
	char buff_snd[3];
	char buff_rcv[3];
	
	
	/** Option */
	paquet_snd[0] = 'r';
	paquet_snd[1] = 'c';
	paquet_snd[2] = 'v';
	
	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
	file = open(filename, O_RDWR);
	
	if (file < 0) {
			printf("file error");
			exit(1);
	}

	int addr = 0x08;

	if (ioctl(file, I2C_SLAVE, addr) < 0) {
			printf("error io");
			exit(1);
	}
	
    do 
    {
	/** I2C Part*/
	//read
		if (read(file,buff_rcv, 3) != 3) {
			printf("error read");
		} else {
			printf("%c %c %c", buff_rcv[0], buff_rcv[1],buff_rcv[2]);
		}
		
		paquet_snd[0] = buff_rcv[0];
		paquet_snd[1] = buff_rcv[1];
		paquet_snd[2] = buff_rcv[2];

	/** UDP Part */
		err_send = sendto(s, paquet_snd, 3, 0,(struct sockaddr *) &adServeur, ladd);
		printf ("UDP snd length %d: %c %c %c\n",err_send,paquet_snd[0],paquet_snd[1],paquet_snd[2]);
		delay(5);
	}
	while ( (strncmp((char *)paquet_snd, "bye", 3)));      // on compare les 3 premiers octets recus a "bye"
    printf ("Disconnected\n"); 
    close (s);								// fermeture de la socket d'閏oute
    return(0);
}


