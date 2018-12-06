/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"

// ------- token types -------------------
struct REG
{
    struct REG_node *start;
    struct REG_node *accept;
};
struct REG_node
{
    struct REG_node *firstneighbor;
    char firstlabel;
    struct REG_node *secondneighbor;
    char secondlabel;
};
struct REG *parse_expr() {

}
typedef enum { END_OF_FILE = 0,
    COMMA, LPAREN, RPAREN,
    DOT, ID, ERROR, INPUT_TEXT, HASH,
    CHAR, UNDERSCORE, OR, SYMBOL, STAR // TODO: Add labels for new token types here
} TokenType;

class Token {
  public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    int line_no;
};

class LexicalAnalyzer {
  public:
    Token GetToken();
    TokenType UngetToken(Token);
    LexicalAnalyzer();

  private:
    std::vector<Token> tokens;
    int line_no;
    Token tmp;
    InputBuffer input;

    bool SkipSpace();
    Token ScanInput();
    Token ScanSymbol();
    Token ScanIdOrChar();

};
class my_LexicalAnalyzer
{
public:
    my_getToken();
    match(r,s,p);
    my_LexicalAnalyzer(list,s);

};


#endif  //__LEXER__H__
