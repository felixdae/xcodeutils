//
//  parserdef.cpp
//  xcodeutils
//
//  Created by Yu Yingshen on 11-7-31.
//  Copyright 2011年 HOME. All rights reserved.
//

#include "parserdef.h"

#include <iostream>



////////////////////////////////////////////////////////////////////////////////
PBXBlock::PBXBlock()
{
}

PBXBlock::~PBXBlock()
{
    std::list<PBXStatement*>::iterator iter = mStatements.begin();
    std::list<PBXStatement*>::iterator end  = mStatements.end();
    for (; iter != end; ++iter) {
        delete *iter;
    }
}

void PBXBlock::addStatement(PBXStatement* statement)
{
    mStatements.push_back(statement);
}
    
std::list<PBXStatement*>::const_iterator PBXBlock::begin() const
{
    return mStatements.begin();
}

std::list<PBXStatement*>::const_iterator PBXBlock::end()   const
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
PBXStatement::PBXStatement()
{
}

PBXStatement::~PBXStatement()
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
PBXCommentStatement::PBXCommentStatement(const std::string& comment)
{
    mComment = comment;
}

std::string PBXCommentStatement::comment() const
{
    return mComment;
}

////////////////////////////////////////////////////////////////////////////////
PBXDocument::PBXDocument(const std::string& preamble, PBXBlock* block)
{
    mPreamble = preamble;
    mBlock    = block;
}

PBXDocument::~PBXDocument()
{
    delete mBlock;
}

const std::string& PBXDocument::preamble() const
{
    return mPreamble;
}


const PBXBlock* PBXDocument::block() const
{
    return mBlock;
}