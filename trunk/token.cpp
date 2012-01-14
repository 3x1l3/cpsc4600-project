

class Token {

	private:
     int value;
     char symbol;
	 
	public:
     Token();  
     Token(char);
	 Token(int);
	~Token() {} 
    int getValue();
	char getSymbol();
	void set(char);
	void set(int);
};



Token::Token() {
	value = 0;
	symbol = ' ';
}

Token::Token(char t) {
	set(t);
}

Token::Token(int t) {
	set(t);
}

int Token::getValue() {
	return value; 
	}
char Token::getSymbol() {
	return symbol;
}

void Token::set(char t) {
	value = (int)t; 
	symbol = t;
}

void Token::set(int t) {
	value = t;
	symbol = (char)t;
}
