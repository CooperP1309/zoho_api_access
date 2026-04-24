#include <windows.h>
#include <string>

// src libs
#include "curl_handler.h"

// TODO: IMPLEMENT POST METHOD CALLS (MAYBE BOOL ARG TO SPECIFY POST OR GET)
int call_api(const char* endpoint, char* access_token, char* result_buffer) {
    
    std::string auth_header = std::string("Authorization: Bearer ") + std::string(access_token);
    std::string url = std::string(client_credentials.host) + endpoint;
    
    std::cout << "[api_caller.h] URL: " << url << std::endl;
    std::cout << "[api_caller.h] Auth Header: " << auth_header << std::endl;

    curl_get_request(url.c_str(), auth_header.c_str(), result_buffer);
    
    return 0;
}