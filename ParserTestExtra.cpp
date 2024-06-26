#include <unistd.h>
#include <iostream>
#include "gtest/gtest.h"
#include "Parser.h"


TEST(ParserTest, TestFileWithExtraBlanklines)
{
    AParser p("testdata/testdata_withextralines.txt");
    ASSERT_EQ( (int) AParser::kErrorSuccess, p.Parse());
}
