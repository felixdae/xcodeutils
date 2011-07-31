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
    
    static PBXDocument* gpDoc = NULL;
    bool loadDocument(const char* filePath, PBXDocument **pDoc)
    {
        if (filePath == NULL || pDoc == NULL)
            return false;
        
        //determine file exists.
        struct stat tmp;
        if (stat(filePath, &tmp) == ENOENT)
            return false;

        //init
        *pDoc = NULL;
        gpDoc = NULL;
        sFailed = false;
        
        //parse
        yyin = fopen(filePath, "r");
        yyparse();
        
        if (sFailed)
            delete gpDoc;
        else
            *pDoc = gpDoc;
        
        gpDoc = NULL;
        yyin = NULL;
        fclose(yyin);
                
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
    PBXDocument     * document;
    PBXStatement    * statement;
    PBXBlock        * block;
    PBXArray        * array;
    PBXValue        * value;
    #endif
}

%token <intValue>       INTEGER
%token <string>         ID
%token <string>         WORD
%token <string>         STRING
%token <string>         PREAMBLE
%token <string>         COMMENT


%token COMMA LBRACKET RBRACKET OBRACE EBRACE SEMICOLON QUOTE ASSIGN

%type <string>      variable
%type <document>    document
%type <block>       block
%type <array>       array
%type <statement>   statement
%type <block>       statements
%type <value>       value
%type <array>       values

%start document

%%

document    :   PREAMBLE block
                {
                    $$ = new PBXDocument($1, $2);
                    gpDoc = $$;
                    //printf("document ");
                }
                ;

block       :   OBRACE statements EBRACE
                {
                    $$ = $2;
                    //printf("block ");
                }
                ;

statements  :   /* Empty */
                {
                    $$ = new PBXBlock;
                }
                ;
              | statements statement
                {
                    $1->addStatement($2);
                    $$ = $1;
                }
                ;

statement   :   variable         ASSIGN value         SEMICOLON
                {
                    $$ = new PBXAssignment($1, $3);
                    //printf("statement1 ");
                }
                ;
              | variable COMMENT ASSIGN value         SEMICOLON
                {
                    PBXAssignment *assign = new PBXAssignment($1, $4);
                    assign->setKeyComment($2);
                    $$ = assign;
                    //printf("statement2 ");
                }
                ;              
              | variable         ASSIGN value COMMENT SEMICOLON
                {
                    $3->setComment($4);
                    $$ = new PBXAssignment($1, $3);
                    //printf("statement3 ");
                }
                ;              
              | variable COMMENT ASSIGN value COMMENT SEMICOLON  
                {
                    $4->setComment($5);
                    PBXAssignment *assign = new PBXAssignment($1, $4);
                    assign->setKeyComment($2);
                    $$ = assign;
                    //printf("statement4 ");
                }
                ;
              | COMMENT
                {
                    $$ = new PBXCommentStatement($1);
                    //printf("statement5 ");
                }
                ;
                
variable    :   ID
                {
                    $$ = $1;
                    //printf("var-id ");
                }
                ;
              | WORD
                {
                    $$ = $1;
                    //printf("var-word ");
                }
                ;               
                
                
value       :   ID
                {
                    $$ = new PBXValueRef($1);
                    //printf("value-id ");
                }
                ;
              | INTEGER
                {
                    $$ = new PBXInteger($1);
                    //printf("value-int ");
                }
                ;
              | WORD
                {
                    $$ = new PBXText($1);
                    //printf("value-word ");
                }
                ;
              | STRING
                {
                    $$ = new PBXText($1);
                    //printf("value-string ");
                }
                ;
              | block
                {
                    $$ = $1;
                    //printf("value-block ");
                }
                ;
              | array
                {
                    $$ = $1;
                    //printf("value-array ");
                }
                ;

values      :   /* Empty */
                {
                    $$ = new PBXArray;
                }
                ;
              |  values value COMMA
                {
                    $1->addValue($2);
                    $$ = $1;
                    //printf("values ");
                }
              |  values value COMMENT COMMA
                {
                    $2->setComment($3);
                    $1->addValue($2);
                    $$ = $1;
                    //printf("values-comment ");
                }
                ;

array       :   LBRACKET values RBRACKET
                {
                    $$ = $2;
                    //printf("array ");
                }
                ;
%%
