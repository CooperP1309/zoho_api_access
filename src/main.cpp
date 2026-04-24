#include <iostream>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>

// src libs
#include "client_credentials.h"
#include "access_token.h"
#include "api_caller.h"

// global client credentials struct
Client_credentials client_credentials;

int main() {

    if (init_client_credentials() != 0) {
        std::cerr << "Failed to initialize client credentials." << std::endl;
        return 1;
    }

    std::cout << "[MAIN] Using client app credentials..." << std::endl << std::endl; 
    std::cout << "\tClient ID: " << client_credentials.client_id << std::endl;
    std::cout << "\tClient Secret: " << client_credentials.client_secret << std::endl;
    std::cout << "\tScope: " << client_credentials.scope << std::endl;
    std::cout << "\tHost: " << client_credentials.host << std::endl << std::endl;
    std::cout << "\tzsoid: " << client_credentials.zsoid << std::endl << std::endl;

    char* access_token = (char*)malloc(BUFFER_SIZE);
    access_token[0] = '\n';

    if (get_access_token(access_token)) {
        return 0;
    }

    std::cout << "[MAIN] Access Token: " << access_token << std::endl << std::endl;

    free(access_token);

    return 0;
}