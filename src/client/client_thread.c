#include <stdio.h>

#include "client_thread.h"

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include "client_thread.h"
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

#define SOCK_PATH "echo_socket"

// Variable de configuration.
extern const int port_number;
extern const unsigned int num_clients;
extern const unsigned int num_resources;
extern const unsigned int num_request_per_client;
extern const unsigned int **max_resources_per_client;

// Variable du journal.
// Nombre de requêtes (REQ envoyés)
unsigned int count = 0; 

// Nombre de requête acceptée (ACK reçus en réponse à REQ)
unsigned int count_accepted = 0; 

// Nombre de requête en attente (WAIT reçus en réponse à REQ)
unsigned int count_on_wait = 0;

// Nombre de requête refusée (REFUSE reçus en réponse à REQ)
unsigned int count_invalid = 0;

// Nombre de client qui se sont terminés correctement (ACC reçu en réponse à END)
unsigned int count_dispatched = 0;

// Nombre total de requêtes envoyées.                          
unsigned int request_sent = 0; 


// Vous devez modifier cette fonction pour faire l'envoie des requêtes
// Les ressources demandées par la requête doivent être choisit aléatoirement
// (sans dépasser le maximum pour le client). Les peuvent être positive ou négative.
// Assurez-vous que la dernière requête d'un client libère toute les ressources qu'il
// a jusqu'alors accumulées.
void
send_request (int client_id, int request_id, int socket_fd)
{

  // TP2 TODO

  fprintf (stdout, "Client %d is sending its %d request\n", client_id,
	   request_id);

  // TP2 TODO:END

}


void *
ct_code (void *param)
{
  client_thread *ct = (client_thread *) param;
    
  //Initialisation pour client
  int socket_fd, connection, len;
  struct sockaddr_in addresse_serveur;
  //A GENERALISER
  char buffer[4] = {'1','2','3','4'};
  
  //Création du socket cllient
  if(socket_fd = socket(AF_INET, SOCK_STREAM,0) ==-1){
	  perror("socket");
	  exit(1);
  }
  printf("Connection en cours...\n");
  //Information du serveur
  addresse_serveur.sin_family = AF_INET;
  addresse_serveur.sin_addr.s_addr = INADDR_ANY;
  addresse_serveur.sin_port = htons (port_number);
  //Connection avec le serveur
  if(connection = connect(socket_fd, (struct addresse_socket *)&
	addresse_serveur,len) ==-1){
		perror("connection");
		exit(1);
	}
	
	
	read(socket_fd, buffer, 4);

    printf("%s", buffer);

  for (unsigned int request_id = 0; request_id < num_request_per_client;
       request_id++)
    {

      // TP2 TODO
      // Vous devez ici coder, conjointement avec le corps de send request,
      // le protocole d'envoie de requête.

      send_request (ct->id, request_id, socket_fd);

      // TP2 TODO:END

    }

  pthread_exit (NULL);
}


// 
// Vous devez changer le contenu de cette fonction afin de régler le
// problème de synchronisation de la terminaison.
// Le client doit attendre que le serveur termine le traitement de chacune
// de ses requêtes avant de terminer l'exécution. 
//
void
ct_wait_server ()
{

  // TP2 TODO

  sleep (2);

  // TP2 TODO:END

}


void
ct_init (client_thread * ct)
{
  ct->id = count++;
}

void
ct_create_and_start (client_thread * ct)
{
  pthread_attr_init (&(ct->pt_attr));
  pthread_create (&(ct->pt_tid), &(ct->pt_attr), &ct_code, ct);
  pthread_detach (ct->pt_tid);
}

//
// Affiche les données recueillies lors de l'exécution du
// serveur.
// La branche else ne doit PAS être modifiée.
//
void
st_print_results (FILE * fd, bool verbose)
{
  if (fd == NULL)
    fd = stdout;
  if (verbose)
    {
      fprintf (fd, "\n---- Résultat du client ----\n");
      fprintf (fd, "Requêtes acceptées: %d\n", count_accepted);
      fprintf (fd, "Requêtes : %d\n", count_on_wait);
      fprintf (fd, "Requêtes invalides: %d\n", count_invalid);
      fprintf (fd, "Clients : %d\n", count_dispatched);
      fprintf (fd, "Requêtes envoyées: %d\n", request_sent);
    }
  else
    {
      fprintf (fd, "%d %d %d %d %d\n", count_accepted, count_on_wait,
	       count_invalid, count_dispatched, request_sent);
    }fprintf (fd, "%d %d %d %d %d\n", count_accepted, count_on_wait,
	       count_invalid, count_dispatched, request_sent);
}
