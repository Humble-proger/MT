#include "Translator.h"
#define NAMEFILEERRORS "out_files/error.txt"
#define NAMEFILETOKEN "out_files/token.txt"
#define NAMEFILEIDENTIFICATORS "out_files/identificators.txt"
#define NAMEFILECONSTANS "out_files/constans.txt"
#define NAMEFILECONSTTABLE "out_files/consttables.txt"


int main() {
	setlocale(LC_ALL, "Russian");
	Translator _TL;
	_TL.ScanningFile("source.txt");
	ofstream FileToken(NAMEFILETOKEN);
	if (!_TL.outFileToken(FileToken)) {
		cerr << NAMEFILETOKEN << " file not opened!";
		return 15;
	}
	FileToken.close();
	ofstream FileErrors(NAMEFILEERRORS);
	if (!_TL.outFileErrors(FileErrors)) {
		cerr << NAMEFILEERRORS << " file not opened!";
		return 16;
	}
	FileErrors.close();
	ofstream FileIC(NAMEFILEIDENTIFICATORS);
	if (!_TL.outFileIC(FileIC)) {
		cerr << NAMEFILEIDENTIFICATORS << " file not opened!";
		return 17;
	}
	FileIC.close();
	ofstream FileC(NAMEFILECONSTANS);
	if (!_TL.outFileC(FileC)) {
		cerr << NAMEFILECONSTANS << " file not opened!";
		return 18;
	}
	FileC.close();
	ofstream FileCon(NAMEFILECONSTTABLE);
	if (!_TL.outConstTable(FileCon)) {
		cerr << NAMEFILECONSTTABLE << " file not opened!";
		return 19;
	}
	FileCon.close();
	return 0;
}