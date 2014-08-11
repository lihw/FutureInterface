#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <enet/enet.h>

ENetAddress  address;
ENetHost   *server;
ENetEvent  event;
ENetPacket      *packet;
ENetHost  *client;
ENetPeer  *peer;

#define HOST "localhost"
#define PORT (6666)
#define BUFFERSIZE (1000)

char buffer[BUFFERSIZE];

int clientMain(int argc, const char *argv[]) 
{
  int connected=0;

  if (enet_initialize() != 0) {
    printf("Could not initialize enet.\n");
    return 0;
  }

  client = enet_host_create(NULL, 1, 2, 5760/8, 1440/8);

  if (client == NULL) {
    printf("Could not create client.\n");
    return 0;
  }

  enet_address_set_host(&address, HOST);
  address.port = PORT;

  peer = enet_host_connect(client, &address, 2, 0);

  if (peer == NULL) {
    printf("Could not connect to server\n");
    return 0;
  }

  if (enet_host_service(client, &event, 1000) > 0 &&
    event.type == ENET_EVENT_TYPE_CONNECT) {

    printf("Connection to %s succeeded.\n", HOST);
    connected++;

    strncpy(buffer, argv[2], BUFFERSIZE);
    packet = enet_packet_create(buffer, strlen(buffer)+1,
      ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);

  } else {
    enet_peer_reset(peer);
    printf("Could not connect to %s.\n", HOST);
    return 0;
  }

  while (1) {
      while (enet_host_service(client, &event, 1000) > 0) {
          switch (event.type) {
          case ENET_EVENT_TYPE_RECEIVE:
              puts( (char*) event.packet->data);
              break;
          case ENET_EVENT_TYPE_DISCONNECT:
              connected=0;
              printf("You have been disconnected.\n");
              return 2;
          }
      }

      if (connected) {
          printf("Input: ");
          gets(buffer);

          if (strlen(buffer) == 0) { continue; }

          if (strncmp("q", buffer, BUFFERSIZE) == 0) {
              connected=0;
              enet_peer_disconnect(peer, 0);
              continue;
          } 

          packet = enet_packet_create(buffer, strlen(buffer)+1,
              ENET_PACKET_FLAG_RELIABLE);
          enet_peer_send(peer, 0, packet);
    }
  }

  enet_deinitialize();
}

int serverMain(int argc, const char* argv[]) 
{
    int  i;

    if (enet_initialize() != 0) {
        printf("Could not initialize enet.");
        return 0;
    }

    address.host = ENET_HOST_ANY;
    address.port = PORT;

    server = enet_host_create(&address, 100, 2, 0, 0);

    if (server == NULL) {
        printf("Could not start server.\n");
        return 0;
    }

    while (1) {
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {

                case ENET_EVENT_TYPE_CONNECT:
                    printf("%d has connected.\n", event.peer->connectID);
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    if (event.peer->data == NULL) {
                        event.peer->data = 
                            malloc(strlen((char*) event.packet->data)+1);
                        strcpy((char*) event.peer->data, (char*) 
                                event.packet->data);

                        sprintf(buffer, "%s has connected\n", 
                                (char*) event.packet->data);
                        printf(buffer, "%s has connected 1\n", 
                                (char*) event.packet->data);
                        packet = enet_packet_create(buffer, 
                                strlen(buffer)+1, 0);
                        enet_host_broadcast(server, 1, packet);
                        enet_host_flush(server);
                    } else {
                        printf("%s: %s (%d)\n", 
                                    (char*)event.peer->data, (char*)event.packet->data, event.packet->dataLength);

                        for (i=0; i < 100; i++) {
                            
                            if (&server->peers[i] != event.peer) {
                                sprintf(buffer, "%s: %s", 
                                        (char*) event.peer->data, (char*)
                                        event.packet->data);
                                packet = enet_packet_create(buffer,
                                        strlen(buffer)+1, 0);
                                enet_peer_send(&server->peers[i], 0,
                                        packet);
                                enet_host_flush(server);
                            } else {

                            }

                        }
                        printf("packet recevied: %s.\n", event.packet->data);
                    }
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    sprintf(buffer, "%s has disconnected.", (char*)
                            event.peer->data);
                    printf("%s has disconnected.\n", (char*)
                            event.peer->data);
                    
                    packet = enet_packet_create(buffer, strlen(buffer)+1, 0);
                    enet_host_broadcast(server, 1, packet);
                    free(event.peer->data);
                    event.peer->data = NULL;
                    break;

                default:
                    printf("Tick tock.\n");
                    break;
            }

        }
    }

    enet_host_destroy(server);
    enet_deinitialize();
}


int main(int argc, const char* argv[])
{
    if (strcmp(argv[1], "server") == 0)
    {
        return serverMain(argc, argv);
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        return clientMain(argc, argv);
    }
    else
    {
        printf("No such kind!");
        return EXIT_FAILURE;
    }
        
    return EXIT_SUCCESS;
}
