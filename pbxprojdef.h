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
    const char* comment() const;
    void setComment(const char* newComment);
};

typedef std::list<PBXValue*> PBXValueList;

class PBXValueRef : public PBXValue {
    std::string mId;
public:
    PBXValueRef(const char* id);
    const char* id() const;
};

class PBXText : public PBXValue {
    std::string mText;
public:
    PBXText(const char* text);
    const char* text() const;
};

class PBXInteger : public PBXValue {
    int mIntValue;
public:
    PBXInteger(int intValue);
    int intValue() const;
};


class PBXBlock : public PBXValue {
    typedef std::map<const std::string, const PBXValue*> PBXValueMap;
    PBXValueMap  mValueMap;
    PBXItemList  mStatements;
    PBXBlock(const PBXItemList& other);
public:
    typedef PBXItemList::const_iterator    const_iterator;
    typedef PBXItemList::iterator          iterator;
    
    PBXBlock();
    virtual ~PBXBlock();
    void addStatement(PBXItem* statement);    
    const PBXValue* valueForKey(const char* name) const;
    
    PBXItemList::const_iterator begin() const;
    PBXItemList::const_iterator end()   const;
};   

class PBXArray : public PBXValue {
    PBXValueList mValues;
public:
    typedef PBXValueList::const_iterator    const_iterator;
    typedef PBXValueList::iterator          iterator;
    
    PBXArray();
    virtual ~PBXArray();
    void addValue(PBXValue* value);
    
    const_iterator begin() const;
    const_iterator end()   const;
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
    PBXAssignment(const char* key, PBXValue* value);
    virtual ~PBXAssignment();
    
    const char* key() const;
    const char* keyComment() const;
    bool        hasKeyComment() const;
    void        setKeyComment(const char* keyComment);
    const PBXValue* value() const;
};

class PBXFile {
    std::string mPreamble;
    PBXBlock*   mBlock;
    typedef std::map<const std::string, const PBXValue*> PBXValueMap;
public:
    PBXFile();
    virtual ~PBXFile();
    const char* preamble() const;
    void setPreamble(const char* preamble);
    const PBXBlock* block() const;
    void setBlock(PBXBlock* block);
    
    const PBXValue* valueForKey(const char* keyName) const;
    const PBXValue* deref(const PBXValueRef* ref) const;
};

extern "C" bool loadDocument(const char* filePath, PBXFile **pDoc);


#endif
