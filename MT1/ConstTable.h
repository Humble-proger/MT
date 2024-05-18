#pragma once
#ifndef TABLE_CONST
#include "Lexeme.h"

template <typename T> 
class ConstantTable {
private:
    map<T, int> Table;
public:
    //Конструктор по умолчанию
    ConstantTable() {}
    ConstantTable(const T& Namefile, const T& ScanFile) {
        ReadKeysFile(Namefile);
        ScanningFile(ScanFile);
    }
    //Деструктор
    ~ConstantTable() {
        Table.clear();
    }
    //Добавить элемент

    bool empty() {
        return Table.empty();
    }

    void Add(const T& NameofType) {
        if (Contains(NameofType)) {
            Table[NameofType] += 1;
        }
        else {
            Table[NameofType] = 0;
        }
    }
    //Содержит ли таблица элемент
    bool Contains(const T& NameOfType) {
        return Table.find(NameOfType) != Table.end();
    }
    //Получить количество вхождений данного элемента
    int Count(const T& NameOfType) {
        if (Contains(NameOfType))
            return Table[NameOfType];
        else 
            return -1;
    }
    //Заполнить таблицу из файла
    bool ReadKeysFile(const string& name) {
        ifstream ReadedFile(name, ios::in);
        if (!ReadedFile.is_open()) return false;
        T Types;
        while (!ReadedFile.eof()) {
            ReadedFile >> Types;
            Table[Types] = 0;
        }
        ReadedFile.close();
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

    int getIndex(T elem) {
        int num = 0;
        for (auto& t_elem : Table) {
            if (t_elem.first == elem) return num;
            else num++;
        }
        return -1;
    }
    pair<T, int> getElembyIndex(int index) {
        auto it = next(Table.begin(), index);
        return { it->first, it->second };
    }
    bool outfile(ofstream& FileOut) {
        if (!FileOut.is_open()) {
            cerr << "Error! File not opened!";
            return false;
        }
        for (auto& t_elem : Table) {
            FileOut << t_elem.first << "\t" << t_elem.second << endl;
        }
        return true;
    }
    vector<int> values() {
        vector<int> result(Table.size());
        int i = 0;
        for (auto& pair : Table) {
            result[i] = Table.second;
            i++;
        }
        return result;
    }
    vector<T> keys() {
        vector<T> result(Table.size());
        int i = 0;
        for (auto& pair : Table) {
            result[i] = Table.first;
            i++;
        }
        return result;
    }
};

#endif