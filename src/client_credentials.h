#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <algorithm>

struct Client_credentials {
    char* client_id;
    char* client_secret;
    char* scope;
    char* host;
    char* zsoid;
};

extern Client_credentials client_credentials;

std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace((unsigned char)s[start])) start++;

    size_t end = s.size();
    while (end > start && std::isspace((unsigned char)s[end - 1])) end--;

    return s.substr(start, end - start);
}

std::string strip_quotes(const std::string& s) {
    if (s.size() >= 2 && s.front() == '\'' && s.back() == '\'')
        return s.substr(1, s.size() - 2);
    return s;
}

int extract_client_credentials(const char* credentials_path, Client_credentials &credentials) {

    std::ifstream file(credentials_path);
    if (!file.is_open()) return -1;

    std::string line;

    while (std::getline(file, line)) {
        // remove Windows CR if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        line = trim(line);
        if (line.empty()) continue;

        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key = trim(line.substr(0, eq));
        std::string value = trim(line.substr(eq + 1));

        value = strip_quotes(value);

        if (key == "client_id") {
            credentials.client_id = _strdup(value.c_str());
        }
        else if (key == "client_secret") {
            credentials.client_secret = _strdup(value.c_str());
        }
        else if (key == "scope") {
            credentials.scope = _strdup(value.c_str());
        }
        else if (key == "host") {
            credentials.host = _strdup(value.c_str());
        }
        else if (key == "zsoid") {
            credentials.zsoid = _strdup(value.c_str());
        }
    }

    return 0;
}

const char* get_credentials_path() {
    
    static std::string path;  // persists after function returns

    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);

    std::string full_path(buffer);

    size_t pos = full_path.find_last_of("\\/");
    if (pos != std::string::npos) {
        path = full_path.substr(0, pos) + "\\client_credentials.txt";
    } else {
        path = "client_credentials.txt";  // fallback
    }

    return path.c_str();
}

int init_client_credentials() {
    
    const char* credentials_path = get_credentials_path();

    if (extract_client_credentials(credentials_path, client_credentials)) {
        std::cerr << "Failed to extract client credentials from file: " << credentials_path << std::endl;
        return -1;
    }

    return 0;
}