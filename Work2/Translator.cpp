#include "Translator.h"
#include "Functions.h"
#include <iostream>
#include <cctype>
#include <locale>


Translator::Translator() {
    initDefault();

    update();
}

void Translator::update() {
    reverseTranslationTable.clear();
    translationTable.clear();
    unordered_map<string, string> DefaultCopy = this->defaultTable;

    std::vector<std::pair<std::string, std::string>> pairs(DefaultCopy.begin(), DefaultCopy.end());

    std::sort(pairs.begin(), pairs.end(),
        [](const auto& lhs, const auto& rhs) { 
            return lhs.second.length() > rhs.second.length();
        }
    );

    for (const auto& entry : DefaultCopy) {
        cout << entry.first << " - " << entry.second << endl;
        reverseTranslationTable[entry.second] = entry.first;
        reverseTranslationTable[toUpperFirst(entry.second)] = toUpperFirst(entry.first);
    }
    for (const auto& entry : DefaultCopy) {
        translationTable[entry.first] = entry.second;
        translationTable[toUpperFirst(entry.first)] = toUpperFirst(entry.second);
    }
}

void Translator::setTranslationTable(unordered_map<string, string> defaultTable) {
    this->defaultTable = defaultTable;

    this->update();
}

unordered_map<string, string> Translator::getTranslationTable() {
    return this->defaultTable;
}

void Translator::initDefault() {
    defaultTable["щ"] = "sh'";
    defaultTable["дж"] = "j";
    defaultTable["ж"] = "zh";
    defaultTable["ш"] = "sh";
    defaultTable["ю"] = "yu";
    defaultTable["я"] = "ya";
    defaultTable["ч"] = "сh";
    defaultTable["ё"] = "jo";
    defaultTable["э"] = "eh";
    defaultTable["а"] = "a";
    defaultTable["б"] = "b";
    defaultTable["в"] = "v";
    defaultTable["г"] = "g";
    defaultTable["д"] = "d";
    defaultTable["е"] = "e";
    defaultTable["з"] = "z";
    defaultTable["и"] = "i";
    defaultTable["й"] = "y";
    defaultTable["к"] = "k";
    defaultTable["л"] = "l";
    defaultTable["м"] = "m";
    defaultTable["н"] = "n";
    defaultTable["о"] = "o";
    defaultTable["п"] = "p";
    defaultTable["р"] = "r";
    defaultTable["с"] = "s";
    defaultTable["т"] = "t";
    defaultTable["у"] = "u";
    defaultTable["ф"] = "f";
    defaultTable["х"] = "h";
    defaultTable["ц"] = "c";
    defaultTable["ъ"] = "'";
    defaultTable["ы"] = "y";
    defaultTable["ь"] = "'";

}
 
string Translator::findAndReplace(string text, unordered_map<string, string> table) {
    string result = text;

    for (const auto& entry : table) {
        size_t startPos = 0;
        while ((startPos = result.find(entry.first, startPos)) != string::npos) {
            result.replace(startPos, entry.first.size(), entry.second);
            startPos += entry.second.length();
        }
    }

    return result;
}
string Translator::translate(string text) {
    return findAndReplace(text, translationTable);
}

string Translator::retranslate(string text) {
    return findAndReplace(text, reverseTranslationTable);
}