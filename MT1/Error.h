#pragma once
#include "Base.h"
#define ERROR0 "No Error"
#define ERROR1 "FATAL ERROR! File is not opened!"
#define ERROR2 "FATAL ERROR! Scanning file is not opened!"
#define ERROR3 "ERROR! Comment block used incorrectly in line "
#define ERROR4 "ERROR! The number is used incorrectly in line "
#define ERROR5 "ERROR! The incorrect number of dots is used in writing a real number in line "
#define ERROR6 "ERROR! Invalid operation in line "
#define ERROR7 "ERROR! Invalid name of variable in line "
#define ERROR8 "ERROR! Failed to add to table in line "
#define ERROR9 "ERROR! Invalid type of variable in line "
#define ERROR10 "ERROR! Unknown name in line "
#define ERROR11 "ERROR! Incorrect use of operation in line "
#define ERROR12 "ERROR! Incorrect variable assignment in line "
#define ERROR13 "ERROR! Performing operations on an uninitialized variable in line "
#define ERROR14 "ERROR! Overwriting a Variable in line "
#define ERROR15 "ERROR! Using a reserved keyword to indicate a variable name in line "
#define ERROR16 "ERROR! Invalid symbol in line "

#ifndef ERROR

enum Code_Errors {
	NO_ERROR = 0,
	FATALERR_FAILLEDREADFILE,
	FATALERR_FAILLEDREADSCANFILE,
	ERR_BLOCKCOMMENT,
	ERR_INVALID_INT_NUM,
	ERR_INVALID_FLOAT_NUM,
	ERR_INVALID_OPERATION,
	ERR_INVALID_NAME,
	ERR_FAILED_ADD_TABLE,
	ERR_INVALID_TYPE_VAR,
	ERR_UNKNOWN_VARIABLE,
	ERR_INCORRECT_USE_OPERATION,
	ERR_INVALID_ASSIGNMENT_OPERATION,
	ERR_PERFORMING_OPERATION_UNINITIALIZED_VAR,
	ERR_VAR_OVERRIDES,
	ERR_KEYWORD_IN_VAR_NAME,
	ERR_INVALID_SYMBOL
};

class Errors {

public:
	int NUM_LINE = 0;
	Code_Errors CODE = NO_ERROR;
	Errors() {}
	Errors(int num_line, Code_Errors error) {
		NUM_LINE = num_line;
		CODE = error;
	}
	inline string info() {
		switch (CODE)
		{
		case NO_ERROR:
			return ERROR0;
		case FATALERR_FAILLEDREADFILE:
			return ERROR1;
		case FATALERR_FAILLEDREADSCANFILE:
			return ERROR2;
		case ERR_BLOCKCOMMENT:
			return ERROR3 + to_string(NUM_LINE) + ".";
		case ERR_INVALID_INT_NUM:
			return ERROR4 + to_string(NUM_LINE) + ".";
		case ERR_INVALID_FLOAT_NUM:
			return ERROR5 + to_string(NUM_LINE) + ".";
		case ERR_INVALID_OPERATION:
			return ERROR6 + to_string(NUM_LINE) + ".";
		case ERR_INVALID_NAME:
			return ERROR7 + to_string(NUM_LINE) + ".";
		case ERR_FAILED_ADD_TABLE:
			return ERROR8 + to_string(NUM_LINE) + ".";
		case ERR_INVALID_TYPE_VAR:
			return ERROR9 + to_string(NUM_LINE) + ".";
		case ERR_UNKNOWN_VARIABLE:
			return ERROR10 + to_string(NUM_LINE) + ".";
		case ERR_INCORRECT_USE_OPERATION:
			return ERROR11 + to_string(NUM_LINE) + ".";
		case ERR_INVALID_ASSIGNMENT_OPERATION:
			return ERROR12 + to_string(NUM_LINE) + ".";
		case ERR_PERFORMING_OPERATION_UNINITIALIZED_VAR:
			return ERROR13 + to_string(NUM_LINE) + ".";
		case ERR_VAR_OVERRIDES:
			return ERROR14 + to_string(NUM_LINE) + ".";
		case ERR_KEYWORD_IN_VAR_NAME:
			return ERROR15 + to_string(NUM_LINE) + ".";
		case ERR_INVALID_SYMBOL:
			return ERROR16 + to_string(NUM_LINE) + ".";
		default:
			break;
		}
	}

};

#endif // !
