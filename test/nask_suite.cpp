#include "nask_utility.hpp"
#include <CppUTest/TestHarness.h>

TEST_GROUP(nask_suite)
{
};


TEST(nask_suite, testNaskUtilityContains)
{
     const std::string line = "		MOV		AX,1*8			;  読み書き可能セグメント32bit";
     const bool a = nask_utility::contains(line, "*");
     CHECK(a);
}

TEST(nask_suite, testNaskUtilityIsContainsMathOp)
{
     const std::string line1 = "		MOV		AX,1*8	;  読み書き可能セグメント32bit";
     const bool a = nask_utility::is_contains_math_op(line1);
     CHECK(a);

     const std::string line2 = "		MOV		ECX,512*1024/4";
     const bool b = nask_utility::is_contains_math_op(line2);
     CHECK(b);

     const std::string line3 = "		DW		83-1";
     const bool c = nask_utility::is_contains_math_op(line3);
     CHECK(c);

     const std::string line4 = "		DW		83+1";
     const bool d = nask_utility::is_contains_math_op(line4);
     CHECK(d);
}