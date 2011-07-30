/*
 *  xcodeproj.l
 *  parser
 *
 *  Created by Jonny Yu on 11/17/10.
 *  Copyright 2010 Autodesk Inc. All rights reserved.
 *
 */

%{

#include <stdio.h>
#include <ctype.h>
#include "xcodeproj.hpp"

%}

%option yylineno

%%
\/\/.*$                     {   printf("PREAMBLE "); return PREAMBLE;    } // one line
\/\*[^\*\/]*\*\/            {   printf("COMMENT ");  return COMMENT;     } // inline
[0-9A-F]{24,24}             {   printf("ID ");       return ID;          }
-?[0-9]+                    {   printf("INTEGER ");  return INTEGER;     }
[a-zA-Z\.\/\$\-_\*\<\>]+    {   printf("WORD ");     return WORD;        }
=                           {   printf("ASSIGN ");   return ASSIGN;      }
\"[^"]*\"                   {   printf("STRING ");   return STRING;      }
,                           {   printf("COMMA ");    return COMMA;       }
;                           {   printf("SEMICOLON ");return SEMICOLON;   }
\{                          {   printf("OBRACE ");   return OBRACE;      }
\}                          {   printf("EBRACE ");   return EBRACE;      }
\(                          {   printf("LBRACKET "); return LBRACKET;    }
\)                          {   printf("RBRACKET "); return RBRACKET;    }
\n                          /*ignore EOL*/;
[ \t]+                      /*ignore whitespace*/;

%%
