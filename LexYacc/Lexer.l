%{
   /*程序明示常量定义*/
   #include<stdio.h>
   int yylval;
   typedef enum { ELSE=1,IF,INT,RETURN,VOID,WHILE,
    RELOP,COMMENT,
    PLUS,MINUS,MULTIPLY,DIVIDE,LT,LE,GT,GE,EQ,NE,ASSIGN,SEMICOLON,COMMA,LSB,RSB,LMB,RMB,LGB,RGB,
    ID,NUM} Type;
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
{comment}  { yylval=Install(); return(COMMENT); }
"+"        { yylval=PLUS; return(RELOP); }
"-"        { yylval=MINUS; return(RELOP); }
"*"        { yylval=MULTIPLY; return(RELOP); }
"/"        { yylval=DIVIDE; return(RELOP); }
"<"        { yylval=LT; return(RELOP); }
"<="       { yylval=LE; return(RELOP); }
">"        { yylval=GT; return(RELOP); }
">="       { yylval=GE; return(RELOP); }
"=="       { yylval=EQ; return(RELOP); }
"!="       { yylval=NE; return(RELOP); }
"="        { yylval=ASSIGN; return(RELOP); }
";"        { yylval=SEMICOLON; return(RELOP); }
","        { yylval=COMMA; return(RELOP); }
"("        { yylval=LSB; return(RELOP); }
")"        { yylval=RSB; return(RELOP); }
"["        { yylval=LMB; return(RELOP); }
"]"        { yylval=RMB; return(RELOP); }
"{"        { yylval=LGB; return(RELOP); }
"}"        { yylval=RGB; return(RELOP); }

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
    case RELOP:
    fprintf(yyout, "<%s , NULL>", yytext);
    break;

    case COMMENT:
    fprintf(yyout, "<comment , \"%s\">", yytext);
    break;

    case NUM:
    fprintf(yyout, "<NUM , %s>", yytext);
    break;

    case ID:
    fprintf(yyout, "<ID , %s>", yytext);
    break;

    case WHILE:
    fprintf(yyout, "<%s , NULL>", yytext);
    break;

    case VOID:
    fprintf(yyout, "<%s , NULL>", yytext);
    break;

    case RETURN:
    fprintf(yyout, "<%s , NULL>", yytext);
    break;

    case INT:
    fprintf(yyout, "<%s , NULL>", yytext);
    break;

    case IF:
    fprintf(yyout, "<%s , NULL>", yytext);
    break;

    case ELSE:
    fprintf(yyout, "<%s , NULL>", yytext);
    break;

    default:
        break;
}
       fprintf(yyout, " ");

}
int main(int argc, char ** argv)
{
    int pd;
    int pre=1;
    if ((yyin = fopen("./in.txt", "r")) == NULL){
	    printf("Can't open file in.txt,please check the file path\n");
	    return 1;
	}
    int End=0;
    printf( "1 :",yylineno);
    while(End==0)
    {
       while(yylineno==pre)
       {
           pd=yylex();
           if(pd<=0)
           {
               End=1;
               break;
           }
           if(yylineno==pre)
           WriteOut(pd);
       }
       printf("\n");
       if(End==0)
       {
           printf( " %d:",yylineno);
           WriteOut(pd);
           pre=yylineno;
       }
    }
        return 0;

}




