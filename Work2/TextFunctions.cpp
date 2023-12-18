#include "Functions.h"


string toLower(string text) {
    string result = text;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return tolower(c, locale(""));
        });
    return result;
}

string toUpper(string text) {
    string result = text;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return toupper(c, locale(""));
        });
    return result;
}

string toRevert(string text) {
    string result = text;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return isupper(c, locale("")) ? tolower(c, locale("")) : toupper(c, locale(""));
        });
    return result;
}


vector<string> splitString(const string& input, char delimiter) {
    vector<string> tokens;
    istringstream tokenStream(input);
    string token;

    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

vector<string> splitText(string text, char delimiter) {
    if (delimiter == '\0') {
        vector<string> stringVector;
        for (char c : text) {
            stringVector.push_back(string(1, c));
        }

        return stringVector;
    }

    return splitString(text, delimiter);
}

int limit_digit(int current, int from, int to) {
    if (current < from) {
        return from;
    }
    if (current > to) {
        return to;
    }

    return current;
}


string toUpperFirst(string str) {
    if (str.empty()) {
        return str; 
    }

    string result = str;
    result[0] = toupper(result[0], locale(""));

    return result;
}