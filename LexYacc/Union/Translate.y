/*y.y*/
%{
#include"Translate.tab.h"
#include <stdio.h>
#include<stdlib.h>
#include"lex.yy.c"
#include"tools.h"

bool error=false;
extern FILE * yyin;
extern FILE * yyout;
%}

%token ELSE IF INT RETURN VOID WHILE
%token RELOP COMMENT
%token PLUS MINUS MULTIPLY DIVIDE LT LE GT GE EQ NE ASSIGN SEMICOLON COMMA LSB RSB LMB RMB LGB RGB
%token ID NUM
%start program
%left PLUS MINUS
%left MULTIPLY DIVIDE
%define parse.error verbose
%%

program : declaration-list   {       fprintf(yyout, "%s\n", "program -> declaration-list");  }  
          ;

declaration-list : declaration-list declaration  { fprintf(yyout, "%s\n", "declaration-list -> declaration-list declaration"); }
                 | declaration { fprintf(yyout, "%s\n", "declaration-list -> declaration"); }
                 ; 

declaration : var-declaration { fprintf(yyout, "%s\n", "declaration -> var-declaration"); }
            | fun-declaration { fprintf(yyout, "%s\n", "declaration -> fun-declaration"); }
            ;

var-declaration : type-specifier ID SEMICOLON { fprintf(yyout, "%s\n", "var-declaration -> type-specifier ID ;"); }
                | type-specifier ID LMB NUM RMB SEMICOLON { fprintf(yyout, "%s\n", "var-declaration -> type-specifier ID [ NUM ] ;");  }
                ;

type-specifier : INT  { fprintf(yyout, "%s\n", "type-specifier -> int"); }
               | VOID { fprintf(yyout, "%s\n", "type-specifier -> void"); }
               ;

fun-declaration : type-specifier ID LSB params RSB  compound-stmt  { fprintf(yyout, "%s\n", "fun-declaration -> type-specifier ID ( params )  compound-stmt"); }
                ;

param-list : param-list COMMA param  { fprintf(yyout, "%s\n", "param-list -> param-list , param"); }
            | param { fprintf(yyout, "%s\n", "param-list -> param"); }
            ;
            
params : param-list  { fprintf(yyout, "%s\n", "params -> param-list"); }
       | VOID { fprintf(yyout, "%s\n", "params -> void"); }
       ;


param : type-specifier ID  { fprintf(yyout, "%s\n", "param -> type-specifier ID"); }
      | type-specifier ID LMB RMB { fprintf(yyout, "%s\n", "param -> type-specifier ID [ ]"); }
      ;

compound-stmt : LGB local-declarations statement-list RGB { fprintf(yyout, "%s\n", "compound-stmt -> { local-declarations statement-list }"); }
              ;

local-declarations : local-declarations var-declaration  { fprintf(yyout, "%s\n", "local-declarations -> local-declarations var-declaration"); }
                   | %empty { fprintf(yyout, "%s\n", "local-declarations -> ^"); }
                   ; 

statement-list : statement-list statement  { fprintf(yyout, "%s\n", "statement-list -> statement-list statement"); }
               | %empty { fprintf(yyout, "%s\n", "statement-list -> ^ "); }
               ;

statement : expression-stmt  { fprintf(yyout, "%s\n", "statement -> expression-stmt"); }
          | compound-stmt  { fprintf(yyout, "%s\n", "statement -> compound-stmt");}
          | selection-stmt {fprintf(yyout, "%s\n", "statement -> selection-stmt"); }
          | iteration-stmt  { fprintf(yyout, "%s\n", "statement -> iteration-stmt"); }
          | return-stmt { fprintf(yyout, "%s\n", "statement -> return-stmt"); }
          ;

expression-stmt : expression SEMICOLON { fprintf(yyout,"%s\n","expression-stmt -> expression ;"); }
                | SEMICOLON { fprintf(yyout,"%s\n","expression-stmt -> ;"); }
                ;

selection-stmt : IF LSB expression RSB statement  { fprintf(yyout,"%s\n","selection-stmt -> if ( expression ) statement"); }
               | IF LSB expression RSB statement ELSE statement  { fprintf(yyout,"%s\n","selection-stmt -> if ( expression ) statement else statement"); }
               ;

iteration-stmt : WHILE LSB expression RSB  statement  { fprintf(yyout,"%s\n","iteration-stmt -> while ( expression ) statement"); }
               ;

return-stmt : RETURN SEMICOLON   { fprintf(yyout,"%s\n","return-stmt -> return ;"); }
            | RETURN expression SEMICOLON { fprintf(yyout,"%s\n","return-stmt -> return expression ; "); }
            ;

expression : var ASSIGN expression  { fprintf(yyout,"%s\n","expression -> var = expression"); }
           | simple-expression { fprintf(yyout,"%s\n","expression -> simple-expression"); }
           ;

var : ID  { fprintf(yyout,"%s\n","var -> ID"); }
    | ID LMB expression RMB { fprintf(yyout,"%s\n","var -> ID [ expression ]"); }
    ;

simple-expression : additive-expression relop additive-expression   { fprintf(yyout,"%s\n","simple-expression -> additive-expression relop additive-expression"); }
                  | additive-expression  { fprintf(yyout,"%s\n","simple-expression -> additive-expression"); }
                  ;

relop : LE { fprintf(yyout,"%s\n","relop -> <="); }
      | LT { fprintf(yyout,"%s\n","relop -> <"); }
      | GT { fprintf(yyout,"%s\n","relop -> >"); }
      | GE { fprintf(yyout,"%s\n","relop -> >="); }
      | EQ { fprintf(yyout,"%s\n","relop -> =="); }
      | NE { fprintf(yyout,"%s\n","relop -> !="); }
      ;

additive-expression : additive-expression addop term  { fprintf(yyout,"%s\n","additive-expression -> additive-expression addop term"); }
                    | term { fprintf(yyout,"%s\n","additive-expression -> term"); }
                    ;

addop : PLUS  { fprintf(yyout,"%s\n","addop -> +"); }
      | MINUS { fprintf(yyout,"%s\n","addop -> -"); }
      ;

term : term mulop factor  { fprintf(yyout,"%s\n","term -> term mulop factor"); }
     | factor { fprintf(yyout,"%s\n","term -> factor"); }
     ;

mulop : MULTIPLY  { fprintf(yyout,"%s\n","mulop -> *"); }
      | DIVIDE { fprintf(yyout,"%s\n","mulop -> /"); }
      ;

factor : LSB expression RSB  { fprintf(yyout,"%s\n","factor -> ( expression )"); }
       | var  { fprintf(yyout,"%s\n","factor -> var"); }
       | call { fprintf(yyout,"%s\n","factor -> call"); }
       | NUM  { fprintf(yyout,"%s\n","factor -> NUM"); }
       ;

call : ID LSB args RSB  { fprintf(yyout,"%s\n","call -> ID ( args )"); }
     ;

args : arg-list  { fprintf(yyout,"%s\n","args -> arg-list"); }
     | %empty { fprintf(yyout,"%s\n","args -> ^"); }
     ;

arg-list : arg-list COMMA  expression  { fprintf(yyout,"%s\n","arg-list -> arg-list ,  expression"); }
         | expression { fprintf(yyout,"%s\n","arg-list ->  expression"); }
         ; 



%%
int yyerror(const char* msg)
{
   error=true;
   printf("Error: %s encountered at line number:%d\n", msg, yylineno);
   //printf("info: %s encountered at line number:%d\n",  YYERROR_VERBOSE, yylineno);
}

int main(int argc, char ** argv)
{
    int pd;
   // yyout=stdout;
   cout<<"hello world\n";
    if ((yyin = fopen("./in.txt", "r")) == NULL){
	    printf("Can't open file in.txt,please check the file path\n");
	    return 1;
	}
     if ((yyout = fopen("./out.txt", "w")) == NULL){
	    printf("Can't open file out.txt,please check the file path\n");
	    return 1;
	}

    do
    {
        pd=yylex();
        WriteOut(pd);
        fprintf(yyout, " ");
    }while(pd>0);
    fprintf(yyout, "\n");
    fseek(yyin,0,SEEK_SET);
    fprintf(yyout, "%d\n",yyparse());
    return 0;

}



