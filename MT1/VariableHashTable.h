#pragma once
#include "Lexeme.h"
#ifndef VARIABLEHASHTABLE

class VariableHashTable {
private:
    static const int TABLE_SIZE = 30;
    unordered_map<int, lexeme> hashTable;

    int calculateHash(const string& key) {
        int sum = 0;
        for (char c : key) {
            sum += c;
        }
        return sum % TABLE_SIZE;
    }

    int rehash(int hash) {
        // Простая стратегия линейного пробирования
        return hash + 1;
    }

public:
    int SIZE_TABLE = 0;
    
    VariableHashTable() {}

    bool Contains(const string& key) {
        int hash = calculateHash(key);
        while (hashTable.find(hash) != hashTable.end() && (hashTable[hash].valid || hashTable.count(hash) == 0)) {
            if (hashTable[hash] == key) {
                return true;
            }
            hash = rehash(hash);
        }
        return false;
    }
    bool Add(const string& key, const string& Type, bool Value=false) {
        if (SIZE_TABLE >= TABLE_SIZE) {
            cerr << "Table is full";
            return false;
        }
        int hash = calculateHash(key);
        while (hashTable.find(hash) != hashTable.end() && hashTable[hash].valid) {
            if (hashTable[hash].name == key) {
                cerr << "Element with key '" << key << "' is exist." << endl;
                return false;
            }
            hash = rehash(hash);
        }
        lexeme newLexeme(key, Type);
        newLexeme.valid = true;
        hashTable[hash] = newLexeme;
        hashTable[hash].value = Value;
        SIZE_TABLE += 1;
        return true;
    }

    bool Set(const string& key, const string& newType, bool newValue) {
        int hash = calculateHash(key);
        while (hashTable.find(hash) != hashTable.end() && hashTable[hash].valid) {
            if (hashTable[hash].name == key) {
                hashTable[hash].type = newType;
                hashTable[hash].value = newValue;
                return true;
            }
            hash = rehash(hash);
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return false;
    }

    bool SetValue(const string& key, bool newValue) {
        int hash = calculateHash(key);
        while (hashTable.find(hash) != hashTable.end() && hashTable[hash].valid) {
            if (hashTable[hash].name == key) {
                hashTable[hash].value = newValue;
                return true;
            }
            hash = rehash(hash);
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return false;
    }

    bool SetType(const string& key, const string& newType) {
        int hash = calculateHash(key);
        while (hashTable.find(hash) != hashTable.end() && hashTable[hash].valid) {
            if (hashTable[hash].name == key) {
                hashTable[hash].type = newType;
                return true;
            }
            hash = rehash(hash);
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return false;
    }

    lexeme getElement(const string& key) {
        int hash = calculateHash(key);
        while (hashTable.find(hash) != hashTable.end() && hashTable[hash].valid) {
            if (hashTable[hash].name == key) {
                return hashTable[hash];
            }
            hash = rehash(hash);
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return lexeme();
       
    }
    

    void PrintTable() {
        cout << "Type" << '\t' << "Name" << '\t' << "Init" << endl;
        for (const pair<int, lexeme> val : hashTable) {
            cout << val.second.type << '\t' << val.second.name << '\t' << (val.second.value ? "yes" : "no") << endl;
        }
    }
    void clear() {
        hashTable.clear();
        SIZE_TABLE = 0;
    }
};
#endif
