#pragma once
#ifndef TOKEN

class Token {

public:
	int NUM_TABLE = 0;
	int INDEX = 0;
	/*
	0 - ��������
	1 - �����
	2 - �������� �����
	3 - �������������� �������
	4 - ���������� �������
	5 - ��������������
	6 - ���������
	*/
	Token() {}
	Token(int num_table, int index) {
		NUM_TABLE = num_table;
		INDEX = index;
	}
};


#endif // !TOKEN
