//
//  parserdef.cpp
//  xcodeutils
//
//  Created by Yu Yingshen on 11-7-31.
//  Copyright 2011年 HOME. All rights reserved.
//

#include "pbxprojdef.h"
#include <iostream>



////////////////////////////////////////////////////////////////////////////////
PBXBlock::PBXBlock()
{
}

PBXBlock::~PBXBlock()
{
    mValueMap.clear();
    
    std::list<PBXItem*>::iterator iter = mStatements.begin();
    std::list<PBXItem*>::iterator end  = mStatements.end();
    for (; iter != end; ++iter) {
        delete *iter;
    }
}

void PBXBlock::addStatement(PBXItem* statement)
{
    mStatements.push_back(statement);
    const PBXAssignment* assign = dynamic_cast<const PBXAssignment*>(statement);
    if (assign) {
        std::pair<const std::string, const PBXValue*> valuePair(assign->key(), assign->value());
        mValueMap.insert(valuePair);
    }
}
const PBXValue* PBXBlock::valueForKey(const char* name) const
{
    PBXValueMap::const_iterator itor = mValueMap.find(name);
    if (itor != mValueMap.end()) {
        return  itor->second;
    }
    return NULL;
}
    
std::list<PBXItem*>::const_iterator PBXBlock::begin() const
{
    return mStatements.begin();
}

std::list<PBXItem*>::const_iterator PBXBlock::end()   const
{
    return mStatements.end();
}

////////////////////////////////////////////////////////////////////////////////
PBXValue::PBXValue()
{
}

PBXValue::~PBXValue()
{
}
bool PBXValue::hasComment() const
{
    return !mComment.empty();
}
const char* PBXValue::comment() const
{
    return mComment.c_str();
}
void PBXValue::setComment(const char* newComment)
{
    mComment = newComment;
}

////////////////////////////////////////////////////////////////////////////////
PBXValueRef::PBXValueRef(const char* id)
{
    mId = id;
}
const char* PBXValueRef::id() const
{   
    return mId.c_str();
}

////////////////////////////////////////////////////////////////////////////////
PBXText::PBXText(const char* text)
{
    mText = text;
}
const char* PBXText::text() const
{
    return mText.c_str();
}


////////////////////////////////////////////////////////////////////////////////
PBXInteger::PBXInteger(int intValue)
{
    mIntValue = intValue;
}
int PBXInteger::intValue() const
{
    return mIntValue;
}

////////////////////////////////////////////////////////////////////////////////
PBXItem::PBXItem()
{
}

PBXItem::~PBXItem()
{
}

////////////////////////////////////////////////////////////////////////////////
PBXAssignment::PBXAssignment(const char* key, PBXValue* value)
{
    mKey = key;
    mValue = value;
}

PBXAssignment::~PBXAssignment()
{
    delete mValue;
}

const char* PBXAssignment::key() const
{
    return mKey.c_str();
}

const char* PBXAssignment::keyComment() const
{
    return mKeyComment.c_str();
}

void PBXAssignment::setKeyComment(const char* keyComment)
{
    mKeyComment = keyComment;
}

bool PBXAssignment::hasKeyComment() const
{
    return !mKeyComment.empty();
}

const PBXValue* PBXAssignment::value() const
{
    return mValue;
}

////////////////////////////////////////////////////////////////////////////////
PBXArray::PBXArray()
{
}
PBXArray::~PBXArray()
{
    std::list<PBXValue*>::iterator iter = mValues.begin();
    std::list<PBXValue*>::iterator end  = mValues.end();
    for (; iter != end; ++iter) {
        delete *iter;
    }
}
void PBXArray::addValue(PBXValue* value)
{
    mValues.push_back(value);
}
std::list<PBXValue*>::const_iterator PBXArray::begin() const
{
    return mValues.begin();
}
std::list<PBXValue*>::const_iterator PBXArray::end()   const
{
    return mValues.end();
}

////////////////////////////////////////////////////////////////////////////////
PBXCommentItem::PBXCommentItem(const std::string& comment)
{
    mComment = comment;
}

std::string PBXCommentItem::comment() const
{
    return mComment;
}

////////////////////////////////////////////////////////////////////////////////
PBXFile::PBXFile()
{
    mBlock = NULL;
}
PBXFile::~PBXFile()
{
    delete mBlock;
}

const char* PBXFile::preamble() const
{
    return mPreamble.c_str();
}

void PBXFile::setPreamble(const char* preamble)
{
    mPreamble = preamble;
}

const PBXBlock* PBXFile::block() const
{
    return mBlock;
}

void PBXFile::setBlock(PBXBlock* block)
{
    mBlock = block;
}


const PBXValue* PBXFile::valueForKey(const char* keyName) const
{
    return mBlock ? mBlock->valueForKey(keyName) : NULL;
}

const PBXValue* PBXFile::deref(const PBXValueRef* ref) const
{
    if (!ref) return NULL;
    
    const PBXBlock* objects = dynamic_cast<const PBXBlock*>(valueForKey("objects"));
    return objects ? objects->valueForKey(ref->id()) : NULL;
}





