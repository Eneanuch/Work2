#include "TextEdit.h"


string TextEdit::textChange(int action) {
	if (type == FullText) {
		return useAction(action, text);
	}
	else {
		vector<string> splitedInputText = splitText(text, delimiters[type]);

		
		if (DEBUG) {
			cout << "Splited Text" << endl;
			for (int i = 0; i < splitedInputText.size(); i++) {
				cout << "Part " << i << endl;
				cout << splitedInputText[i] << endl;
			}
		}

		from = limit_digit(from, 0, splitedInputText.size());
		to = limit_digit(to, 0, splitedInputText.size());

		int rto = max(from, to);
		int rfrom = min(from, to);

		for (int i = rfrom; i < rto; i++) {
			splitedInputText[i] = useAction(action, splitedInputText[i]);
		}

		if (type != Letters) {
			for (int i = 0; i < splitedInputText.size(); i++) {
				splitedInputText[i] = splitedInputText[i] + delimiters[type];
			}
		}

		string result = accumulate(splitedInputText.begin(), splitedInputText.end(), string());
		return result;
	}
}

void TextEdit::setInputText(const string& inputText) {
	text = inputText;
}

void TextEdit::setType(int type) {
	this->type = type;
}

void TextEdit::setFrom(int from) {
	this->from = from;
}

void TextEdit::setTo(int to) {
	this->to = to;
}

Translator TextEdit::getTranslator() {
	return this->translator;
}

string TextEdit::useAction(int action, string text) {
	switch (action) {
	case LOWER:
		return toLower(text);
	case UPPER:
		return toUpper(text);
	case REVERT:
		return toRevert(text);
	case KIRILLICA:
		return translator.translate(text);
	case LATINICA:
		return translator.retranslate(text);
	}
}

void TextEdit::setTranslator(Translator translato) {
	this->translator = translato;
}