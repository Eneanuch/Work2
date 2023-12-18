#pragma once

using namespace std;

#include <unordered_map>
#include <iostream>
#include <fstream>


class Translator {
public:
	Translator();

	string translate(string text);

	string retranslate(string text);

	void setTranslationTable(unordered_map<string, string> translationTable);
	unordered_map<string, string> getTranslationTable();
	void update();

private:

	unordered_map<string, string> defaultTable;
	unordered_map<string, string> translationTable;
	unordered_map<string, string> reverseTranslationTable;


	string findAndReplace(string text, unordered_map<string, string> table);

	void initDefault();

	// void loadFile();

};