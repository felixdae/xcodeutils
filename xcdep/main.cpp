//
//  main.cpp
//  xcdep
//
//  Created by Yu Yingshen on 11-8-3.
//  Copyright 2011 HOME. All rights reserved.
//

#include <iostream>
#include <string>
#include "pbxprojdef.h"

static void printHelp();
static void doWork(const PBXFile* pDoc);

int main (int argc, const char * argv[])
{

    if (argc < 2) {
        printHelp();
        return -1;
    }
    
    std::string projectDir(argv[1]);
    std::string projectFile(projectDir );
    
    PBXFile * pDoc = NULL;
    if (!loadDocument(projectFile.c_str(), &pDoc)) {
        std::cout<<"failed to load "<<projectFile<<std::endl;
        return -1;
    }
    
    doWork(pDoc);
    
    delete pDoc;
    return 0;
}


void printHelp()
{
    std::cout << "xcdep <project.pbxproj>" << std::endl;
}

static void printTarget(const PBXFile* doc, const PBXBlock* target)
{
    const PBXText* nameNode = dynamic_cast<const PBXText*>(target->valueForKey("name"));
    if (!nameNode) return;
        
    std::cout << nameNode->text() << std::endl;
    
    const PBXArray* depRefs = dynamic_cast<const PBXArray*>(target->valueForKey("dependencies"));
    if (!depRefs) return;
    
    for (PBXArray::const_iterator itor = depRefs->begin(); itor != depRefs->end(); ++itor) 
    {
        const PBXValueRef* depRef = dynamic_cast<const PBXValueRef*>(*itor);
        const PBXBlock*    dep    = dynamic_cast<const PBXBlock*>(doc->deref(depRef));
        if (!dep) continue;
    
        const PBXText* depNameNode = dynamic_cast<const PBXText*>(dep->valueForKey("name"));
        if (!depNameNode) continue;
        
        std::cout << "\t" << depNameNode->text() << std::endl;
    }
}

void doWork(const PBXFile* doc)
{
    const PBXValueRef* rootObjRef = dynamic_cast<const PBXValueRef*>(doc->valueForKey("rootObject"));
    if (!rootObjRef) return;
    
    const PBXBlock* projObj = dynamic_cast<const PBXBlock*>(doc->deref(rootObjRef));
    if (!projObj) return;
            
    const PBXArray* targets = dynamic_cast<const PBXArray*>(projObj->valueForKey("targets"));
    if (!targets) return;


    for (PBXArray::const_iterator itor = targets->begin(); itor != targets->end();  ++itor) 
    {
        const PBXValueRef* targetRef = dynamic_cast<const PBXValueRef*>(*itor);
        const PBXBlock* target = dynamic_cast<const PBXBlock*>(doc->deref(targetRef));
        if (!target) continue;
        
        printTarget(doc, target);
    }
}



