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


class PBXItem {
protected:
    PBXItem();
public:
    virtual ~PBXItem();
};

typedef std::list<PBXItem*> PBXItemList;

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
    typedef std::map<const std::string, const PBXValue*> PBXValueMap;
    PBXValueMap       mValueMap;
    PBXItemList  mStatements;
    PBXBlock(const PBXItemList& other);
public:
    PBXBlock();
    virtual ~PBXBlock();
    void addStatement(PBXItem* statement);    
    const PBXValue* valueForKey(const std::string& name) const;
    
    PBXItemList::const_iterator begin() const;
    PBXItemList::const_iterator end()   const;
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

class PBXCommentItem : public PBXItem {
    std::string mComment;
public:
    PBXCommentItem(const std::string& comment);
    std::string comment() const;
};

class PBXAssignment : public PBXItem {
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

class PBXFile {
    std::string mPreamble;
    PBXBlock*   mBlock;
public:
    PBXFile(const std::string& preamble,  PBXBlock* block);
    virtual ~PBXFile();
    const std::string& preamble() const;
    const PBXBlock* block() const;
};

extern "C" bool loadDocument(const char* filePath, PBXFile **pDoc);


#endif
