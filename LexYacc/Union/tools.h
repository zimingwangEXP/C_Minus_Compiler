#ifndef MAIN_HPP
#define MAIN_HPP
 
#include<fstream>
#include<iostream>
#include<set>
#include<string>
#include<map>
#include<list>
#include<iterator>
#include<algorithm>
#include<sstream>
#include<vector>
#include<assert.h>
#include<ctype.h>
#include<stdlib.h>
#include<stdio.h>

extern "C"
{	

	int yylex(void);
    int yywrap(void);
    int yyerror( const char* msg);
}


using namespace std;

 #endif