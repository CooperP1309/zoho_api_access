#pragma once

#include <iostream>
#include <string>
#include <winsock2.h>   // For Windows socket initialization
#include <ws2tcpip.h>
#include <curl/curl.h>  // libcurl header

#pragma comment(lib, "Ws2_32.lib")  // Winsock library
#pragma comment(lib, "libcurl.lib") // Link against libcurl

// Callback function to write received data into a std::string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Initialize Winsock (optional but good practice on Windows)
bool initWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return false;
    }
    return true;
}

void cleanupWinsock() {
    WSACleanup();
}

/*
 *  Sends a Curl Request
 *  
 *  If no body is present, will send a GET request. E.g: curl_send_request("https://www.example.com", "");
 *  Otherwise, will prepare a POST. TODO: Make two functions. Because empty body posts are often required
 */
int curl_post_request(const char* target_host, char* body, char* result_buffer) {

    std::cout << "[curl_handler.h] Target Host: " << target_host << std::endl;
    std::cout << "[curl_handler.h] Posting Body: " << body << std::endl;

    // Step 1: Initialize Winsock
    if (!initWinsock()) {
        return 1;
    }

    // Step 2: Initialize libcurl
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        std::string readBuffer;

        // Step 3: Set libcurl options
        curl_easy_setopt(curl, CURLOPT_URL, target_host);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // build headers first
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // check for provided body: Case for POST request
        if (body[0] != '\0') {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
        }

        // Optional: Set timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        // Step 4: Perform the request
        res = curl_easy_perform(curl);

        // Step 5: Error handling
        if (res != CURLE_OK) {
            std::cerr << "[curl_handler.h] curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "[curl_handler.h] HTTP Response:\n\t" << readBuffer << std::endl << std::endl;
        }

        // write to result buffer
        strcpy(result_buffer, readBuffer.c_str());

        // Step 6: Cleanup
        curl_easy_cleanup(curl);
    } else {
        std::cout << "Failed to initialize libcurl.\n";
    }

    curl_global_cleanup();
    cleanupWinsock();

    return 0;
}

int curl_get_request(const char* target_host, const char* auth_header, char* result_buffer) {

    std::cout << "[curl_handler.h] Target Host: " << target_host << std::endl;
    std::cout << "[curl_handler.h] GET with headers: " << auth_header << std::endl;

    // Step 1: Initialize Winsock
    if (!initWinsock()) {
        return 1;
    }

    // Step 2: Initialize libcurl
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        std::string readBuffer;

        // Step 3: Set libcurl options
        curl_easy_setopt(curl, CURLOPT_URL, target_host);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // build headers first
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, auth_header);
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Optional: Set timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        // Step 4: Perform the request
        res = curl_easy_perform(curl);

        // Step 5: Error handling
        if (res != CURLE_OK) {
            std::cerr << "[curl_handler.h] curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "[curl_handler.h] HTTP Response Recieved:\n\n" << std::endl;
        }

        // write to result buffer
        strcpy(result_buffer, readBuffer.c_str());

        // Step 6: Cleanup
        curl_easy_cleanup(curl);
    } else {
        std::cout << "Failed to initialize libcurl.\n";
    }

    curl_global_cleanup();
    cleanupWinsock();

    return 0;
}