#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <shellapi.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

// src libs
#include "curl_handler.h"
#include "client_credentials.h"

#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_SIZE 5056

extern Client_credentials client_credentials;

char* build_body(Client_credentials client_credentials) {

    char* body = (char*)malloc(BUFFER_SIZE);
    body[0] = '\0';

    strcat(body, "grant_type=client_credentials&client_id=");
    strcat(body, client_credentials.client_id);
    strcat(body, "&client_secret=");
    strcat(body, client_credentials.client_secret);
    strcat(body, "&scope=");
    strcat(body, client_credentials.scope);
    strcat(body, "&soid=");
    strcat(body, client_credentials.scope);

    return body;
} 

int extract_access_token(char* token_response) {
    
    if (!token_response) {
        return -1;  
    }

    const char* key = "\"access_token\":\"";
    char* start = strstr(token_response, key);
    if (!start) {
        return -1;
    }

    start += strlen(key);  // move to start of token

    char* end = strchr(start, '"');
    if (!end) {
        return -1;
    }

    size_t len = end - start;

    // Move token to beginning of buffer
    memmove(token_response, start, len);
    token_response[len] = '\0';

    return 0;  // success
}

int get_access_token(char* token_buffer) {

    std::string url = std::string(client_credentials.host) + "/oauth/v2/token";
    char* body = build_body(client_credentials);

    curl_post_request(url.c_str(), body, token_buffer);

    if (extract_access_token(token_buffer)) {
        std::cout << "[access_token.h] 'access_token' not found in response" << std::endl;
        return -1;
    }

    return 0;
}