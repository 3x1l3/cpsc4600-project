

class Number : public Token {
	public:
	int value;
	Number(int);
	void SetValue(int);
};

Number::Number(int v) : Token(NUM) {
	value = v;
}

void Number::SetValue(int newVal)
{
  value = newValue;
}