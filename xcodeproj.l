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
\/\/.*$                     {   return PREAMBLE;    } // one line
\/\*[^\*\/]*\*\/            {   return COMMENT;     } // inline
[0-9A-F]{24,24}             {   return ID;          }
-?[0-9]+                    {   return INTEGER;     }
[a-zA-Z\.\/\$\-_\*\<\>]+    {   return WORD;        }
=                           {   return ASSIGN;      }
\"[^"]*\"                   {   return STRING;      }
,                           {   return COMMA;       }
;                           {   return SEMICOLON;   }
\{                          {   return OBRACE;      }
\}                          {   return EBRACE;      }
\(                          {   return LBRACKET;    }
\)                          {   return RBRACKET;    }
\n                          /*ignore EOL*/;
[ \t]+                      /*ignore whitespace*/;

%%
