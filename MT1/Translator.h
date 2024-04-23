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
#define PARSERFILENAME "input_files/parse_table.txt"
#define NAMEFILEERRORS "out_files/error.txt"
#define NAMEFILETOKEN "out_files/token.txt"
#define NAMEFILEIDENTIFICATORS "out_files/identificators.txt"
#define NAMEFILECONSTANS "out_files/constans.txt"
#define NAMEFILECONSTTABLE "out_files/consttables.txt"
#define NAMEFILEPOSTFIX "out_files/postfix.txt"

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

	struct table_parser_elem {
		vector<string> terminal;
		int jump = 0;
		bool accept = false;
		bool stack_ = false;
		bool return_ = false;
		bool err = false;
	};
	vector<table_parser_elem> parser_table;
	vector<string> postfix_buffer;

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

	bool is_number(const std::string& s)
	{
		if (s == "-") return false;
		return !s.empty() && (s.find_first_not_of("-0123456789") == s.npos);
	}
	
	bool fill_parser_table() {
		ifstream ParserFile(PARSERFILENAME, ios::in);
		if (!ParserFile.is_open()) return false;
		string temp;
		int num;
		getline(ParserFile, temp);
		while (!ParserFile.eof()) {
			table_parser_elem el;
			ParserFile >> temp;
			while (!is_number(temp)) {
				el.terminal.push_back(temp);
				ParserFile >> temp;
			}
			el.jump = stoi(temp) - 1;
			ParserFile >> temp;
			el.accept = stoi(temp) == 1;
			ParserFile >> temp;
			el.stack_ = stoi(temp) == 1;
			ParserFile >> temp;
			el.return_ = stoi(temp) == 1;
			ParserFile >> temp;
			el.err = stoi(temp) == 1;
			parser_table.push_back(el);
		}
		ParserFile.close();
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
	Code_Errors LexicalLineScanWithoutComments(string& line) {
		ltrim(line);
		queue<char> buffer;
		/*
		0 - не выбрано
		1 - число
		2 - ключевое слово
		3 - операция
		4 - идентификатор
		*/

		/*
		0 - не выбрано
		1 - число
		2 - ключевое слово
		3 - операция
		*/
		int temp_mode = 0, num_dot = 0;
		string temp_str;
		char temp_char;
		for (char ch : line) {
			if (ch == ' ' || ch == ';') {
				switch (temp_mode)
				{
				case 0:
					if (ch == ';')
						QToken.push(Token(3, separators.getIndex(ch)));
					continue;
				case 1:
					temp_str = buffer_to_string(buffer);
					if (!constans.Contains(temp_str)) {
						if (!constans.Add(temp_str, "const")) return ERR_FAILED_ADD_TABLE;
					}
					QToken.push(Token(6, constans.getIndex(temp_str)));
					if (ch == ';') QToken.push(Token(3, separators.getIndex(ch)));
					temp_mode = 0;
					num_dot = 0;
					clear_buffer(buffer);
					continue;
				case 2:
					temp_str = buffer_to_string(buffer);
					if (keyword.Contains(temp_str)) {
						keyword.Add(temp_str);
						QToken.push(Token(2, keyword.getIndex(temp_str)));
						if (ch == ';') QToken.push(Token(3, separators.getIndex(ch)));
						temp_mode = 0;
						clear_buffer(buffer);
						continue;
					}
					else {
						if (!identificators.Contains(temp_str)) {
								if (!identificators.Add(temp_str)) return ERR_FAILED_ADD_TABLE;
						}
						QToken.push(Token(5, identificators.getIndex(temp_str)));
						if (ch == ';') QToken.push(Token(3, separators.getIndex(ch)));
						temp_mode = 0;
						clear_buffer(buffer);
						continue;
					}
				case 3:
					if (ch == ' ') {
						temp_str = buffer_to_string(buffer);
						if (operation.Contains(temp_str)) {
							operation.Add(temp_str);
							QToken.push(Token(0, operation.getIndex(temp_str)));
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
			else if (separators.Contains(ch)) {
				if (temp_mode == 2) {
					temp_str = buffer_to_string(buffer);
					if (keyword.Contains(temp_str)) {
						keyword.Add(temp_str);
						QToken.push(Token(2, keyword.getIndex(temp_str)));
						QToken.push(Token(3, separators.getIndex(ch)));
						temp_mode = 0;
						clear_buffer(buffer);
						continue;
					}
					else {
						if (!identificators.Contains(temp_str)) {
							if (!identificators.Add(temp_str)) return ERR_FAILED_ADD_TABLE;
						}
						QToken.push(Token(5, identificators.getIndex(temp_str)));
						QToken.push(Token(3, separators.getIndex(ch)));
						temp_mode = 0;
						clear_buffer(buffer);
						continue;
					}
				}
				separators.Add(ch);
				QToken.push(Token(3, separators.getIndex(ch)));
				continue;
				
			}
			else if (numbers.Contains(ch)) {
				switch (temp_mode)
				{
				case 0:
					buffer.push(ch);
					temp_mode = 1;
					continue;
				case 1:
					if (buffer.size() == 1) {
						temp_char = buffer.front();
						if (temp_char != '0') {
							numbers.Add(ch);
							buffer.push(ch);
							continue;
						}
						else {
							return ERR_INVALID_INT_NUM;
						}
					}
					else {
						numbers.Add(ch);
						buffer.push(ch);
						continue;
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
					if (operation.Contains(to_string(ch))) {
						operation.Add(to_string(ch));
						QToken.push(Token(0, operation.getIndex(to_string(ch))));
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
						else {
							return ERR_INVALID_OPERATION;
						}
					}
				}
			}
		}
		return NO_ERROR;
	}

	string get_token_info(Token& val) {
		switch (val.NUM_TABLE)
		{
		case 0:
			return operation.getElembyIndex(val.INDEX).first;
		case 1:
			return numbers.getElembyIndex(val.INDEX).first + string();
		case 2:
			return keyword.getElembyIndex(val.INDEX).first;
		case 3:
			return separators.getElembyIndex(val.INDEX).first + string();
		case 4:
			return letters.getElembyIndex(val.INDEX).first + string();
		case 5:
			return identificators.getElemIndex(val.INDEX).name;
		case 6:
			return constans.getElemIndex(val.INDEX).name;
		default:
			return "";
		}
	}

	Errors SentenceAnalysis() {
		Token fir_token, sec_token;
		stack<int> parser_stack;
		parser_stack.push(0);
		int row = 0;
		string type;
		vector<Token> infix_expr;
		bool postfix = false;
		bool have_type = false;
		queue<Token> CQToken = QToken;
		string name_token;
		bool flag_err = false;

		fir_token = CQToken.front(); CQToken.pop();
		bool end_flag = CQToken.empty();
		sec_token = CQToken.front(); CQToken.pop();
		while (!flag_err) {
			name_token = get_token_info(fir_token);
			if (fir_token.NUM_TABLE == 5 && name_token != "main") name_token = "var";
			if (fir_token.NUM_TABLE == 6) name_token = "const";

			bool is_terminal = false;
			for (int i = 0; i < parser_table[row].terminal.size() && !is_terminal; i++) {
				if (parser_table[row].terminal[i] == name_token) is_terminal = true;
			}
			if (is_terminal) {
				if (end_flag && parser_table[row].accept) break;
				if (parser_table[row].stack_)
					parser_stack.push(row + 1);
				if (parser_table[row].accept) {
					if ((name_token == "var" || name_token == "const") && get_token_info(sec_token) == "=")
						postfix = true;
					if (!have_type && name_token == "var" && identificators.getElemIndex(fir_token.INDEX).type == "") {
						return Errors(ERR_UNKNOWN_VARIABLE, parser_table[row].terminal, get_token_info(fir_token));
					}
					if (postfix)
						infix_expr.push_back(fir_token);
					if (name_token == ";" || name_token == ",") {
						if (!make_postfix(infix_expr))
							flag_err = true;
						infix_expr.clear();
						postfix = false;
					}
					if (name_token == ";")
						have_type = false;
					if (name_token == "int" || name_token == "float" || name_token == "struct") {
						have_type = true;
						type = name_token;
					}
					if (name_token == "var" && have_type && (row == 54 || row == 31 || row == 18))
						identificators.SetType(get_token_info(fir_token), type);
					end_flag = CQToken.empty();
					fir_token = sec_token;
					if (!end_flag) {
						sec_token = CQToken.front(); CQToken.pop();
					}
				}
				if (parser_table[row].return_) {
					if (!parser_stack.empty()) {
						row = parser_stack.top();
						parser_stack.pop();
					}
					else {
						return Errors(ERR_PARSESTACK);
					}
				}
				else {
					row = parser_table[row].jump;
				}
			}
			else {
				if (parser_table[row].err) {
					return Errors(ERR_UNKNOWN_TERMINAL, parser_table[row].terminal, get_token_info(fir_token));
				}
				else {
					row++;
				}
			}
		
		}
		if (!flag_err && parser_stack.size() > 1) {
			return Errors(ERR_PARSESTACK_NO_EMPTY, parser_stack);
		}
		return Errors(NO_ERROR);
	}
	bool make_postfix(vector<Token> v) {
		stack<string> stack_temp;
		bool error_flag = false;
		int index = 0;
		while (index < v.size() && !error_flag) {
			int i;
			for (i = index; i < v.size() && !error_flag && get_token_info(v[i]) != ";" && get_token_info(v[i]) != ","; i++) {
				string token_text = get_token_info(v[i]);
				if (v[i].NUM_TABLE == 5 || v[i].NUM_TABLE == 6) {
					postfix_buffer.push_back(token_text);
				}
				else if (token_text == "(") {
					stack_temp.push(token_text);
				}
				else if (token_text == ")")
				{
					while (!stack_temp.empty() && stack_temp.top() != "(")
					{
						string tmpstr = stack_temp.top();
						postfix_buffer.push_back(tmpstr);
						stack_temp.pop();
					}
					if (stack_temp.empty())
					{
						// Syntax Error: Unexpected ")"
						QErrors.push(Errors(ERR_INVALID_SYMBOL));
						error_flag = true;
					}
					else
					{
						stack_temp.pop();
					}
				}
				else if (v[i].NUM_TABLE == 0)
				{
					while (!stack_temp.empty() && priority_le(token_text, stack_temp.top()))
					{
						string tmpstr = stack_temp.top();
						postfix_buffer.push_back(tmpstr);
						stack_temp.pop();
					}
					stack_temp.push(token_text);
				}
			}
			if (error_flag)
			{
				postfix_buffer.clear();
				return false;
			}
			else
			{
				while (!stack_temp.empty() &&
					stack_temp.top() != "(" && stack_temp.top() != ")")
				{
					string tmpstr = stack_temp.top();
					postfix_buffer.push_back(tmpstr);
					stack_temp.pop();
				}
				if (!stack_temp.empty())
				{
					//Syntax Error: Brackets balance error!
					QErrors.push(Errors(ERR_BRACKET));
					error_flag = true;
				}
			}
			if (error_flag)
			{
				postfix_buffer.clear();
				return false;
			}
			index = i + 1;
			postfix_buffer.push_back(";");
		}
		return true;
	}

	// Печать постфиксной записи в файл и на экран
	void postfix_print(const string& file_tree)
	{
		ofstream out(file_tree);
		for (int i = 0; i < postfix_buffer.size(); i++)
		{
			out << postfix_buffer[i] << " ";
		}
		out.close();
	}
	bool priority_le(string what, string with_what)
	{
		int pw = 0, pww = 0;
		if (what == "=" || what == "+=" || what == "-=") pw = 10;
		else if (what == "|") pw = 20;
		else if (what == "&") pw = 30;
		else if (what == "<" || what == ">") pw = 40;
		else if (what == "<<" || what == ">>") pw = 50;
		else if (what == "+" || what == "-") pw = 60;
		else pw = 70;
		if (with_what == "=" || with_what == "+=" || with_what == "-=") pww = 10;
		else if (with_what == "|") pww = 20;
		else if (with_what == "&") pww = 30;
		else if (with_what == "<" || with_what == ">") pww = 40;
		else if (with_what == "<<" || with_what == ">>") pww = 50;
		else if (with_what == "+" || with_what == "-") pww = 60;
		else pww = 70;
		if (pw <= pww) return true;
		return false;
	};

public:
	Translator() {
		if (!operation.ReadKeysFile(OPERATIONFILENAME)) QErrors.push(Errors(0, FATALERR_FAILLEDREADFILE));
		if (!numbers.ReadKeysFile(NUMFILENAME)) QErrors.push(Errors(0, FATALERR_FAILLEDREADFILE));
		if (!keyword.ReadKeysFile(KEYWORDFILENAME)) QErrors.push(Errors(0, FATALERR_FAILLEDREADFILE));
		if (!separators.ReadKeysFile(SEPARATORFILENAME)) QErrors.push(Errors(0, FATALERR_FAILLEDREADFILE));
		if (!letters.ReadKeysFile(LETTERSFILENAME)) QErrors.push(Errors(0, FATALERR_FAILLEDREADFILE));
		if (!fill_parser_table()) QErrors.push(Errors(FATALERR_FAILLEDREADFILE));
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
		if (operation.empty() || numbers.empty() || keyword.empty() || letters.empty() || separators.empty() || parser_table.empty()) return;

		ifstream ScanFile(NameFile, ios::in);
		if (!ScanFile.is_open()) {
			QErrors.push(Errors(0, FATALERR_FAILLEDREADSCANFILE));
			return;
		}

		string line;
		bool flag_err = false;
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
			if ((code = LexicalLineScanWithoutComments(line)) != NO_ERROR) { 
				QErrors.push(Errors(num_line, code));
				flag_err = true;
				break;
			}
			num_line++;
		}
		ScanFile.close();
		if (!flag_err) {
			Errors err = SentenceAnalysis();
			if (err.CODE == NO_ERROR) {
				postfix_print(NAMEFILEPOSTFIX);
			}
			else {
				QErrors.push(err);
			}
		}
		if (check || nextcheck) QErrors.push(Errors(num_line, ERR_BLOCKCOMMENT));
		if (!outFileToken()) {
			cerr << NAMEFILETOKEN << " file not opened!";
		}
		if (!outFileErrors()) {
			cerr << NAMEFILEERRORS << " file not opened!";
			return ;
		}
		if (!outFileIC()) {
			cerr << NAMEFILEIDENTIFICATORS << " file not opened!";
			return ;
		}
		if (!outFileC()) {
			cerr << NAMEFILECONSTANS << " file not opened!";
			return ;
		}
		if (!outConstTable()) {
			cerr << NAMEFILECONSTTABLE << " file not opened!";
			return ;
		}
	}



	bool outFileToken() {
		ofstream FileOut(NAMEFILETOKEN);
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
		FileOut.close();
		return true;
	}
	bool outFileErrors() {
		ofstream FileOut(NAMEFILEERRORS);
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
		FileOut.close();
		return true;
	}
	bool outFileIC() {
		ofstream FileOut(NAMEFILEIDENTIFICATORS);
		return identificators.outFile(FileOut);
		FileOut.close();
	}
	bool outFileC() {
		ofstream FileOut(NAMEFILECONSTANS);
		return constans.outFile(FileOut);
		FileOut.close();
	}
	bool outConstTable() {
		ofstream FileOut(NAMEFILECONSTTABLE);
		if (!FileOut.is_open()) {
			cerr << "Error! File not opened!" << endl;
			return false;
		}
		letters.outfile(FileOut);
		keyword.outfile(FileOut);
		operation.outfile(FileOut);
		separators.outfile(FileOut);
		numbers.outfile(FileOut);
		FileOut.close();
		return true;
	}
};

#endif // !TRANSLATOR
