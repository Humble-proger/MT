#pragma once
#ifndef TOKEN

class Token {

public:
	int NUM_TABLE = 0;
	int INDEX = 0;
	/*
	0 - операции
	1 - числа
	2 - ключевые слова
	3 - разделительные символы
	4 - допустимые символы
	5 - идентификаторы
	6 - константы
	*/
	Token() {}
	Token(int num_table, int index) {
		NUM_TABLE = num_table;
		INDEX = index;
	}
	bool operator == (const Token& b) {
		return NUM_TABLE == b.NUM_TABLE && INDEX == b.INDEX;
	}
};


#endif // !TOKEN
