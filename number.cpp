

class Number : public Token {
	public:
	int value;
	Number(int);
};

Number::Number(int v) : Token(NUM) {
	value = v;
}