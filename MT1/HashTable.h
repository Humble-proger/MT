#pragma once
#include "Lexeme.h"
#ifndef HASHTABLE
class HashTable {
private:
    static const int TABLE_SIZE = 20;
    vector<pair<string, lexeme>> table[TABLE_SIZE];

    int hashFunction(const string& key) {
        int sum = 0;
        for (char c : key) {
            sum += c;
        }
        return sum % TABLE_SIZE;
    }
    int rehash(int hash) {
        // ѕроста€ стратеги€ линейного пробировани€
        return hash + 1;
    }

public:
    bool Contains(const string& key) {
        int index = hashFunction(key);
        if (table[index].size() == 0) return false;
        for (const auto& pair : table[index] ) {
            if (pair.first == key) {
                return true;
            }
        }
        return false;
    }

    bool SetType(const string& key, const string& newType) {
        int index = hashFunction(key);
        for (auto& pair : table[index]) {
            if (pair.first == key) {
                pair.second.type = newType;
                return true;
            }
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return false;
    }
    bool SetValue(const string& key, bool newValue) {
        int index = hashFunction(key);
        for (auto& pair : table[index]) {
            if (pair.first == key) {
                pair.second.value = newValue;
                return true;
            }
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return false;
    }

    bool Set(const string& key, const string& NewType, bool NewValue) {
        int index = hashFunction(key);
        for (auto& pair : table[index]) {
            if (pair.first == key) {
                pair.second.value = NewValue;
                pair.second.type = NewType;
                return true;
            }
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return false;
    }

    lexeme getElement(const string& key) {
        int index = hashFunction(key);
        for (const auto& pair : table[index]) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return lexeme();
    }

    lexeme popElement(const string& key) {
        int index = hashFunction(key);
        for (int i = 0; i < table[index].size(); i++) {
            auto pair = table[index][i];
            if (pair.first == key) {
                table[index].erase(table[index].begin() + i);
                return pair.second;
            }
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return lexeme();
    }

    bool addElement(const string& key, const string& Type) {
        if (Contains(key)) {
            cerr << "Element with key '" << key << "' is exist." << endl;
            return false;
        }
        int index = hashFunction(key);
        table[index].push_back(make_pair(key, lexeme(key, Type)));
        return true;
    }
};
#endif