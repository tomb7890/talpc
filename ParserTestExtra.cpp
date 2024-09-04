#include "Parser.h"
#include "gtest/gtest.h"
#include <iostream>
#include <unistd.h>

TEST(ParserTest, TestFileWithExtraBlanklines) {
  AParser p("testdata/testdata_withextralines.txt");
  ASSERT_EQ((int)AParser::kErrorSuccess, p.Parse());
}
