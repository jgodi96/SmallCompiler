/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// this function simply checks the next token without
// consuming the input
// Written by Mohsen Zohrevandi
Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

// Parsing

void Parser::parse_input()
{
    //input -> tokens_section INPUT_TEXT

    parse_tokens_section();
    expect(INPUT_TEXT);
}


void Parser::parse_tokens_section()
{
    // tokens_section -> token_list HASH


    parse_token_list();
    expect(HASH);
}

void Parser::parse_token_list()
{
    // token_list -> token
    // token_list -> token COMMA token_list

    parse_token();
    Token t = peek();
    if (t.token_type == COMMA)
    {
        // token_list -> token COMMA token_list
        expect(COMMA);
        parse_token_list();
    }
    else if (t.token_type == HASH)
    {
        // token_list -> token
    }
    else
    {
        syntax_error();
    }

}

void Parser::parse_token()
{
    // token -> ID expr

    expect(ID);
    parse_expr();

}

void Parser::parse_expr()
{
    // expr -> CHAR
    // expr -> LPAREN expr RPAREN DOT LPAREN expr RPAREN
    // expr -> LPAREN expr RPAREN OR LPAREN expr RPAREN
    // expr -> LPAREN expr RPAREN STAR
    // expr -> UNDERSCORE

    Token t = lexer.GetToken();
    if(t.token_type == CHAR){
        // expr -> CHAR
    }
    else if (t.token_type == UNDERSCORE){
        // expr -> UNDERSCORE
    }
    else if(t.token_type == LPAREN) {
        // expr -> LPAREN expr RPAREN DOT LPAREN expr RPAREN
        // expr -> LPAREN expr RPAREN OR LPAREN expr RPAREN
        // expr -> LPAREN expr RPAREN STAR
        parse_expr();
        expect(RPAREN);
        Token t2 = lexer.GetToken();
        if (t2.token_type == DOT || t2.token_type == OR) {
            expect(LPAREN);
            parse_expr();
            expect(RPAREN);
        } else if (t2.token_type == STAR)
        {
        }

    }
    else
    {
        syntax_error();
    }
}


void Parser::ParseProgram()
{
    parse_input();
    expect(END_OF_FILE);
}

int main()
{
    Parser parser;

    parser.ParseProgram();
}
