#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include "lex.h"

using namespace std;


Lex getNextToken(istream& in, int& linenum)
{
    enum LexState { //Different states 
        START,
        INID,
        ININT,
        INSTR,
        INCOMMENT,
        ONESLASH
    };
   
    LexState lexstate = START; //starting with initial element in the input
    string lexeme;
    bool doubleQuoteSeen = false;    //using to avoid double quotes in the output of string constant
    int doubleQuoteLineNum = 0;
    char ch;
    
    while(in.get(ch))    //read character at a time 
    {
        switch (lexstate)
        {
            case START:
                if (ch =='\n')
                {
                    linenum++;
                    continue;
                }
                else if (isspace(ch))
                {
                   continue; 
                }
                lexeme = ch;
                if (ch == '+')
                {
                    return Lex(PLUS,lexeme,linenum);
                }
                else if (ch == '-')
                {
                   return Lex(MINUS,lexeme,linenum);
                }
                else if (ch == '*')
                {
                    return Lex(STAR,lexeme,linenum);
                }
                else if (ch == '!')
                {
                    return Lex(BANG,lexeme,linenum);
                }
                else if (ch == '(')
                {
                    return Lex(LPAREN,lexeme,linenum);
                }
                else if (ch == ')')
                {
                    return Lex(RPAREN,lexeme,linenum);
                }
                else if (ch == ';')
                {
                    return Lex(SC,lexeme,linenum);
                }
                if (ch == '/')
                {
                    lexstate = ONESLASH;
                }
                else if (isdigit(ch))
                {
                    lexstate = ININT;
                }
                else if (isalpha(ch))
                {
                    lexstate = INID;
                }
                else if (ch == '"')    //begining of string
                {
                    lexstate = INSTR;
                    doubleQuoteSeen = true;
                    doubleQuoteLineNum = linenum;
                }
                //if above conditions weren't matched then must be an error 
                else
                {
                    linenum++;
                    return Lex(ERR,lexeme,linenum);
                }
                break;
            //when first character in the input had number or letter 
            case INID:
                if (isalnum(ch))
                {
                    lexeme += ch;
                    continue;
                }
                else if (lexeme == "let")
                {
                    return Lex(LET,lexeme,linenum);
                }
                in.putback(ch);
                return Lex(ID,lexeme,linenum);
            //first character is a number 
            case ININT:
                if (isdigit(ch))
                {
                    lexeme += ch;
                    continue;
                }
                in.putback(ch);
                return Lex(INT,lexeme,linenum);
            // input started with double quotes
            case INSTR:
                if (doubleQuoteSeen)    //avoid double quotes in the final output 
                {
                    lexeme = "";
                    doubleQuoteSeen = false;
                    in.putback(ch);
                    continue;
                }
                if (ch == '\\')        //handling escape characters 
                {
                    //doubleQuoteSeen = false;
                    in.get(ch);
                    if (ch == 'n')
                    {
                        lexeme += "\n";
                        continue;
                    }
                    lexeme += ch;
                    continue;
                }
                else if (ch == '"')    //end of string constant 
                {
                    if (doubleQuoteLineNum != linenum)
                    {
                        string tmp = "\"";
                        tmp.append(lexeme);
                        return Lex(ERR,tmp,linenum);
                    }
                    doubleQuoteLineNum = 0;
                    return Lex(STR,lexeme,linenum);
                }
                if (ch == '\n')    //new line interpreter 
                {
                    linenum++;
                    lexeme += '\n';
                    continue;
                }
                lexeme += ch;
            // when the first two character were //
            case INCOMMENT:
                if( ch != '\n')
                {
                    continue;
                }
                lexstate = START;
                linenum++;
                break;
            // first character is / and check if following char is also / or not and if it is then must be comment 
            case ONESLASH:
                if ( ch != '/')
                {
                    in.putback(ch);
                    return Lex(SLASH,lexeme,linenum);
                }
                lexstate = INCOMMENT;
                break;
        }
    }
    //when nothing to read 
    return Lex(DONE,"",linenum);
}
ostream& operator<<(ostream& out, const Lex& tok)
{
    map<Token,string> tokenMap = {
        {PRINT,"PRINT"},{LET,"LET"},{IF,"IF"},{LOOP,"LOOP"},
        {BEGIN,"BEGIN"},{END,"END"},{ID,"ID"},{INT,"INT"},
        {STR,"STR"},{PLUS,"PLUS"},{MINUS,"MINUS"},{STAR,"STAR"},
        {SLASH,"SLASH"},{BANG,"BANG"},{LPAREN,"LPAREN"},{RPAREN,"RPAREN"},
        {SC,"SC"},{ERR,"ERR"}
    };
    if(tok.GetToken() == ID || tok.GetToken() == INT || tok.GetToken() == STR || tok.GetToken() == ERR)
    {
        out << tokenMap[tok.GetToken()] << "(" << tok.GetLexeme() << ")";
        return out;
    }
    out << tokenMap[tok.GetToken()];
    return out;
}