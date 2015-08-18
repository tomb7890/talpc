#include <cppunit/extensions/HelperMacros.h>

#include "Parser.h"
#include <unistd.h>

class ParserTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( ParserTest );

   CPPUNIT_TEST ( KeyValuePairs );
   CPPUNIT_TEST ( ParseSectionNames );

   CPPUNIT_TEST ( FirstNonBlankStartsSection );
   CPPUNIT_TEST ( SectionStartsWithoutHeader );

   CPPUNIT_TEST ( DuplicateKeys );
   CPPUNIT_TEST ( DuplicateSections );

   CPPUNIT_TEST ( OpenABogusFile );
   CPPUNIT_TEST ( OpenValidFile );

   CPPUNIT_TEST ( GetFloat );
   CPPUNIT_TEST ( GetInt );
   CPPUNIT_TEST ( GetString );

   CPPUNIT_TEST ( SetInt );
   CPPUNIT_TEST ( SetFloat );
   CPPUNIT_TEST ( SetString );

   CPPUNIT_TEST_SUITE_END();

   void KeyValuePairs ();
   void ParseSectionNames ();

   void FirstNonBlankStartsSection ();
   void SectionStartsWithoutHeader ();

   void DuplicateKeys ();
   void DuplicateSections ();

   void OpenABogusFile ();
   void OpenValidFile ();

   void GetString ();
   void GetFloat ();
   void GetInt ();

   void SetString ();
   void SetFloat ();
   void SetInt ();

   void CopyFile(const string& src, const string& dst );
};

CPPUNIT_TEST_SUITE_REGISTRATION( ParserTest );

void ParserTest::KeyValuePairs()
{
   AParser p("testdata/testdata.txt");

   const string key = "key";
   const string value = "value";

   const string sample1 = "key:value";
   CPPUNIT_ASSERT_EQUAL(key, p.GetKey( sample1 ));
   CPPUNIT_ASSERT_EQUAL(value, p.GetValue( sample1 ));

   const string sample2 = "key :value";
   CPPUNIT_ASSERT_EQUAL(key, p.GetKey( sample2 ));
   CPPUNIT_ASSERT_EQUAL(value, p.GetValue(sample2 ));

   const string sample3 = "key :  value";
   CPPUNIT_ASSERT_EQUAL(key, p.GetKey( sample3 ));
   CPPUNIT_ASSERT_EQUAL(value, p.GetValue( sample3 ));

   const string badkey = "";
   CPPUNIT_ASSERT_EQUAL(badkey, p.GetKey(" key :  value"));
   CPPUNIT_ASSERT_EQUAL(badkey, p.GetKey("   key :  value"));

   const string spaceykey = "spacey key";
   CPPUNIT_ASSERT_EQUAL(spaceykey, p.GetKey("spacey key :  value"));


   const string k = "k";
   const string v = "v";
   const string minimal = "k:v";
   CPPUNIT_ASSERT_EQUAL(k, p.GetKey("k : v  "));
   CPPUNIT_ASSERT_EQUAL(k, p.GetKey("k: v  "));
   CPPUNIT_ASSERT_EQUAL(k, p.GetKey("k :v "));

}

void ParserTest::ParseSectionNames()
{
   AParser p("testdata/testdata.txt");
   const string expected = "Section demonstration";

   CPPUNIT_ASSERT_EQUAL(expected, p.SectionName("[Section demonstration]"));
   CPPUNIT_ASSERT_EQUAL(expected, p.SectionName("[   Section demonstration]"));
   CPPUNIT_ASSERT_EQUAL(expected, p.SectionName("[ Section demonstration     ]"));

   const string boundaryCond1 = "";
   CPPUNIT_ASSERT_EQUAL(boundaryCond1, p.SectionName("[]"));
   CPPUNIT_ASSERT_EQUAL(boundaryCond1, p.SectionName(" []"));
   CPPUNIT_ASSERT_EQUAL(boundaryCond1, p.SectionName("[] "));

   const string boundaryCond2 = "a";
   CPPUNIT_ASSERT_EQUAL(boundaryCond2, p.SectionName("[a]"));
   CPPUNIT_ASSERT_EQUAL(boundaryCond2, p.SectionName("[a] "));
   CPPUNIT_ASSERT_EQUAL(boundaryCond2, p.SectionName("[ a] "));
   CPPUNIT_ASSERT_EQUAL(boundaryCond2, p.SectionName("[ a ] "));
   CPPUNIT_ASSERT_EQUAL(boundaryCond2, p.SectionName("[ a  ]"));

   const string malformed = "";
   CPPUNIT_ASSERT_EQUAL(malformed, p.SectionName("]["));
}

void ParserTest::DuplicateKeys()
{
   AParser p("testdata/dupekeys.txt");
   CPPUNIT_ASSERT_EQUAL((int) AParser::kErrorDuplicateKey, p.Parse() );
   CPPUNIT_ASSERT_EQUAL( 0, p.NumSections() );
}

void ParserTest::DuplicateSections()
{
   AParser p("testdata/dupesecs.txt");
   CPPUNIT_ASSERT_EQUAL((int) AParser::kErrorDuplicateSection, p.Parse() );
   CPPUNIT_ASSERT_EQUAL( 0, p.NumSections() );

   // even though invalid, can we cause any trouble
   p.SetInt("header", "accessed", 0);
   CPPUNIT_ASSERT_EQUAL( 0, p.GetInt("header", "accessed"));
}


void ParserTest::OpenABogusFile()
{
   AParser p("bogus.txt");
   CPPUNIT_ASSERT_EQUAL( (int) AParser::kErrorCantOpenFile, p.Parse());
}

void ParserTest::OpenValidFile()
{
   AParser p("testdata/testdata.txt");
   CPPUNIT_ASSERT_EQUAL( (int) AParser::kErrorSuccess, p.Parse());
}


void ParserTest::FirstNonBlankStartsSection()
{
   AParser p("testdata/nosec.txt");
   CPPUNIT_ASSERT_EQUAL( (int) AParser::kErrorDoesntStartWithSection, p.Parse());

   // even though invalid, can we cause any trouble
   p.SetInt("header", "accessed", 0);
   CPPUNIT_ASSERT_EQUAL( 0, p.GetInt("header", "accessed"));


   AParser p2("testdata/simplesec.txt");
   CPPUNIT_ASSERT_EQUAL( (int) AParser::kErrorSuccess, p2.Parse());
}

void ParserTest::GetString()
{
   AParser p("testdata/testdata.txt");
   p.Parse();

   CPPUNIT_ASSERT_EQUAL( 3, p.NumSections() );

   string expected = "all out of budget.";
   CPPUNIT_ASSERT_EQUAL( expected, p.GetString("trailer", "budget"));

   expected = "I meant 'moderately,' not 'tediously,' above.";
   CPPUNIT_ASSERT_EQUAL( expected, p.GetString("meta data", "correction text"));

   expected = "This is a tediously long description of the Art & Logic" ;
   expected += " programming test that you are taking. Tedious isn't the right word, but" ;
   expected += " it's the first word that comes to mind.";

   CPPUNIT_ASSERT_EQUAL( expected, p.GetString("meta data", "description"));
}

void ParserTest::GetInt()
{
   AParser p("testdata/testdata.txt");
   p.Parse();

   int expected = 205;
   CPPUNIT_ASSERT_EQUAL( expected, p.GetInt("header", "accessed"));
}

void ParserTest::GetFloat()
{
   AParser p("testdata/testdata.txt");
   p.Parse();

   float expected = 4.5;
   CPPUNIT_ASSERT_EQUAL( expected, p.GetFloat("header", "budget"));
}

void ParserTest::SetString()
{
   // copy to a temp file for testing
   CopyFile("testdata/testdata.txt","/tmp/temp1.txt");

   AParser p("/tmp/temp1.txt");
   p.Parse();

   CPPUNIT_ASSERT_EQUAL( 3, p.NumSections() );

   string expected = "all out of budget.";
   CPPUNIT_ASSERT_EQUAL( expected, p.GetString("trailer", "budget"));
   p.SetString("trailer", "budget", "replacement");

   expected = "replacement";
   CPPUNIT_ASSERT_EQUAL( expected, p.GetString("trailer", "budget"));

   // make a new parser to verify disk has the new stuff.
   AParser p2("/tmp/temp1.txt");
   p2.Parse();
   CPPUNIT_ASSERT_EQUAL( expected, p2.GetString("trailer", "budget"));

   unlink("temp1.txt");
}

void ParserTest::SetFloat()
{
   // copy to a temp file for testing
   CopyFile("testdata/testdata.txt","/tmp/temp1.txt");

   AParser p("/tmp/temp1.txt");

   p.Parse();

   float f = 4.5;
   CPPUNIT_ASSERT_EQUAL( f, p.GetFloat("header", "budget"));

   f = 12342.7;
   p.SetFloat("header", "budget", f);
   CPPUNIT_ASSERT_EQUAL( f, p.GetFloat("header", "budget"));

   // make a new parser to verify disk has the new stuff.
   AParser p2("/tmp/temp1.txt");
   p2.Parse();
   CPPUNIT_ASSERT_EQUAL( f, p2.GetFloat("header", "budget"));

   unlink ("/tmp/temp1.txt");
}

void ParserTest::SetInt()
{
   // copy to a temp file for testing
   CopyFile("testdata/testdata.txt","temp2.txt" );

   AParser p("temp2.txt");

   p.Parse();

   int d = 205;
   CPPUNIT_ASSERT_EQUAL( d, p.GetInt("header", "accessed"));

   d = -23421;
   p.SetInt("header", "accessed", d);
   CPPUNIT_ASSERT_EQUAL( d, p.GetInt("header", "accessed"));

   // make a new parser to verify disk has the new stuff.
   AParser p2("temp2.txt");
   p2.Parse();
   CPPUNIT_ASSERT_EQUAL( d, p2.GetInt("header", "accessed"));

   unlink("temp2.txt");
}

void ParserTest::SectionStartsWithoutHeader()
{
   AParser p("testdata/sansheader.txt");
   CPPUNIT_ASSERT_EQUAL( (int) AParser::kErrorDoesntStartWithSection, p.Parse());
   CPPUNIT_ASSERT_EQUAL(0, p.NumSections());

   // even though invalid, can we cause any trouble
   p.SetInt("header", "accessed", 0);
   CPPUNIT_ASSERT_EQUAL( 0, p.GetInt("header", "accessed"));
}

void ParserTest::CopyFile( const string& src, const string& dst )
{
   ofstream ofs(dst.c_str(), ios::out );
   ifstream ifs(src.c_str(), ios::in );

   if ( ofs && ifs)
   {
      char c = '0';
      while ( EOF != ( c  = ifs.get()))
      {
         ofs.put( c );
      }
      ofs.flush();
      ofs.close();
   }
}
