#include "common.h"

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        if (token == "" )
            continue;
        tokens.push_back(token);
    }
    return tokens;
}