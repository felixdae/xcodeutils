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
const PBXValue* PBXBlock::valueForKey(const std::string& name) const
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
std::string PBXValue::comment() const
{
    return mComment;
}
void PBXValue::setComment(const std::string& newComment)
{
    mComment = newComment;
}

////////////////////////////////////////////////////////////////////////////////
PBXValueRef::PBXValueRef(const std::string& id)
{
    mId = id;
}
std::string PBXValueRef::id() const
{   
    return mId;
}

////////////////////////////////////////////////////////////////////////////////
PBXText::PBXText(const std::string& text)
{
    mText = text;
}
std::string PBXText::text() const
{
    return mText;
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
PBXAssignment::PBXAssignment(std::string key, PBXValue* value)
{
    mKey = key;
    mValue = value;
}

PBXAssignment::~PBXAssignment()
{
    delete mValue;
}

std::string PBXAssignment::key() const
{
    return mKey;
}

std::string PBXAssignment::keyComment() const
{
    return mKeyComment;
}

void PBXAssignment::setKeyComment(const std::string& keyComment)
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
PBXFile::PBXFile(const std::string& preamble, PBXBlock* block)
{
    mPreamble = preamble;
    mBlock    = block;
}

PBXFile::~PBXFile()
{
    delete mBlock;
}

const std::string& PBXFile::preamble() const
{
    return mPreamble;
}


const PBXBlock* PBXFile::block() const
{
    return mBlock;
}