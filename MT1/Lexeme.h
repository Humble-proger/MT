#pragma once
#ifndef LEXEME
#include "Base.h"

struct lexeme
{
	//��� �������������� ��� �������� ���������
	string name;
	string type;
	bool value; // ��������������� �� ��������������
	bool valid;
	//������������
	lexeme() : name(""), type(""), valid(false), value(false) {}
	lexeme(string ind_name) {
		name = ind_name;
		type = "";
		value = false;
		valid = false;
	}
	lexeme(string ind_name, string _type) {
		name = ind_name;
		type = _type;
		value = false;
		valid = false;
	}
	lexeme(string ind_name, string _type, bool val) {
		name = ind_name;
		type = _type;
		value = val;
		valid = false;
	}
	//����������
	//~lexeme();
	bool operator < (const lexeme& b) const {
		return name < b.name;
	}

	bool operator == (const lexeme& b) {
		return name == b.name;
	}
	lexeme& operator = (const lexeme &other) {
		if (this != &other) {
			name = other.name;
			type = other.type;
			valid = other.valid;
		}
		return *this;
	}
};
#endif // !LEXEME