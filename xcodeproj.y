/*
 *  parser.y
 *  parser
 *
 *  Created by Jonny Yu on 11/17/10.
 *  Copyright 2010 Autodesk Inc. All rights reserved.
 *
 */

%{
#include <stdio.h>
#include <string.h>
#include <list>
#include <sys/stat.h>
#include <sys/errno.h>
#include "parserdef.h"

extern "C"
{
    extern int yylineno;
    extern FILE* yyin;
    int yyparse(void);
    int yylex(void);
    int yywrap(void);
    void yyerror(const char *str);
    
    int yywrap()
    {
        //only one file
        return 1;
    }
    
    static bool sFailed = false;
    void yyerror(const char *str)
    {
        sFailed = true;
        fprintf(stdout, "[%d]error: %s\n", yylineno, str);
    }
    
    bool loadFile(const char* filePath)
    {
        if (filePath == NULL) {
            return false;
        }
        
        //determine file exists.
        struct stat tmp;
        if (stat(filePath, &tmp) == ENOENT)
            return false;

        //init
        
        //parse
        yyin = fopen(filePath, "r");
        yyparse();
        
        yyin = NULL;
        fclose(yyin);
        
        printf("\nparse complete\n");
        
        return !sFailed;
    }

    
}
    
%}

%union
{
    /* C types shared with lex */
    int intValue;
    char * string;
    #ifdef __cplusplus
    #endif
}

%token <intValue>       INTEGER
%token <string>         ID
%token <string>         WORD
%token <string>         STRING
%token <string>         PREAMBLE
%token <string>         COMMENT

%token COMMA LBRACKET RBRACKET OBRACE EBRACE SEMICOLON QUOTE ASSIGN

/*
%type <document>    document
%type <block>       block
%type <statement>   statement
%type <statements>  statements
%type <array>       array
%type <value>       value
%type <var>         key
 */

%start document

%%

document    :   PREAMBLE block
                {
                    printf("document ");
                }
                ;

block       :   OBRACE statements EBRACE
                {
                    printf("block ");
                }
                ;

statements  :   /* Empty */
              | statements statement

statement   :   variable         ASSIGN value         SEMICOLON
                {
                    printf("statement1 ");
                }
                ;
              | variable COMMENT ASSIGN value         SEMICOLON
                {
                    printf("statement2 ");
                }
                ;              
              | variable         ASSIGN value COMMENT SEMICOLON
                {
                    printf("statement3 ");
                }
                ;              
              | variable COMMENT ASSIGN value COMMENT SEMICOLON  
                {
                    printf("statement4 ");
                }
                ;
              | COMMENT
                {
                    printf("statement5 ");
                }
                ;
                
variable    :   ID
                {
                    printf("var-id ");
                }
                ;
              | WORD
                {
                    printf("var-word ");
                }
                ;               
                
                
value       :   ID
                {
                    printf("value-id ");
                }
                ;
              | INTEGER
                {
                    printf("value-int ");
                }
                ;
              | WORD
                {
                    printf("value-word ");
                }
                ;
              | STRING
                {
                    printf("value-string ");
                }
                ;
              | block
                {
                    printf("value-block ");
                }
                ;
              | array
                {
                    printf("value-array ");
                }
                ;

values      :   /* Empty */
                ;
              |  values value COMMA
                {
                    printf("values ");
                }
              |  values value COMMENT COMMA
                {
                    printf("values-comment ");
                }
                ;

array       :   LBRACKET values RBRACKET
                {
                    printf("array ");
                }
                ;
%%
