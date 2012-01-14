
class Scanner {
	private:
	void skipWhitespace(char);
	void handleDigit(char);
	int line, src_i;
	std::string source;
	
	public:
	Scanner();
	~Scanner() { };
	Token nextToken();
	void loadSource(std::string);

};

Scanner::Scanner() {
	line = 0;
	src_i = 0;
	source = "";
}

void Scanner::loadSource(std::string src) {
	source = src;
}

void Scanner::skipWhitespace(char peek) {
	
	for(src_i = 0; src_i < source.size(); src_i++) {
		peek = source[src_i];
		
		if ( peek == ' ' || peek == '\t') continue;
		else if (peek == '\n') line++;
		else 
			break;
		
	}
	
}

Token Scanner::nextToken() {	
	char peek;
	skipWhitespace(peek);
	handleDigit(peek);
	return Token();
}

void Scanner::handleDigit(char peek) {
	if (isdigit(peek)) {
	int v = 0;
	do {
		v = 10 * v + atoi((char*)peek);
		src_i++;
		peek = source[src_i];
	} while ( isdigit(peek) );
	
	}
}
