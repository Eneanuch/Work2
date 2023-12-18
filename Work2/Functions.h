#pragma once

using namespace std;

#include <cctype>
#include <cwctype>
#include <locale>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>
#include <sstream>
#include <codecvt>
#include "Definitions.h"

string toLower(string text);
string toUpper(string text);
string toRevert(string text);
string useAction(int action, string text);

string toUpperFirst(string text);

vector<string> splitString(const string& input, char delimiter);
vector<string> splitText(string text, char delimiter);

int limit_digit(int current, int from, int to);
