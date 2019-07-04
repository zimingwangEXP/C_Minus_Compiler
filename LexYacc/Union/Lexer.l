%{
   /*程序明示常量定义*/
    #include<stdio.h>
    #include"Translate.tab.h"
    #include"tools.h"
    int Install();
%}
%option yylineno
/*正则表达式定义*/
delim    [ \t\n]
ws       {delim}+
letter   [a-zA-Z]
id       {letter}+
digit    [0-9]
num      {digit}+
comment  \/\*([^*]*|\*+[^/])*\*\/
%%
    /*
       注意对于运算符key和value值的分派可能存在冗余
    */
{ws}       { /*do no thing ,just look at the next one*/ }
else       { return(ELSE); }
if         { return(IF); }
int        { return(INT); }
return     { return(RETURN); }
void       { return(VOID); }
while      { return(WHILE); }
{id}       { yylval=Install(); return(ID); }
{num}      { yylval=Install(); return(NUM); }
{comment}  { yylval=Install();  }
"+"        { yylval=PLUS; return(PLUS); }
"-"        { yylval=MINUS; return(MINUS); }
"*"        { yylval=MULTIPLY; return(MULTIPLY); }
"/"        { yylval=DIVIDE; return(DIVIDE); }
"<"        { yylval=LT; return(LT); }
"<="       { yylval=LE; return(LE); }
">"        { yylval=GT; return(GT); }
">="       { yylval=GE; return(GE); }
"=="       { yylval=EQ; return(EQ); }
"!="       { yylval=NE; return(NE); }
"="        { yylval=ASSIGN; return(ASSIGN); }
";"        { yylval=SEMICOLON; return(SEMICOLON); }
","        { yylval=COMMA; return(COMMA); }
"("        { yylval=LSB; return(LSB); }
")"        { yylval=RSB; return(RSB); }
"["        { yylval=LMB; return(LMB); }
"]"        { yylval=RMB; return(RMB); }
"{"        { yylval=LGB; return(LGB); }
"}"        { yylval=RGB; return(RGB); }

%%
int Install()
{
    
    return 0;
}
int yywrap (){
  return 1;
}
void WriteOut(int pd)
{
  switch (pd)
{
    case COMMENT:
    fprintf(yyout, "<comment , \"%s\"|", yytext);
    break;

    case NUM:
    fprintf(yyout, "|NUM|%s|", yytext);
    break;

    case ID:
    fprintf(yyout, "|ID|%s|", yytext);
    break;

    case WHILE:
    fprintf(yyout, "|%s|NULL|", yytext);
    break;

    case VOID:
    fprintf(yyout, "|%s|NULL|", yytext);
    break;

    case RETURN:
    fprintf(yyout, "|%s|NULL|", yytext);
    break;

    case INT:
    fprintf(yyout, "|%s|NULL|", yytext);
    break;

    case IF:
    fprintf(yyout, "|%s|NULL|", yytext);
    break;

    case ELSE:
    fprintf(yyout, "|%s|NULL|", yytext);
    break;

    default:
    fprintf(yyout, "|%s|NULL|", yytext);
        break;
}
       fprintf(yyout, " ");

}

