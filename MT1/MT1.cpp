#include "ConstTable.h"
#include "VariableHashTable.h"

ConstantTable CTable;
VariableHashTable HTable;

bool ScanningLine(const string& line, int num_line = -1) {
	if (line.size() == 0 || line[line.size() - 1] != ';') return true;
	stringstream ss(line);
	regex re1("^([a-zA-Z_])|([a-zA-Z_][a-zA-Z0-9_]+)$");
	regex re0("^([+-\/*%=]=)$");
	string type, name, value;
	ss >> type;
	if (type[type.size() - 1] == ';') return true;
	if (type.size() > 2 && CTable.Contains(type)) {
		ss >> name;
		if (name[name.size() - 1] == ';') {
			name.replace(name.size() - 1, 1, "");
			if (HTable.Contains(name)) {
				cerr << "Error! Redefinition of type " << name;
				if (num_line != -1)
					cerr << " on line " << num_line << endl;
				else
					cerr << endl;
				return false;
			}
			if (regex_match(name, re1))
				HTable.Add(name, type);
			else {
				cerr << "Error! Invalid variable name " << name;
				if (num_line != -1)
					cerr << " on line " << num_line << endl;
				else
					cerr << endl;
				return false;
			}
		}
		else {
			ss >> value;
			if (HTable.Contains(name)) {
				if (regex_match(value, re0)) {
					cerr << "Error! Redefinition of type " << name;
					if (num_line != -1)
						cerr << " on line " << num_line << endl;
					else
						cerr << endl;
					cerr << "Error! Performing operations on an uninitialized variable " << name;
					if (num_line != -1)
						cerr << " on line " << num_line << endl;
					else
						cerr << endl;
					return false;
				}
				cerr << "Error! Redefinition of type " << name;
				if (num_line != -1)
					cerr << " on line " << num_line << endl;
				else
					cerr << endl;
				return false;
			}
			else {
				if (value == "=") {
					if (regex_match(name, re1))
						HTable.Add(name, type, true);
					else {
						cerr << "Error! Invalid variable name " << name;
						if (num_line != -1)
							cerr << " on line " << num_line << endl;
						else
							cerr << endl;
						return false;
					}
				}
				else {
					if (regex_match(value, re0)) {
						cerr << "Error! Performing operations on an uninitialized variable " << name;
						if (num_line != -1)
							cerr << " on line " << num_line << endl;
						else
							cerr << endl;
						return false;
					}
					if (regex_match(name, re1))
						HTable.Add(name, type);
					else {
						cerr << "Error! Invalid variable name " << name;
						if (num_line != -1)
							cerr << " on line " << num_line << endl;
						else
							cerr << endl;
						return false;
					}
				}
			}
		}
	}
	else {
		if (regex_match(type, re1)) {
			if (HTable.Contains(type)) {
				ss >> value;
				if (value == "=") {
					HTable.SetValue(type, true);
				}
				if (regex_match(value, re0)) {
					lexeme temp = HTable.getElement(type);
					if (!temp.value) {
						cerr << "Error! Performing operations on an uninitialized variable " << type;
						if (num_line != -1)
							cerr << " on line " << num_line << endl;
						else
							cerr << endl;
						return false;
					}
				}
			}
			else {
				cerr << "Error! Using an uninitialized variable " << type;
				if (num_line != -1)
					cerr << " on line " << num_line << endl;
				else
					cerr << endl;
				return false;
			}
		}
	}
}

bool ScanningFile(const string& ScanFile) {
	if (!CTable.ScanningFile(ScanFile)) {
		cerr << "Error! Failed to read scanning file." << endl;
		return false;
	}
	ifstream ScanningFile(ScanFile, ios::in);
	if (!ScanningFile.is_open()) {
		cerr << "Error! Failed to read scanning file." << endl;
		return false;
	}
	int num_line = 0;
	string line;
	while (getline(ScanningFile, line)) {
		ScanningLine(line, num_line);
		num_line++;
	}
	ScanningFile.close();
	return true;
}

void Menu() {
	/*
	1 - Добавить строку в таблицы
	2 - Сканировать файл
	3 - Напечатать статическую таблицу
	4 - Напечатать переменную таблицу
	5 - Вернуть таблицы к исходному состоянию
	6 - Получить полную информацию об переменной по имени идентификатора
	7 - Проверить существует ли переменная по идентификатору
	8 - Установить другой тип для переменной
	9 - Установить состояние свойства об инициализации переменной
	10 - Получить кол-во вхождений типа или операции
	11 - Выйти
	*/
	int ans = 0;
	string temp = "";
	while (ans != 11) {
		cout << "1 - Добавить строку в таблицы" << endl << "2 - Сканировать файл" << endl << "3 - Напечатать статическую таблицу" << endl <<
			"4 - Напечатать переменную таблицу" << endl << "5 - Вернуть таблицы к исходному состоянию" << endl <<
			"6 - Получить полную информацию об переменной по имени идентификатора" << endl << "7 - Проверить существует ли переменная по идентификатору" << endl <<
			"8 - Установить другой тип для переменной" << endl << "9 - Установить состояние свойства об инициализации переменной" << endl << "10 - Получить кол-во вхождений типа или операции" << 
			endl << "11 - Выйти" << endl;
		cin >> ans;
		switch (ans)
		{
		case 1:
			cout << "Введите строчку для сканирования:" << endl;
			cin.ignore();
			getline(cin, temp);
			if (temp != "") {
				CTable.ScanningLine(temp);
				ScanningLine(temp);
			}
			break;
		case 2:
			cout << "Введите имя файла для сканирования:" << endl;
			cin >> temp;
			ScanningFile(temp);
			break;
		case 3:
			CTable.PrintTable();
			break;
		case 4:
			HTable.PrintTable();
			break;
		case 5:
			CTable.clear();
			HTable.clear();
			CTable.ReadKeysFile("Types.txt");
			break;
		case 6:
			cout << "Введите имя идентификатора:" << endl;
			cin >> temp;
			if (HTable.Contains(temp)) {
				lexeme val = HTable.getElement(temp);
				cout << "-----------" << endl << "Имя переменной: " << val.name << endl <<
					"Тип: " << val.type << endl << "Инициализированна: " << (val.value ? "Да" : "Нет") << endl <<
					"-----------" << endl;
			}
			else {
				cerr << "Такого имени " << temp << " не существует" << endl;
			}
			break;
		case 7:
			cout << "Введите имя переменной:" << endl;
			cin >> temp;
			if (HTable.Contains(temp)) {
				cout << "Переменная с именем " << temp << "существует.";
			}
			else {
				cout << "Переменной с именем " << temp << " не существует.";
			}
			break;
		case 8:
			cout << "Введите имя переменной:" << endl;
			cin >> temp;
			if (HTable.Contains(temp)) {
				cout << "Введите новый тип для переменной:" << endl;
				string temp2;
				cin >> temp2;
				if (temp2.size() > 2 && CTable.Contains(temp2)) {
					if (HTable.SetType(temp, temp2)) {
						cout << "Тип успешно изменён" << endl;
					}
					else {
						cerr << "При изменении произошла ошибка" << endl;
					}
				}
				else {
					cerr << "Введённый тип не доступен для изменения. Для корректного ввода выведете доступные типы" << endl;
				}
			}
			else {
				cerr << "Введённого имени не существует" << endl;
			}
			break;
		case 9:
			cout << "Введите имя переменной:" << endl;
			cin >> temp;
			if (HTable.Contains(temp)) {
				cout << "Введите новый стостояние свойства об инициализации переменной (Y/N): " << endl;
				string temp2;
				cin >> temp2;
				if (temp2 == "Y") {
					if (HTable.SetValue(temp, true)) {
						cout << "Состояние успешно изменёно" << endl;
					}
					else {
						cerr << "При изменении произошла ошибка" << endl;
					}
				}
				else if (temp2 == "N") {
					if (HTable.SetValue(temp, false)) {
						cout << "Состояние успешно изменёно" << endl;
					}
					else {
						cerr << "При изменении произошла ошибка" << endl;
					}
				}
				else {
					cerr << "Состояние введенно не корректно" << endl;
				}
			}
			else {
				cerr << "Введённого имени не существует" << endl;
			}
			break;
		case 10:
			cout << "Введите тип или операцию:" << endl;
			cin >> temp;
			if (CTable.Contains(temp)) {
				cout << "--------" << endl << "Операция или тип: " << temp << endl << "Количество вхождений: " << CTable.Count(temp) << endl << "--------" << endl;
			}
			else {
				cerr << "Введённая операция или тип не найден" << endl;
			}
			break;
		default:
			break;
		}
	}
}


int main() {
	setlocale(LC_ALL, "Russian");
	if (!CTable.ReadKeysFile("Types.txt")) {
		cerr << "Error! Failed to read type file." << endl;
		return 1;
	}
	Menu();
	return 0;
}