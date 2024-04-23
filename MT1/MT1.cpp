#include "Translator.h"


int main() {
	setlocale(LC_ALL, "Russian");
	Translator _TL;
	_TL.ScanningFile("source.txt");
	_TL.PrintErrors();
	return 0;
}