#pragma once
#include "ConstTable.h"
#include "VariableHashTable.h"
#include "Token.h"
#include "Error.h"
#define KEYWORDFILENAME "input_files/Types.txt"
#define SEPARATORFILENAME "input_files/Separator.txt"
#define NUMFILENAME "input_files/Numbers.txt"
#define OPERATIONFILENAME "input_files/Operation.txt"
#define LETTERSFILENAME "input_files/Letters.txt"


#ifndef TRANSLATOR

class Translator {
private:
	ConstantTable<string> operation; // 0
	ConstantTable<char> numbers; // 1
	ConstantTable<string> keyword; // 2
	ConstantTable<char> separators; // 3
	ConstantTable<char> letters; // 4
	VariableHashTable identificators; // 5
	VariableHashTable constans; // 6
	queue<Errors> QErrors;
	queue<Token> QToken;

	int num_line = 0;

	void ltrim(string& str) {
		str.erase(0, str.find_first_not_of("\t\n\v\f\r "));
	}
	void rtrim(string& str) {
		str.erase(str.find_last_not_of("\t\n\v\f\r ") + 1);
	}
	void CheckingComments(string& line, bool& continuecheck, bool& nextcheck) {
		size_t ind3 = line.find("*/");
		if (continuecheck) {
			if (ind3 != -1) {
				line.erase(0, ind3 + 2);
				ind3 = line.find("*/");
			}
			else {
				line = "";
				continuecheck = true;
				return;
			}
		}
		size_t ind1 = line.find("//"); size_t ind2 = line.find("/*");
		while (ind1 != -1 || ind2 != -1 || ind3 != -1) {
			if (ind1 != -1 && (ind1 < ind2 || ind2 == -1) && (ind1 < ind3 || ind3 == -1)) {
				line.erase(ind1);
				rtrim(line);
				continuecheck = false;
				return;
			}
			else if (ind2 != -1) {
				if (ind3 == -1) {
					line.erase(ind2);
					rtrim(line);
					nextcheck = true;
					continuecheck = false;
					return;
				}
				else if (ind2 < ind3) {
					line.erase(ind2, ind3 - ind2 + 2);
					ind1 = line.find("//"); ind2 = line.find("/*"); ind3 = line.find("*/");
					continue;
				}
				else {
					QErrors.push(Errors(num_line, ERR_BLOCKCOMMENT));
					line = "";
					continuecheck = false;
					return;
				}
			}
			else {
				QErrors.push(Errors(num_line, ERR_BLOCKCOMMENT));
				line = "";
				continuecheck = false;
				return;
			}
		}
		ltrim(line);
		rtrim(line);
		continuecheck = false;
		return;
	}

	string buffer_to_string(queue<char> buffer) {
		stringstream ss;
		while (!buffer.empty()) {
			ss << buffer.front();
			buffer.pop();
		}
		return ss.str();
	}
	void clear_buffer(queue<char>& buffer) {
		while (!buffer.empty()) buffer.pop();
	}
	Code_Errors ScanLineWithoutComments(string& line) {
		regex re0("^([+-\/*%=]=)$");

		ltrim(line);
		queue<char> buffer;
		queue<char> old_buffer;
		/*
		0 - не выбрано
		1 - число
		2 - ключевое слово
		3 - операция
		4 - идентификатор
		*/
		int old_mode = 0;

		/*
		0 - не выбрано
		1 - число
		2 - ключевое слово
		3 - операция
		*/
		int temp_mode = 0, num_dot = 0;
		string temp_str, dop_str;
		char temp_char;
		for (char ch : line) {
			if (separators.Contains(ch) || ch == ' ') {
				if (ch != ' ')
					separators.Add(ch);
				switch (temp_mode)
				{
				case 0:
					if (ch != ' ')
						QToken.push(Token(3, separators.getIndex(ch)));
					continue;
				case 1:
					if (ch == ' ' || ch == ';') {
						temp_str = buffer_to_string(buffer);
						if (!constans.Contains(temp_str)) {
							if (!constans.Add(temp_str, "const")) return ERR_FAILED_ADD_TABLE;
						}
						QToken.push(Token(6, constans.getIndex(temp_str)));
						if (ch == ';') QToken.push(Token(3, separators.getIndex(ch)));
						temp_mode = 0;
						num_dot = 0;
						old_mode = 1;
						old_buffer.swap(buffer);
						clear_buffer(buffer);
						continue;
					}
					return ERR_INVALID_INT_NUM;
				case 2:
					if (ch == ' ' || ch == ';') {
						temp_str = buffer_to_string(buffer);
						if (keyword.Contains(temp_str)) {
							keyword.Add(temp_str);
							if (old_mode == 2) return ERR_KEYWORD_IN_VAR_NAME;
							QToken.push(Token(2, keyword.getIndex(temp_str)));
							if (ch == ';') QToken.push(Token(3, separators.getIndex(ch)));
							old_mode = temp_mode;
							temp_mode = 0;
							old_buffer.swap(buffer);
							clear_buffer(buffer);
							continue;
						}
						else {
							if (!identificators.Contains(temp_str)) {
								if (old_mode == 2) {
									dop_str = buffer_to_string(old_buffer);
									if (!identificators.Add(temp_str, dop_str)) return ERR_FAILED_ADD_TABLE;
								}
								else return ERR_UNKNOWN_VARIABLE;
							}
							else if (old_mode == 2) return ERR_VAR_OVERRIDES;
							QToken.push(Token(5, identificators.getIndex(temp_str)));
							if (ch == ';') QToken.push(Token(3, separators.getIndex(ch)));
							old_mode = 4;
							temp_mode = 0;
							old_buffer.swap(buffer);
							clear_buffer(buffer);
							continue;
						}
					}
					return ERR_INVALID_NAME;
				case 3:
					if (ch == ' ') {
						temp_str = buffer_to_string(buffer);
						if (operation.Contains(temp_str)) {
							operation.Add(temp_str);
							if (old_mode != 4 && old_mode != 1) return ERR_INCORRECT_USE_OPERATION;
							QToken.push(Token(0, operation.getIndex(temp_str)));
							old_mode = 3;
							old_buffer.swap(buffer);
							clear_buffer(buffer);
							temp_mode = 0;
							continue;
						}
					}
					return ERR_INVALID_OPERATION;
				default:
					break;
				}
			}
			else if (numbers.Contains(ch)) {
				switch (temp_mode)
				{
				case 0:
					buffer.push(ch);
					temp_mode = 1;
					continue;
				case 1:
					temp_char = buffer.front();
					if (temp_char != '0') {
						numbers.Add(ch);
						buffer.push(ch);
						continue;
					}
					else {
						return ERR_INVALID_INT_NUM;
					}
				case 2:
					numbers.Add(ch);
					buffer.push(ch);
					continue;
				case 3:
					return ERR_INVALID_SYMBOL;
				default:
					break;
				}
			}
			else if (letters.Contains(ch)) {
				letters.Add(ch);
				switch (temp_mode)
				{
				case 0:
					buffer.push(ch);
					temp_mode = 2;
					continue;
				case 1:
					if (old_mode == 2) return ERR_INVALID_NAME;
					return ERR_INVALID_INT_NUM;
				case 2:
					buffer.push(ch);
					continue;
				case 3:
					return ERR_INVALID_OPERATION;
				default:
					break;
				}
			}
			else {
				if (temp_mode == 0) {
					if (ch == '=') {
						if (old_mode != 4) return ERR_INVALID_ASSIGNMENT_OPERATION;
						dop_str = buffer_to_string(old_buffer);
						if (!identificators.SetValue(dop_str, true)) return ERR_FAILED_ADD_TABLE;
						old_mode = 3;
						clear_buffer(old_buffer);
						continue;
					}
					temp_mode = 3;
					buffer.push(ch);
					continue;
				}
				else {
					if (temp_mode != 3) {
						if (ch == '.' && temp_mode == 1) {
							if (num_dot == 0) {
								buffer.push(ch);
								num_dot++;
								continue;
							}
							else {
								return ERR_INVALID_FLOAT_NUM;
							}
						}
						return ERR_INVALID_NAME;
					}
					else {
						buffer.push(ch);
						temp_str = buffer_to_string(buffer);
						if (operation.Contains(temp_str)) {
							operation.Add(temp_str);
							QToken.push(Token(0, operation.getIndex(temp_str)));
							clear_buffer(buffer);
							temp_mode = 0;
							continue;
						}
						else if (regex_match(temp_str, re0)) {
							if (old_mode == 4) {
								dop_str = buffer_to_string(old_buffer);
								if (identificators.getElement(dop_str).value) {
									old_mode = 3;
									clear_buffer(old_buffer);
									clear_buffer(buffer);
									temp_mode = 0;
									continue;
								}
								else return ERR_PERFORMING_OPERATION_UNINITIALIZED_VAR;
							}
							else return ERR_INCORRECT_USE_OPERATION;
						}
						else return ERR_INVALID_OPERATION;
					}
				}
			}
		}
		return NO_ERROR;
	}

public:
	Translator() {
		if (!operation.ReadKeysFile(OPERATIONFILENAME)) QErrors.push(Errors(0, FATALERR_FAILLEDREADFILE));
		if (!numbers.ReadKeysFile(NUMFILENAME)) QErrors.push(Errors(0, FATALERR_FAILLEDREADFILE));
		if (!keyword.ReadKeysFile(KEYWORDFILENAME)) QErrors.push(Errors(0, FATALERR_FAILLEDREADFILE));
		if (!separators.ReadKeysFile(SEPARATORFILENAME)) QErrors.push(Errors(0, FATALERR_FAILLEDREADFILE));
		if (!letters.ReadKeysFile(LETTERSFILENAME)) QErrors.push(Errors(0, FATALERR_FAILLEDREADFILE));
	}

	void PrintToken() {
		cout << "TABLE" << "\t" << "NUM_TABLE" << endl;
		queue<Token> copy_token = QToken;
		Token temp_token;
		while (!copy_token.empty()) {
			temp_token = copy_token.front();
			copy_token.pop();
			cout << temp_token.NUM_TABLE << "\t" << temp_token.INDEX << endl;
		}
	}
	void PrintErrors() {
		cout << "NUM LINE" << "\t" << "CODE ERROR" << endl;
		queue<Errors> copy_Error = QErrors;
		Errors temp_error;
		while (!copy_Error.empty()) {
			temp_error = copy_Error.front();
			copy_Error.pop();
			cout << temp_error.NUM_LINE << "\t" << temp_error.CODE << endl;
		}
	}
	void ScanningFile(const string& NameFile) {
		if (operation.empty() || numbers.empty() || keyword.empty() || letters.empty() || separators.empty()) return;

		ifstream ScanFile(NameFile, ios::in);
		if (!ScanFile.is_open()) {
			QErrors.push(Errors(0, FATALERR_FAILLEDREADSCANFILE));
			return;
		}

		string line;
		bool check = false, nextcheck = false;
		Code_Errors code;
		while (getline(ScanFile, line)) {
			if (nextcheck) {
				check = true;
				nextcheck = false;
			}
			CheckingComments(line, check, nextcheck);
			if (check == true || line.empty()) {
				num_line++;
				continue;
			}
			if ((code = ScanLineWithoutComments(line)) != NO_ERROR) QErrors.push(Errors(num_line, code));
			num_line++;
		}
		ScanFile.close();
		if (check || nextcheck) QErrors.push(Errors(num_line, ERR_BLOCKCOMMENT));
	}

	bool outFileToken(ofstream& FileOut) {
		if (!FileOut.is_open()) {
			cerr << "Error! File not opened!" << endl;
			return false;
		}
		queue<Token> CQtoken = QToken;
		Token t_token;
		while (!CQtoken.empty()) {
			t_token = CQtoken.front();
			CQtoken.pop();
			FileOut << t_token.NUM_TABLE << "\t" << t_token.INDEX << endl;
		}
		return true;
	}
	bool outFileErrors(ofstream& FileOut) {
		if (!FileOut.is_open()) {
			cerr << "Error! File not opened!" << endl;
			return false;
		}
		queue<Errors> CQErrors = QErrors;
		Errors t_error;
		while (!CQErrors.empty()) {
			t_error = CQErrors.front();
			CQErrors.pop();
			FileOut << t_error.info() << endl;
		}
		return true;
	}
	bool outFileIC(ofstream& FileOut) {
		return identificators.outFile(FileOut);
	}
	bool outFileC(ofstream& FileOut) {
		return constans.outFile(FileOut);
	}
	bool outConstTable(ofstream& FileOut) {
		if (!FileOut.is_open()) {
			cerr << "Error! File not opened!" << endl;
			return false;
		}
		letters.outfile(FileOut);
		keyword.outfile(FileOut);
		operation.outfile(FileOut);
		separators.outfile(FileOut);
		numbers.outfile(FileOut);
		return true;
	}
};

#endif // !TRANSLATOR
