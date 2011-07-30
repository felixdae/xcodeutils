//
//  xcodeutils.cpp
//  xcodeutils
//
//  Created by Yu Yingshen on 11-7-29.
//  Copyright 2011年 HOME. All rights reserved.
//

#include "xcodeutils.h"
#include "parserdef.h"

bool XCodeProjDocument::loadFromFile(std::string fileName)
{   
    mFilename = fileName;
    
    return loadFile(fileName.c_str());
}

std::string XCodeProjDocument::fileName() const
{
    return mFilename;
}