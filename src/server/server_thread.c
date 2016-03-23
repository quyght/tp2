#include "server_thread.h"
#include <stdio.h>

#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <time.h>

// Variable obtenue de /conf.c
extern const int port_number;

extern const unsigned int num_resources;
extern const unsigned int num_server_threads;
extern const unsigned int max_wait_time;
extern const unsigned int server_backlog_size;
extern const unsigned int *available_resources;


unsigned int server_socket_fd;


// Structures de données pour l'algorithme du banquier.
// Pour les tableaux de 2 dimensions, utilisez des tableaux de pointeurs de tableaux.
//  ___
// |   |      _______________
// | *-----> | 5  | 7  | ...
// |___|
// |   |      ______________
// | *-----> | 12 | 23 | ...
// |___|      
// |   |
// |   |
//
int *available;
int **max;
int **allocation;
int **need;

// Variable du journal.
// Nombre de requête acceptée (ACK envoyé en réponse à REQ)
unsigned int count_accepted = 0; 

// Nombre de requête en attente (WAIT envoyé en réponse à REQ)
unsigned int count_on_wait = 0; 

// Nombre de requête refusée (REFUSE envoyé en réponse à REQ)
unsigned int count_invalid = 0; 

// Nombre de client qui se sont terminés correctement (ACC envoyé en réponse à END)
unsigned int count_dispatched = 0;

// Nombre total de requête (REQ) traités.
unsigned int request_processed = 0;

// Nombre de clients ayant envoyé le message END.
unsigned int clients_ended = 0;

// Nombre de clients. Nombre reçu du client lors de la requête BEGIN.
unsigned int num_clients; 

void
st_init ()
{
 struct sockaddr_in  client_addr;
 int clilen = sizeof(client_addr);
 int new_server_socket_fd = accept(server_socket_fd,(struct sockaddr *)&client_addr,&clilen);
  // Attend la connection d'un client et initialise les structures pour
  // l'algorithme du banquier.

  // END TODO
}
 //algorithme du banquier
void algo_banquier(){
    int *available;
    int **max;
    int **allocation;
    int **need;
    
    //declarations des tableaux statiques qui representent les requetes recu par le serveur
    
    int res_alloue[1][4] = {{4,0,0,3}};
    int max_res[1][4]={{3,2,1,4}};
    int res_dispo[4] = {1,2,2,2};
    int i,j, count = 0, safe = 1, exec;
    
    //declaration des structures des données ou seront stocké les données provenant des requetes ( tableaux ci haut)
    
    available = (int * ) malloc (num_resources * sizeof (int[num_resources]));
    for (i=0; i<num_resources; i++) {
        available[i] = res_dispo[i];
    }
    allocation = (int ** ) malloc (num_clients * sizeof (int[num_clients]));
    for (i=0;i<num_clients;i++)
    {
        allocation[i]=(int *)malloc(num_resources * sizeof(int[num_resources]));
    }
    
    need = (int ** ) malloc (num_clients * sizeof (int[num_clients]));
    for (i=0;i<num_clients;i++)
    {
        need[i]=(int *)malloc(num_resources * sizeof(int[num_resources]));
    }
    
    max = (int ** ) malloc (num_clients * sizeof (int[num_clients]));
    for (i=0;i<num_clients;i++)
    {
        max[i]=(int *)malloc(num_resources * sizeof(int[num_resources]));
    }
    
    for (i = 0; i < num_clients; i++)
    {
        for (j = 0; j < num_resources; j++)
        {
            allocation[i][j] = res_alloue[i][j];
            max[i][j] = max_res[i][j];
            if (allocation[i][j]<=0) {
                need[i][j] = max[i][j] - abs(allocation[i][j]);
            }
            else
               need[i][j] = max[i][j] - allocation[i][j];
        }
        
    }
   int running[1];
    for (i = 0; i < num_clients; i++) {
        running[i] = 1;
        count++;
    }
    
    //boucle pour verifier si l'etat est safe ou unsafe
    
    while (count != 0) {
        safe = 0;
        for (i = 0; i < num_clients; i++) {
            if (running[i]) {
                exec = 1;
                for (j = 0; j < num_resources; j++) {
                    if (need[i][j] > available[j]) {
                        exec = 0;
                        break;
                    }
                }
                if (exec) {
                    printf("\nProcess%d is executing\n", i + 1);
                    running[i] = 0;
                    count--;
                    safe = 1;
                    
                    //ici on retourne toutes les ressources allouées
                    
                    for (j = 0; j < num_resources; j++) {
                        
                       if (allocation[i][j]<=0) {
                             available[j] += abs(allocation[i][j]);
                        }
                        else
                            available[j] += allocation[i][j];
                    }
                    break;
                }
            }
        }
        if (!safe) {
            printf("\nThe processes are in unsafe state.\n");
            break;
        } else {
            printf("\nThe process is in safe state");
            printf("\nAvailable vector:");
            for (i = 0; i < num_resources; i++) {
                printf("\t%d", available[i]);
            }
            printf("\n");
        
    }
        free (allocation);
        free (max);
        free(available);
        free(need);
    }
}

void
st_process_request (server_thread * st, int socket_fd)
{
  // TODO: Remplacer le contenu de cette fonction

  char buffer[20];
  bzero (buffer, 20);
  int n = read (socket_fd, buffer, 19);
  if (n < 0)
    perror ("ERROR reading from socket");

  printf ("Thread %d received the request: %s\n", st->id, buffer);

  int answer_to_client = -(rand () % 2);
  n = sprintf (buffer, "%d", answer_to_client);
  n = write (socket_fd, buffer, n);
  if (n < 0)
    perror ("ERROR writing to socket");

  if (read (socket_fd, buffer, 255) == 0)
    {
      request_processed++;
    }
  // TODO end
};


void
st_signal ()
{
  // TODO: Remplacer le contenu de cette fonction

  

  // TODO end
}

st_recv()
{
	
}


void *
st_code (void *param)
{
  server_thread *st = (server_thread *) param;

  struct sockaddr_in thread_addr;
    socklen_t socket_len = sizeof (thread_addr);
  int thread_socket_fd = -1;
  int start = time (NULL);
  // Boucle jusqu'à ce que accept recoive la première connection.
  while (thread_socket_fd < 0)
    {
		//printf("allo");
      thread_socket_fd =
	accept (server_socket_fd, (struct sockaddr *) &thread_addr,
		&socket_len);
      if ((time (NULL) - start) >= max_wait_time)
	{
	  break;
	}
    }
	  
		//TESTING
	/*int truc;
	fd_set readfds; 
	FD_ZERO(&readfds);
	 struct timeval tv;
	 tv.tv_sec = 2;
    tv.tv_usec = 500000;
	FD_SET(client_socket, &readfds);*/
	/*unsigned int msg[3];
int lentest, bytes_sent;
lentest = 12;
bytes_sent = recv(thread_socket_fd, msg, lentest, 0);
if (bytes_sent <0){
	perror("recv");
	  exit(1);
}
printf("%d \n", msg[0]);
printf("%d \n", msg[1]);
printf("%d \n", msg[2]);
/*if (msg[0] =! 1){
	printf("Not following the protocol");
	msg[0] = 7;
	bytes_sent = send(client_socket, msg, lentest, 0);
	exit(1);
}
if (msg[1] =! num_resources){
	printf("BEGIN : pas le meme nombre de resource entre server et client");
	msg[0] = 7;
	bytes_sent = send(client_socket, msg, lentest, 0);
	exit(1);
}
msg[0] = 3;
bytes_sent = send(client_socket, msg, lentest, 0);
*/
	
   
  // Boucle de traitement des requêtes.
  while (clients_ended < num_clients)
    {
      if ((time (NULL) - start) >= max_wait_time)
	{
	  fprintf (stderr, "Time out on thread %d.\n", st->id);
	  pthread_exit (NULL);
	}
      if (thread_socket_fd > 0)
	{
	  st_process_request (st, thread_socket_fd);
	  close (thread_socket_fd);
	}
      thread_socket_fd =
	accept (server_socket_fd, (struct sockaddr *) &thread_addr,
		&socket_len);
    }
}


//
// Ouvre un socket pour le serveur. Vous devrez modifier ce code.
//
void
st_open_socket ()
{
	int lis;
	int client_socket_fd;
	int start = time(NULL);

    server_socket_fd = socket (AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (server_socket_fd < 0){
    perror ("ERROR opening socket");
     close(server_socket_fd);
	 exit(1);
	}
  struct sockaddr_in serv_addr;
  	printf("%d \n", server_socket_fd);
  bzero ((char *) &serv_addr, sizeof (serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons (port_number);
  if (bind
      (server_socket_fd, (struct sockaddr *) &serv_addr,
       sizeof (serv_addr)) < 0){
    perror ("ERROR on binding");
	close(server_socket_fd);
	exit(1);
	   }
  
  lis = listen (server_socket_fd, server_backlog_size);
  if (lis < 0){
	  perror("listen");
	  close(server_socket_fd);
		exit(1);
  }

  socklen_t socket_len = sizeof (serv_addr);
  client_socket_fd = -1;
  // Boucle jusqu'à ce que accept recoive la première connection.
  while (client_socket_fd < 0)
    {
		//printf("allo");
      client_socket_fd =
	accept (server_socket_fd, (struct sockaddr *) &serv_addr,
		&socket_len);
      if ((time (NULL) - start) >= max_wait_time)
	{
	  break;
	}
    }
	if (client_socket_fd < 0){
		perror("accept");
		close(server_socket_fd);
		exit(1);
	}
	  
		//TESTING
	/*int truc;
	fd_set readfds; 
	FD_ZERO(&readfds);
	 struct timeval tv;
	 tv.tv_sec = 2;
    tv.tv_usec = 500000;
	FD_SET(client_socket, &readfds);*/
	unsigned int msg[3];
int lentest, bytes_sent;
lentest = 12;
bytes_sent = recv(client_socket_fd, msg, lentest, 0);
if (bytes_sent <0){
	perror("recv");
	close(server_socket_fd);
	close(client_socket_fd);
	  exit(1);
}
printf("%d \n", msg[0]);
printf("%d \n", msg[1]);
printf("%d \n", msg[2]);
/*if (msg[0] =! 1){
	printf("Not following the protocol");
	msg[0] = 7;
	bytes_sent = send(client_socket, msg, lentest, 0);
	exit(1);
}
if (msg[1] =! num_resources){
	printf("BEGIN : pas le meme nombre de resource entre server et client");
	msg[0] = 7;
	bytes_sent = send(client_socket, msg, lentest, 0);
	exit(1);
}
msg[0] = 3;
bytes_sent = send(client_socket, msg, lentest, 0);
*/

}


//
// Affiche les données recueillies lors de l'exécution du
// serveur.
// La branche else ne doit PAS être modifiée.
//
void
st_print_results (FILE * fd, bool verbose)
{
  if (fd == NULL) fd = stdout;
  if (verbose)
    {
      fprintf (fd, "\n---- Résultat du serveur ----\n");
      fprintf (fd, "Requêtes acceptées: %d\n", count_accepted);
      fprintf (fd, "Requêtes : %d\n", count_on_wait);
      fprintf (fd, "Requêtes invalides: %d\n", count_invalid);
      fprintf (fd, "Clients : %d\n", count_dispatched);
      fprintf (fd, "Requêtes traitées: %d\n", request_processed);
    }
  else
    {
      fprintf (fd, "%d %d %d %d %d\n", count_accepted, count_on_wait,
	       count_invalid, count_dispatched, request_processed);
    }
}
