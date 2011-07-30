//
//  xcodeutils.h
//  xcodeutils
//
//  Created by Yu Yingshen on 11-7-29.
//  Copyright 2011年 HOME. All rights reserved.
//

#ifndef xcodeutils_xcodeutils_h
#define xcodeutils_xcodeutils_h

#include <string>
#include <vector>
#include <map>

class XCodeProjDocument
{
    std::string mFilename;
public:
    bool loadFromFile(std::string fileName);
    std::string fileName() const;
};

#endif
