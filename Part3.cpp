//=================================================================================
// Name        : Part3.cpp
// Description : Support all flags  
//=================================================================================

#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <vector>
#include "lex.h"

using namespace std;

int main(int argc, char *argv[])
{
    istream *in;
    bool fileInput = false;    //input file indicator 
    bool vflag = false;        //"-v" flag indicator 
    bool cflag = false;        //"-consts" flag indicator 
    bool iflag = false;        //"-ids" flag indicator 
    int lineNumber = 0;        //keeping track of number of input lines 
    int numberOfTokens = 0;    //keeping track of total tokens in input 
    Lex lex;
    //storing lexeme based on the token    
    vector<string> identifiers;
    vector<string> strConstant;
    vector<int> intConstant;
    //iterator for iterating over lexemes;
    vector<string>::iterator it;
    
    for (int i = 1; i < argc; i++)
    {
        //if valid FLAG the we want to continue looping
        if (!strcmp(argv[i],"-v")) 
        {
            vflag = true;
            continue;
        }
        else if(!strcmp(argv[i],"-consts"))
        {
            cflag = true;
            continue;
        }
        else if (!strcmp(argv[i],"-ids")) 
        {
            iflag = true;
            continue;
        }
        //if the argument contains '-' but not a valid argument
        else if ( argv[i][0] == '-')
        {
            cout << "UNRECOGNIZED FLAG " << argv[i] << endl;
            return -1;
        }
        //if argument doesn't start with '-' then assuming it's a filename 
        else
        {
            //if we already found filename in the previous loop
            if (fileInput)
            {
                cout << "ONLY ONE FILE NAME ALLOWED" << endl;
                return -1;
            }
            else
            {
                fileInput = true;
            }
        }
        
    }
    //reading input from provided filename
    if (fileInput)
    {
        ifstream infile;
        infile.open(argv[argc-1]);
        //check the validity of the file
        if (infile.is_open() == false)
        {
            cout << "CANNOT OPEN " << argv[argc-1] << endl;
            return -1;
        }
        in = &infile;
        while ((lex=getNextToken(*in, lineNumber)) != DONE && lex != ERR) 
        {
            numberOfTokens++;        
            if (vflag)            //when "-v" flag, print ID (lexeme) 
            {
                cout << lex << endl;
            }
            if (lex.GetToken() == ID)    //add identifiers to the vector storing ID lexeme
            {
                identifiers.push_back(lex.GetLexeme());
            }
            else if (lex.GetToken() == STR)        //add string constant to the vector storing STR lexeme
            {
                strConstant.push_back(lex.GetLexeme());
            }
            else if (lex.GetToken() == INT)        //add integer constant to the vector storing INT lexeme
            {
                intConstant.push_back(stoi(lex.GetLexeme()));
            }
        }
    }
    else {        //reading input from standard input 
        in = &cin;
        while ((lex=getNextToken(*in, lineNumber)) != DONE && lex != ERR) 
        {
            numberOfTokens++;
            if (vflag)        //when "-v" flag, print ID (lexeme)
            {
                cout << lex << endl;
            }
            if (lex.GetToken() == ID)        //add identifiers to the vector storing ID lexeme
            {
                identifiers.push_back(lex.GetLexeme());
            }
            else if (lex.GetToken() == STR)    //add string constant to the vector storing STR lexeme
            {
                strConstant.push_back(lex.GetLexeme());
            }
            else if (lex.GetToken() == INT)     //add integer constant to the vector storing INT lexeme
            {
                intConstant.push_back(stoi(lex.GetLexeme()));
            }
            
        }
    }
    
    if (lex.GetToken() == ERR)        //if ERROR was found then print the line number and the error and STOP there 
    {
        numberOfTokens++;
        cout << "Error on line "<< lex.GetLinenum() << " (" << lex.GetLexeme() << ")" << endl;
        return -1;
    }
    
    if (iflag && (identifiers.size() != 0))    //print identifiers; if "=ids" and at least one identifier in the input
    {
        identifiers.erase( unique( identifiers.begin(), identifiers.end() ), identifiers.end() );    //erase duplicate entries 
        sort(identifiers.begin(), identifiers.end());
        cout << "IDENTIFIERS: ";
        for (it=identifiers.begin(); it!=identifiers.end(); ++it)
        {
            cout << *it;
            if (it != identifiers.end()-1)
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
    else if (cflag && (strConstant.size() !=0))    //print strings; if "-consts" and at least one string present in the input 
    {
        //unique (strConstant.begin(), strConstant.end());
        sort(strConstant.begin(), strConstant.end());
        cout << "STRINGS:" << endl;
        for (it=strConstant.begin(); it!=strConstant.end(); ++it)
        {
            cout << *it << endl;
        }
    }
    else if (cflag && (intConstant.size() !=0))    //print integers; if "-consts" and at least one integer present in the input 
    {
        //unique (intConstant.begin(), intConstant.end());
        sort(intConstant.begin(), intConstant.end());
        cout << "INTEGERS:" << endl;
        for (auto tmp: intConstant)
        {
            cout << tmp << endl;
        }
    }
  
    if (lineNumber == 0)    //if the input is empty 
    {
        cout << "Lines: "<< lineNumber << endl;
        return -1;
    }
    cout << "Lines: "<< lineNumber << endl;
    cout << "Tokens: "<< numberOfTokens << endl;
   
    
    return 0;
}