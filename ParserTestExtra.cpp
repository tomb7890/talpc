#include <cppunit/extensions/HelperMacros.h>

#include "Parser.h"
#include <unistd.h>

class ParserTestExtra : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( ParserTestExtra );
   CPPUNIT_TEST ( TestFileWithExtraBlanklines );
   CPPUNIT_TEST_SUITE_END();

    void TestFileWithExtraBlanklines ();
};

CPPUNIT_TEST_SUITE_REGISTRATION( ParserTestExtra );

void ParserTestExtra::TestFileWithExtraBlanklines()
{
    AParser p("testdata/testdata_withextralines.txt");
    CPPUNIT_ASSERT_EQUAL( (int) AParser::kErrorSuccess, p.Parse());
}
