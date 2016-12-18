%{
#include<ctype.h>
#include<stdio.h>
#include<string.h>
int yylex();
void yyerror(char *);
%}

%token GE LE LB RB AND OR SEMI GT LT EQ NE TRUE FALSE
%union{
    char *string;
    int value;
}
%token <string> ID
%token <string> NUM
%token <value> OTHERS

%%

Start:List;
List:Bool SEMI List
    |;
Bool:Bool OR Join {printf("|| ");}
    |Join;
Join:Join AND Equality {printf("&& ");}
    |Equality;
Equality:Equality EQ Rel {printf("== ");}
    |Equality NE Rel {printf("!= ");}
    |Rel;
Rel:Rel LT Factor {printf("< ");}
    |Rel GT Factor {printf("> ");}
    |Rel GE Factor {printf(">= ");}
    |Rel LE Factor {printf("<= ");}
    |Factor;
Factor:LB Bool RB
    |ID {printf("%s " ,$1);}
    |NUM {printf("%s " ,$1);}
    |TRUE {printf("True ");}
    |FALSE {printf("False ");};
    
%%

int main(){
    yyparse();
    return 0; 
}
yywrap(){
    return 1;
}
void yyerror(char *s)
{
    fprintf(stderr, "%s", s);
}