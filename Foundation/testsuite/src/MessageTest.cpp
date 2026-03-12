//
// MessageTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MessageTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Message.h"
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"
#include <thread>


using Poco::Message;
using Poco::Timestamp;
using Poco::NotFoundException;


MessageTest::MessageTest(const std::string& name): CppUnit::TestCase(name)
{
}


MessageTest::~MessageTest()
{
}


void MessageTest::testDefaultConstructor()
{
	Message msg;
	assertTrue (msg.getSource().empty());
	assertTrue (msg.getText().empty());
	assertTrue (msg.getPriority() == Message::PRIO_FATAL);
	assertTrue (msg.getTime() != 0);
	assertTrue (msg.getTid() != 0);
	assertTrue (msg.getPid() != 0);
	assertTrue (msg.getSourceFile() == nullptr);
	assertTrue (msg.getSourceLine() == 0);
}


void MessageTest::testConstructorWithParams()
{
	Message msg("TestSource", "Test message", Message::PRIO_ERROR);
	assertTrue (msg.getSource() == "TestSource");
	assertTrue (msg.getText() == "Test message");
	assertTrue (msg.getPriority() == Message::PRIO_ERROR);
	assertTrue (msg.getTime() != 0);
	assertTrue (msg.getTid() != 0);
	assertTrue (msg.getPid() != 0);
	assertTrue (msg.getSourceFile() == nullptr);
	assertTrue (msg.getSourceLine() == 0);
}


void MessageTest::testConstructorWithMove()
{
	std::string text = "Moveable message";
	Message msg("MoveSource", std::move(text), Message::PRIO_WARNING);
	assertTrue (msg.getSource() == "MoveSource");
	assertTrue (msg.getText() == "Moveable message");
	assertTrue (msg.getPriority() == Message::PRIO_WARNING);
	// text should be moved from (empty or unspecified state)
}


void MessageTest::testConstructorWithFile()
{
	const char* file = __FILE__;
	Message msg("FileSource", "File message", Message::PRIO_DEBUG, file, 100);
	assertTrue (msg.getSource() == "FileSource");
	assertTrue (msg.getText() == "File message");
	assertTrue (msg.getPriority() == Message::PRIO_DEBUG);
	assertTrue (msg.getSourceFile() == file);
	assertTrue (msg.getSourceLine() == 100);
}


void MessageTest::testConstructorWithFileAndMove()
{
	const char* file = __FILE__;
	std::string text = "Move file message";
	Message msg("FileMoveSource", std::move(text), Message::PRIO_INFORMATION, file, 200);
	assertTrue (msg.getSource() == "FileMoveSource");
	assertTrue (msg.getText() == "Move file message");
	assertTrue (msg.getPriority() == Message::PRIO_INFORMATION);
	assertTrue (msg.getSourceFile() == file);
	assertTrue (msg.getSourceLine() == 200);
}


void MessageTest::testCopyConstructor()
{
	Message msg1("Source1", "Text1", Message::PRIO_CRITICAL);
	msg1.set("param1", "value1");
	msg1.setSourceFile(__FILE__);
	msg1.setSourceLine(123);

	Message msg2(msg1);
	assertTrue (msg2.getSource() == msg1.getSource());
	assertTrue (msg2.getText() == msg1.getText());
	assertTrue (msg2.getPriority() == msg1.getPriority());
	assertTrue (msg2.getTime() == msg1.getTime());
	assertTrue (msg2.getTid() == msg1.getTid());
	assertTrue (msg2.getPid() == msg1.getPid());
	assertTrue (msg2.getSourceFile() == msg1.getSourceFile());
	assertTrue (msg2.getSourceLine() == msg1.getSourceLine());
	assertTrue (msg2.has("param1"));
	assertTrue (msg2.get("param1") == "value1");
}


void MessageTest::testMoveConstructor()
{
	Message msg1("Source1", "Text1", Message::PRIO_FATAL);
	msg1.set("param1", "value1");
	const char* file = msg1.getSourceFile();
	long line = msg1.getSourceLine();
	Timestamp ts = msg1.getTime();

	Message msg2(std::move(msg1));
	assertTrue (msg2.getSource() == "Source1");
	assertTrue (msg2.getText() == "Text1");
	assertTrue (msg2.getPriority() == Message::PRIO_FATAL);
	assertTrue (msg2.getTime() == ts);
	assertTrue (msg2.getSourceFile() == file);
	assertTrue (msg2.getSourceLine() == line);
}


void MessageTest::testCopyWithText()
{
	Message msg1("Source1", "Text1", Message::PRIO_NOTICE);
	msg1.set("param1", "value1");
	msg1.setSourceFile(__FILE__);
	msg1.setSourceLine(456);

	Message msg2(msg1, "Text2");
	assertTrue (msg2.getSource() == msg1.getSource());
	assertTrue (msg2.getText() == "Text2");
	assertTrue (msg2.getPriority() == msg1.getPriority());
	assertTrue (msg2.getTime() == msg1.getTime());
	assertTrue (msg2.getTid() == msg1.getTid());
	assertTrue (msg2.getPid() == msg1.getPid());
	assertTrue (msg2.getSourceFile() == msg1.getSourceFile());
	assertTrue (msg2.getSourceLine() == msg1.getSourceLine());
	assertTrue (msg2.has("param1"));
	assertTrue (msg2.get("param1") == "value1");
}


void MessageTest::testCopyAssignment()
{
	Message msg1("Source1", "Text1", Message::PRIO_ERROR);
	msg1.set("param1", "value1");

	Message msg2;
	msg2 = msg1;
	assertTrue (msg2.getSource() == msg1.getSource());
	assertTrue (msg2.getText() == msg1.getText());
	assertTrue (msg2.getPriority() == msg1.getPriority());
	assertTrue (msg2.has("param1"));
	assertTrue (msg2.get("param1") == "value1");
}


void MessageTest::testMoveAssignment()
{
	Message msg1("Source1", "Text1", Message::PRIO_WARNING);
	msg1.set("param1", "value1");

	Message msg2;
	msg2 = std::move(msg1);
	assertTrue (msg2.getSource() == "Source1");
	assertTrue (msg2.getText() == "Text1");
	assertTrue (msg2.getPriority() == Message::PRIO_WARNING);
}


void MessageTest::testSwap()
{
	Message msg1("Source1", "Text1", Message::PRIO_DEBUG);
	msg1.set("param1", "value1");

	Message msg2("Source2", "Text2", Message::PRIO_TRACE);
	msg2.set("param2", "value2");

	msg1.swap(msg2);

	assertTrue (msg1.getSource() == "Source2");
	assertTrue (msg1.getText() == "Text2");
	assertTrue (msg1.getPriority() == Message::PRIO_TRACE);
	assertTrue (msg1.has("param2"));
	assertTrue (msg1.get("param2") == "value2");
	assertTrue (!msg1.has("param1"));

	assertTrue (msg2.getSource() == "Source1");
	assertTrue (msg2.getText() == "Text1");
	assertTrue (msg2.getPriority() == Message::PRIO_DEBUG);
	assertTrue (msg2.has("param1"));
	assertTrue (msg2.get("param1") == "value1");
	assertTrue (!msg2.has("param2"));

	// Test free function swap
	swap(msg1, msg2);
	assertTrue (msg1.getSource() == "Source1");
	assertTrue (msg2.getSource() == "Source2");
}


void MessageTest::testSettersGetters()
{
	Message msg;

	msg.setSource("TestSource");
	assertTrue (msg.getSource() == "TestSource");

	msg.setText("TestText");
	assertTrue (msg.getText() == "TestText");

	msg.setPriority(Message::PRIO_INFORMATION);
	assertTrue (msg.getPriority() == Message::PRIO_INFORMATION);

	msg.setThread("TestThread");
	assertTrue (msg.getThread() == "TestThread");
}


void MessageTest::testPriority()
{
	Message msg;

	msg.setPriority(Message::PRIO_FATAL);
	assertTrue (msg.getPriority() == Message::PRIO_FATAL);

	msg.setPriority(Message::PRIO_CRITICAL);
	assertTrue (msg.getPriority() == Message::PRIO_CRITICAL);

	msg.setPriority(Message::PRIO_ERROR);
	assertTrue (msg.getPriority() == Message::PRIO_ERROR);

	msg.setPriority(Message::PRIO_WARNING);
	assertTrue (msg.getPriority() == Message::PRIO_WARNING);

	msg.setPriority(Message::PRIO_NOTICE);
	assertTrue (msg.getPriority() == Message::PRIO_NOTICE);

	msg.setPriority(Message::PRIO_INFORMATION);
	assertTrue (msg.getPriority() == Message::PRIO_INFORMATION);

	msg.setPriority(Message::PRIO_DEBUG);
	assertTrue (msg.getPriority() == Message::PRIO_DEBUG);

	msg.setPriority(Message::PRIO_TRACE);
	assertTrue (msg.getPriority() == Message::PRIO_TRACE);

	// Test priority ordering
	assertTrue (Message::PRIO_FATAL < Message::PRIO_CRITICAL);
	assertTrue (Message::PRIO_CRITICAL < Message::PRIO_ERROR);
	assertTrue (Message::PRIO_ERROR < Message::PRIO_WARNING);
	assertTrue (Message::PRIO_WARNING < Message::PRIO_NOTICE);
	assertTrue (Message::PRIO_NOTICE < Message::PRIO_INFORMATION);
	assertTrue (Message::PRIO_INFORMATION < Message::PRIO_DEBUG);
	assertTrue (Message::PRIO_DEBUG < Message::PRIO_TRACE);
}


void MessageTest::testTime()
{
	Timestamp before;
	Message msg;
	Timestamp after;

	assertTrue (msg.getTime() >= before);
	assertTrue (msg.getTime() <= after);

	Timestamp newTime = Timestamp::fromEpochTime(1000000000);
	msg.setTime(newTime);
	assertTrue (msg.getTime() == newTime);
}


void MessageTest::testThreadInfo()
{
	Message msg;

	// Default tid should be non-zero
	assertTrue (msg.getTid() != 0);
	assertTrue (msg.getOsTid() != 0);

	msg.setTid(12345);
	assertTrue (msg.getTid() == 12345);

	msg.setThread("MainThread");
	assertTrue (msg.getThread() == "MainThread");
}


void MessageTest::testProcessInfo()
{
	Message msg;

	// Default pid should be non-zero
	long origPid = msg.getPid();
	assertTrue (origPid != 0);

	msg.setPid(99999);
	assertTrue (msg.getPid() == 99999);
}


void MessageTest::testSourceFileInfo()
{
	Message msg;

	assertTrue (msg.getSourceFile() == nullptr);
	assertTrue (msg.getSourceLine() == 0);

	const char* file = "test.cpp";
	msg.setSourceFile(file);
	assertTrue (msg.getSourceFile() == file);

	msg.setSourceLine(42);
	assertTrue (msg.getSourceLine() == 42);
}


void MessageTest::testParameters()
{
	Message msg;

	assertTrue (!msg.has("param1"));

	msg.set("param1", "value1");
	assertTrue (msg.has("param1"));
	assertTrue (msg.get("param1") == "value1");

	msg.set("param1", "newvalue1");
	assertTrue (msg.get("param1") == "newvalue1");
}


void MessageTest::testParameterAccess()
{
	Message msg;
	msg.set("param1", "value1");

	// Test const access
	const Message& constMsg = msg;
	assertTrue (constMsg.get("param1") == "value1");
	assertTrue (constMsg["param1"] == "value1");

	// Test exception for non-existent parameter
	try
	{
		constMsg.get("nonexistent");
		failmsg("Should throw NotFoundException");
	}
	catch (const NotFoundException&)
	{
		// Expected
	}
}


void MessageTest::testParameterOperator()
{
	Message msg;

	// Non-const operator[] should create parameter if not exists
	msg["param1"] = "value1";
	assertTrue (msg.has("param1"));
	assertTrue (msg["param1"] == "value1");

	// Should be able to modify existing parameter
	msg["param1"] = "newvalue1";
	assertTrue (msg["param1"] == "newvalue1");

	// Access non-existent parameter creates it with empty string
	std::string& val = msg["param2"];
	assertTrue (msg.has("param2"));
	assertTrue (val.empty());
	val = "value2";
	assertTrue (msg["param2"] == "value2");
}


void MessageTest::testParameterGetWithDefault()
{
	Message msg;
	msg.set("param1", "value1");

	assertTrue (msg.get("param1", "default") == "value1");
	assertTrue (msg.get("nonexistent", "default") == "default");
	assertTrue (!msg.has("nonexistent")); // Should not create the parameter
}


void MessageTest::testParameterAll()
{
	Message msg;
	msg.set("param1", "value1");
	msg.set("param2", "value2");
	msg.set("param3", "value3");

	const Message::StringMap& all = msg.getAll();
	assertTrue (all.size() == 3);
	assertTrue (all.find("param1") != all.end());
	assertTrue (all.find("param2") != all.end());
	assertTrue (all.find("param3") != all.end());
	assertTrue (all.at("param1") == "value1");
	assertTrue (all.at("param2") == "value2");
	assertTrue (all.at("param3") == "value3");
}


void MessageTest::testEmptyParameters()
{
	Message msg;
	const Message::StringMap& all = msg.getAll();
	assertTrue (all.empty());
	assertTrue (!msg.has("anything"));
}


void MessageTest::testMultipleParameters()
{
	Message msg("TestSource", "TestText", Message::PRIO_ERROR);

	// Add multiple parameters
	msg.set("user", "admin");
	msg.set("action", "login");
	msg.set("ip", "192.168.1.1");
	msg.set("timestamp", "2024-01-01");

	assertTrue (msg.has("user"));
	assertTrue (msg.has("action"));
	assertTrue (msg.has("ip"));
	assertTrue (msg.has("timestamp"));

	assertTrue (msg.get("user") == "admin");
	assertTrue (msg.get("action") == "login");
	assertTrue (msg.get("ip") == "192.168.1.1");
	assertTrue (msg.get("timestamp") == "2024-01-01");

	// Test parameter independence
	msg.set("user", "guest");
	assertTrue (msg.get("user") == "guest");
	assertTrue (msg.get("action") == "login"); // Other params unchanged

	// Test copy preserves all parameters
	Message msg2(msg);
	assertTrue (msg2.has("user"));
	assertTrue (msg2.has("action"));
	assertTrue (msg2.has("ip"));
	assertTrue (msg2.has("timestamp"));
	assertTrue (msg2.get("user") == "guest");
}


void MessageTest::setUp()
{
}


void MessageTest::tearDown()
{
}


CppUnit::Test* MessageTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MessageTest");

	CppUnit_addTest(pSuite, MessageTest, testDefaultConstructor);
	CppUnit_addTest(pSuite, MessageTest, testConstructorWithParams);
	CppUnit_addTest(pSuite, MessageTest, testConstructorWithMove);
	CppUnit_addTest(pSuite, MessageTest, testConstructorWithFile);
	CppUnit_addTest(pSuite, MessageTest, testConstructorWithFileAndMove);
	CppUnit_addTest(pSuite, MessageTest, testCopyConstructor);
	CppUnit_addTest(pSuite, MessageTest, testMoveConstructor);
	CppUnit_addTest(pSuite, MessageTest, testCopyWithText);
	CppUnit_addTest(pSuite, MessageTest, testCopyAssignment);
	CppUnit_addTest(pSuite, MessageTest, testMoveAssignment);
	CppUnit_addTest(pSuite, MessageTest, testSwap);
	CppUnit_addTest(pSuite, MessageTest, testSettersGetters);
	CppUnit_addTest(pSuite, MessageTest, testPriority);
	CppUnit_addTest(pSuite, MessageTest, testTime);
	CppUnit_addTest(pSuite, MessageTest, testThreadInfo);
	CppUnit_addTest(pSuite, MessageTest, testProcessInfo);
	CppUnit_addTest(pSuite, MessageTest, testSourceFileInfo);
	CppUnit_addTest(pSuite, MessageTest, testParameters);
	CppUnit_addTest(pSuite, MessageTest, testParameterAccess);
	CppUnit_addTest(pSuite, MessageTest, testParameterOperator);
	CppUnit_addTest(pSuite, MessageTest, testParameterGetWithDefault);
	CppUnit_addTest(pSuite, MessageTest, testParameterAll);
	CppUnit_addTest(pSuite, MessageTest, testEmptyParameters);
	CppUnit_addTest(pSuite, MessageTest, testMultipleParameters);

	return pSuite;
}