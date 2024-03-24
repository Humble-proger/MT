#pragma once
#ifndef TABLE_CONST
#include "Lexeme.h"

class ConstantTable {
private:
    map<string, int> Table;
public:
    //Конструктор по умолчанию
    ConstantTable() {}
    ConstantTable(const string& Namefile, const string& ScanFile) {
        ReadKeysFile(Namefile);
        ScanningFile(ScanFile);
    }
    //Деструктор
    ~ConstantTable() {
        Table.clear();
    }
    //Добавить элемент
    void Add(const string& NameofType) {
        if (Contains(NameofType)) {
            Table[NameofType] += 1;
        }
        else {
            Table[NameofType] = 0;
        }
    }
    //Содержит ли таблица элемент
    bool Contains(const string& NameOfType) {
        return Table.find(NameOfType) != Table.end();
    }
    //Получить количество вхождений данного элемента
    int Count(const string& NameOfType) {
        if (Contains(NameOfType))
            return Table[NameOfType];
        else 
            return -1;
    }
    //Заполнить таблицу из файла
    bool ReadKeysFile(const string& name) {
        ifstream ReadedFile(name, ios::in);
        if (!ReadedFile.is_open()) return false;
        string Types;
        while (!ReadedFile.eof()) {
            ReadedFile >> Types;
            Table[Types] = 0;
        }
        ReadedFile.close();
        return true;
    }
    void ScanningLine(const string& line) {
        stringstream ss(line);
        string elem;
        while (!ss.eof()) {
            ss >> elem;
            if (elem[elem.size() - 1] == ';') {
                elem.replace(elem.size() - 1, 1, "");
            }
            if (Contains(elem)) {
                Table[elem] += 1;
            }
        }
    }
    
    bool ScanningFile(const string& Name) {
        ifstream ScanFile(Name, ios::in);
        if (!ScanFile.is_open()) return false;
        string elem;
        while (!ScanFile.eof()) {
            ScanFile >> elem;
            if (elem[elem.size() - 1] == ';') {
                elem.replace(elem.size()-1, 1, "");
            }
            if (Contains(elem)) {
                Table[elem] += 1;
            }
        }
        ScanFile.close();
        return true;
    }
    void PrintTable() {
        cout << "Element" << '\t' << "Count" << endl;
        for (const pair<const string&, int> val : Table) {
            cout << val.first << '\t' << val.second << endl;
        }
    }
    void clear() {
        Table.clear();
    }
};

#endif