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

extern "C" bool loadFile(const char* filePath);



class PBXStatement {
protected:
    PBXStatement();
public:
    virtual ~PBXStatement();
};

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
    std::list<PBXStatement*>  statements;
    PBXBlock(const std::list<PBXStatement*>& other);
public:
    PBXBlock();
    virtual ~PBXBlock();
    void addStatement(PBXStatement* statement);
    
    std::list<PBXStatement*>::const_iterator begin() const;
    std::list<PBXStatement*>::const_iterator end()   const;
};   

class PBXArray : public PBXValue {
    std::list<PBXValue*> values;
public:
    PBXArray();
    virtual ~PBXArray();
    void addValue(PBXValue* value);
    
    std::list<PBXValue*>::const_iterator begin() const;
    std::list<PBXValue*>::const_iterator end()   const;
};

class PBXCommentStatement : public PBXStatement {
    std::string mComment;
public:
    PBXCommentStatement(std::string comment);
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
    void        setKeyComment(const std::string& keyComment);
    const PBXValue* value() const;
};

class PBXDocument : public PBXBlock {
    std::string mPreamble;
public:
    PBXDocument(std::string preamble);
};


#endif
