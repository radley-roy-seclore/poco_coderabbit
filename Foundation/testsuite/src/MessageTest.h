//
// MessageTest.h
//
// Definition of the MessageTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MessageTest_INCLUDED
#define MessageTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class MessageTest: public CppUnit::TestCase
{
public:
	MessageTest(const std::string& name);
	~MessageTest();

	void testDefaultConstructor();
	void testConstructorWithParams();
	void testConstructorWithMove();
	void testConstructorWithFile();
	void testConstructorWithFileAndMove();
	void testCopyConstructor();
	void testMoveConstructor();
	void testCopyWithText();
	void testCopyAssignment();
	void testMoveAssignment();
	void testSwap();
	void testSettersGetters();
	void testPriority();
	void testTime();
	void testThreadInfo();
	void testProcessInfo();
	void testSourceFileInfo();
	void testParameters();
	void testParameterAccess();
	void testParameterOperator();
	void testParameterGetWithDefault();
	void testParameterAll();
	void testEmptyParameters();
	void testMultipleParameters();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // MessageTest_INCLUDED