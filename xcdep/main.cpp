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
static std::string defineTargetShape(const PBXFile* doc,const PBXBlock* target);

int main (int argc, const char * argv[])
{

    if (argc < 2) {
        printHelp();
        return -1;
    }
    
    std::string projectDir(argv[1]);
    std::string projectFile(projectDir );
    
    PBXFile * pDoc = NULL;
    if (!loadProject(projectFile.c_str(), &pDoc)) {
        std::cout<<"failed to load "<<projectFile<<std::endl;
        return -1;
    }
    
    doWork(pDoc);
    
    delete pDoc;
    return 0;
}


static void printHelp()
{
    std::cout << "xcdep <project.pbxproj>" << std::endl;
}

static std::string defineTargetShape(const PBXFile* doc,const PBXBlock* target)
{
    const char* shapeDefinitions[] = {
        " [style=filled, color=\"#FF0000\"];",
        " [style=filled, color=\"#FFFF00\"];",
        " [style=filled, color=\"#00FF00\"];"
    };
    
    int useLLVMCount = 0;
    const PBXArray* buildCfgList = dynamic_cast<const PBXArray*>(doc->valueForKeyPath("rootObject.buildConfigurationList.buildConfigurations"));
    for (PBXArray::const_iterator itor = buildCfgList->begin(); itor != buildCfgList->end(); ++itor) 
    {
        const PBXValueRef* buildCfgRef = PBXValueRef::cast(*itor);
        const PBXBlock* buildCfg = PBXBlock::cast(doc->deref(buildCfgRef));
        const PBXText* name = dynamic_cast<const PBXText*> (doc->valueForKeyPath(buildCfg, "baseConfigurationReference.name"));
        if (!name) {
            name = dynamic_cast<const PBXText*> (doc->valueForKeyPath(buildCfg, "baseConfigurationReference.path"));
        }
        
        std::string configFile(name->text());
        
        if (configFile.find("LLVM") != std::string::npos) 
            useLLVMCount++;
    }
    
    int state;
    if (useLLVMCount == 0) {
        state = 0; // not using LLVM
    }
    else if(useLLVMCount < buildCfgList->count()) {
        state = 1; // partially using LLVM
    }
    else {
        state = 2; // all using LLVM
    }

    return shapeDefinitions[state];
}

static void printTarget(const PBXFile* doc, const PBXBlock* target)
{
    const PBXText* nameNode = dynamic_cast<const PBXText*>(target->valueForKey("name"));
    if (!nameNode) 
        return;
            
    const PBXArray* depRefs = dynamic_cast<const PBXArray*>(target->valueForKey("dependencies"));
    if (!depRefs) 
        return;
	
    //define the target shape
    defineTargetShape(doc, target);
    std::cout << nameNode->text() << defineTargetShape(doc, target) << std::endl;
	
    // if this project doesn't have any dependent projects
    if (depRefs->count() == 0)
		return;
    
    for (PBXArray::const_iterator itor = depRefs->begin(); itor != depRefs->end(); ++itor) 
    {
        const PBXValueRef* depRef = dynamic_cast<const PBXValueRef*>(*itor);
        const PBXBlock*    dep    = dynamic_cast<const PBXBlock*>(doc->deref(depRef));
        if (!dep) continue;
    
        const PBXText* depNameNode = dynamic_cast<const PBXText*>(dep->valueForKey("name"));
        if (!depNameNode) continue;
        
        std::cout << nameNode->text() << defineTargetShape(doc, target) << std::endl;
        std::cout << nameNode->text() << " -> " << depNameNode->text() << ";" << std::endl;
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



