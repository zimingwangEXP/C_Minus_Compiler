lex Lexer.l
bison -d Translate.y
g++ -o test Translate.tab.c
./test
g++ -o test1 YaccTree.cpp
./test1



