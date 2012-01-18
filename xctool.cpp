/*
 *  xctool.cpp
 *  xcodeutils
 *
 *  Created by Jonny Yu on 1/18/12.
 *  Copyright 2012 HOME. All rights reserved.
 *
 */
#include "assert.h"

#include <iostream>
#include <string>
#include <list>
#include "pbxprojdef.h"

static void printHelp(int argc, const char* argv[]);
static void printReferencedProject(const char* rootProject);
static std::list<std::string> referencedProjectsInDoc(const PBXFile* pDoc);
static void printProjects(const std::list<std::string>& projects);

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        printHelp(argc, argv);
        return -1;
    }
    
    std::string projectDir(argv[1]);
    std::string projectFile(projectDir );
    
    PBXFile * pDoc = NULL;
    if (!loadProject(projectFile.c_str(), &pDoc)) {
        std::cout<<"failed to load "<<projectFile<<std::endl;
        return -1;
    }
    
    std::list<std::string> referencedProjects = referencedProjectsInDoc(pDoc);
    
    printProjects(referencedProjects);
    
    return 0;
}


void printHelp(int argc, const char* argv[])
{
    printf("Usage: %s <project.pbxproj>\n", argv[0]);
}

std::list<std::string> referencedProjectsInDoc(const PBXFile* pDoc)
{
    std::list<std::string> refProjects;
    //refProjects.push_back("test.xcodeproj");
    
    const PBXValue* v = pDoc->valueForKeyPath("rootObject.projectReferences");
    const PBXArray* refs = PBXArray::cast(v);
    
    // project has no project references.
    if (!refs)
        return refProjects;
    
    
    for (PBXArray::const_iterator itor = refs->begin(); itor != refs->end(); ++itor) {
        const PBXBlock* projectRef = PBXBlock::cast(*itor);
        assert(projectRef);
        const PBXText* projectPath = PBXText::cast(pDoc->valueForKeyPath(projectRef, "ProjectRef.path"));
        assert(projectPath);

        refProjects.push_back(projectPath->text());
    }
    
    return refProjects;
}

void printProjects(const std::list<std::string>& projects)
{
    if (projects.empty()) {
        std::cout << "Project has no project references" << std::endl;
        return;
    }
    
    for (std::list<std::string>::const_iterator itor = projects.begin(); itor != projects.end(); ++itor) {
        std::cout<< *itor << std::endl;
    }
}