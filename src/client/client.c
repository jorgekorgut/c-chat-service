#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "communication.h"
#include "interface.h"


int initialiser_client() {
    
    initialiser_communication();
    
    initialiser_interface();
    
    fermer_communication();
    
    return 1;
}