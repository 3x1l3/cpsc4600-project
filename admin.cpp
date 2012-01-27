#include "admin.h"


Admin::Admin(string& src)
{
    current_line = 1;
    column = 1;
    symTIndex = 0;
    table = new SymbolTable();
    scanner = new Scanner(*table, *this);
    correctLine = true;
    scanner->loadSource(src);
    errorCount = 0;

}

void Admin::recordError(string msg)
{
    if (correctLine) {
        correctLine = false;
        cerr << msg << endl;
        errorCount++;
    }

}

int Admin::scan()
{

    stringstream error_str;
    Token tok;
    do {
        tok = scanner->nextToken();
        if (tok.getType() == NEWLINE) {
            newLine();
        }
        else if (tok.getType() == BADCHAR) {
            error_str << "Illegal character " << current_line << ":" << column << endl;
            recordError(error_str.str());
        } else if (tok.getType() == BADNUMERAL) {
            error_str << "Numeral overflow on " << current_line << ":" << column << endl;
            recordError(error_str.str());
        } else if (tok.getType() == BADNAME) {
	  error_str << "Identifier cannot exceed 80 characters " << current_line << ":" << column << endl;
	  recordError(error_str.str());
	}

        cout << tok.toString() << endl;
    }
    while (scanner->inRange()) ;

    if (errorCount == 0)
	return 0;
    return -1;
}

void Admin::newLine()
{
    current_line++;
    correctLine = true;
}

void Admin::increaseColumn()
{
    column++;
}
void Admin::resetColumn()
{
    column = 1;
}


