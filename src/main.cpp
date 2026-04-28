#include <iostream>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include <nlohmann/json.hpp>

// src libs
#include "client_credentials.h"
#include "access_token.h"
#include "api_caller.h"

using json = nlohmann::json;

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

    /*----- EXAMPLE API CALL -----*/
    std::string api_response;

    call_api("/crm/v8/Contacts?fields=Last_Name,Email,Record_Status__s,Converted__s,Converted_Date_Time&converted=true&per_page=5", access_token, api_response);

    std::cout << "[MAIN] API Response:\n\n" << api_response << "\n\n";

    json response_json = json::parse(api_response);
    std::cout << std::endl << "[MAIN] 5 Contacts in Zoho CRM: " << std::endl << response_json.dump(4) << std::endl << std::endl;

    free(access_token);

    return 0;
}