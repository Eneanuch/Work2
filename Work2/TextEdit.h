#pragma once

#include "Definitions.h"
#include "Functions.h"
#include "Translator.h"

class TextEdit {

public:

    string textChange(int action);

    void setTranslator(Translator translator);

    void setInputText(const string& inputText);

    void setType(int type);

    void setFrom(int from);

    void setTo(int to);

    Translator getTranslator();

private:
    Translator translator;
    string text;

    int type = 0;
    int from = 0;
    int to = 0;

    char delimiters[6] = { ' ', '\0', ' ', '\r', '.', '\t'};

    string useAction(int action, string text);

};

