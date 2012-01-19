#ifndef WORD_H
#define WORD_H

class Word : public Token {
public:
    std::string lexeme;
    Word() : Token(0) { lexeme = ""; }
    Word(std::string s, int tag) : Token(tag) {
        lexeme = s;
    }
    std::string toString() {
        return lexeme;
    }

};

#endif
