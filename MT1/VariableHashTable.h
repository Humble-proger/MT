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
        // ѕроста€ стратеги€ линейного пробировани€
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
    bool Add(const string& key, const string& Type="", bool Value = false) {
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
    bool SetStruct(const string& key, bool newS) {
        int hash = calculateHash(key);
        while (hashTable.find(hash) != hashTable.end() && hashTable[hash].valid) {
            if (hashTable[hash].name == key) {
                hashTable[hash].in_struct = newS;
                return true;
            }
            hash = rehash(hash);
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return false;
    }
    lexeme getElement(const string& key) {
        int hash = calculateHash(key);
        lexeme temp;
        while (hashTable.find(hash) != hashTable.end() && hashTable[hash].valid) {
            if (hashTable[hash].name == key) {
                temp = hashTable[hash];
                if (hashTable[hash].value)
                    temp.value = true;
                return temp;
            }
            hash = rehash(hash);
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return lexeme();
       
    }
    int getIndex(const string& key) {
        int hash = calculateHash(key);
        while (hashTable.find(hash) != hashTable.end() && (hashTable[hash].valid || hashTable.count(hash) == 0)) {
            if (hashTable[hash] == key) {
                return hash;
            }
            hash = rehash(hash);
        }
        return -1;
    }

    lexeme getElemIndex(int index) {
        lexeme temp;
        if (index >= 0 && index < TABLE_SIZE) {
            temp = hashTable[index];
            if (hashTable[index].value)
                temp.value = true;
            return temp;
        }
        else {
            return lexeme();
        }
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
    bool outFile(ofstream& FileOut) {
        if (!FileOut.is_open()) {
            cerr << "Error! File not opened!" << endl;
            return false;
        }
        for (auto& t_elem : hashTable) {
            FileOut << t_elem.second.name << "\t" << t_elem.second.type << "\t" << t_elem.second.value << endl;
        }
        return true;
    }
    vector<lexeme> values() {
        vector<lexeme> result(hashTable.size());
        int i = 0;
        for (auto& pair : hashTable) {
            result[i] = pair.second;
            i++;
        }
        return result;
    }
    vector<int> keys() {
        vector<int> result(hashTable.size());
        int i = 0;
        for (auto& pair : hashTable) {
            result[i] = pair.first;
            i++;
        }
        return result;
    }
    bool getValue(const string& key) {
        int hash = calculateHash(key);
        while (hashTable.find(hash) != hashTable.end() && hashTable[hash].valid) {
            if (hashTable[hash].name == key) {
                return hashTable[hash].value;
            }
            hash = rehash(hash);
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return false;
    }
    bool getValueIndex(int index) {
        if (index >= 0 && index < TABLE_SIZE) {
            return hashTable[index].value;
        }
        else {
            return false;
        }
    }
    bool getStruct(const string& key) {
        int hash = calculateHash(key);
        while (hashTable.find(hash) != hashTable.end() && hashTable[hash].valid) {
            if (hashTable[hash].name == key) {
                return hashTable[hash].in_struct;
            }
            hash = rehash(hash);
        }
        cerr << "Element with key '" << key << "' not found." << endl;
        return false;
    }
    bool getStructIndex(int index) {
        if (index >= 0 && index < TABLE_SIZE) {
            return hashTable[index].in_struct;
        }
        else {
            return false;
        }
    }

};
#endif
