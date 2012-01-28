#include "admin.h"


Admin::Admin(string& src)
{
    current_line = 1;
    column = 0;
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
    //if file is empty, it will return 0, casue no errors were found
    if(scanner->inRange() == false)
      return 0;
  
    stringstream error_str;
    Token tok;
    do {
        tok = scanner->nextToken();
	//if token is a new line token, then we call our newline funciton and reset the 1 error per line boolean
        if (tok.getType() == NEWLINE) {
            newLine();
        }
        //program will only report one error per line at the moment
        else if (tok.getType() == BADCHAR) 
	{
	  error_str << "Illegal character " << current_line << ":" << column << endl;
	  recordError(error_str.str());
	  if(errorCount >= MAXERRORS)
	  {
	    cerr<<"Error threashold limit reached"<<endl;
	    return errorCount;
	  }
        } 
        else if (tok.getType() == BADNUMERAL) 
	{
	  error_str << "Numeral overflow on " << current_line << ":" << column << endl;
	  recordError(error_str.str());
	  if(errorCount >= MAXERRORS)
	  {
	    cerr<<"Error threashold limit reached"<<endl;
	    return errorCount;
	  }
        } 
        else if (tok.getType() == BADNAME) 
	{
	  error_str << "Identifier cannot exceed 80 characters " << current_line << ":" << column << endl;
	  recordError(error_str.str());
	  if(errorCount >= MAXERRORS)
	  {
	    cerr<<"Error threashold limit reached"<<endl;
	    return errorCount;
	  }
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


