//
//  parserdef.h
//  xcodeutils
//
//  Created by Yu Yingshen on 11-7-29.
//  Copyright 2011年 HOME. All rights reserved.
//

#ifndef xcodeutils_parserdef_h
#define xcodeutils_parserdef_h

#include <list>
#include <map>
#include <string>




class PBXStatement {
protected:
    PBXStatement();
public:
    virtual ~PBXStatement();
};

typedef std::list<PBXStatement*> PBXStatementList;

class PBXValue {
    std::string mComment;
protected:
    PBXValue();
public:
    virtual ~PBXValue();
    bool hasComment() const;
    std::string comment() const;
    void setComment(const std::string& newComment);
};

typedef std::list<PBXValue*> PBXValueList;

class PBXValueRef : public PBXValue {
    std::string mId;
public:
    PBXValueRef(const std::string& id);
    std::string id() const;
};

class PBXText : public PBXValue {
    std::string mText;
public:
    PBXText(const std::string& text);
    std::string text() const;
};

class PBXInteger : public PBXValue {
    int mIntValue;
public:
    PBXInteger(int intValue);
    int intValue() const;
};


class PBXBlock : public PBXValue {
    PBXStatementList  mStatements;
    PBXBlock(const PBXStatementList& other);
public:
    PBXBlock();
    virtual ~PBXBlock();
    void addStatement(PBXStatement* statement);
    
    PBXStatementList::const_iterator begin() const;
    PBXStatementList::const_iterator end()   const;
};   

class PBXArray : public PBXValue {
    PBXValueList mValues;
public:
    PBXArray();
    virtual ~PBXArray();
    void addValue(PBXValue* value);
    
    PBXValueList::const_iterator begin() const;
    PBXValueList::const_iterator end()   const;
};

class PBXCommentStatement : public PBXStatement {
    std::string mComment;
public:
    PBXCommentStatement(const std::string& comment);
    std::string comment() const;
};

class PBXAssignment : public PBXStatement {
    std::string mKey;
    std::string mKeyComment;
    PBXValue*   mValue;
public:
    PBXAssignment(std::string key, PBXValue* value);
    virtual ~PBXAssignment();
    
    std::string key() const;
    std::string keyComment() const;
    bool        hasKeyComment() const;
    void        setKeyComment(const std::string& keyComment);
    const PBXValue* value() const;
};

class PBXDocument {
    std::string mPreamble;
    PBXBlock*   mBlock;
public:
    PBXDocument(const std::string& preamble,  PBXBlock* block);
    virtual ~PBXDocument();
    const std::string& preamble() const;
    const PBXBlock* block() const;
};

extern "C" bool loadDocument(const char* filePath, PBXDocument **pDoc);


#endif
