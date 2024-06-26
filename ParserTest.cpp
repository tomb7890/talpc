
#include <unistd.h>
#include <iostream>
#include "gtest/gtest.h"
#include "Parser.h"

void CopyFile( const string& src, const string& dst );

TEST(ParserTest, KeyValuePairs) { 

   AParser p("testdata/testdata.txt");

   const string key = "key";
   const string value = "value";

   const string sample1 = "key:value";
   ASSERT_EQ(key, p.GetKey( sample1 ));
   ASSERT_EQ(value, p.GetValue( sample1 ));

   const string sample2 = "key :value";
   ASSERT_EQ(key, p.GetKey( sample2 ));
   ASSERT_EQ(value, p.GetValue(sample2 ));

   const string sample3 = "key :  value";
   ASSERT_EQ(key, p.GetKey( sample3 ));
   ASSERT_EQ(value, p.GetValue( sample3 ));

   const string badkey = "";
   ASSERT_EQ(badkey, p.GetKey(" key :  value"));
   ASSERT_EQ(badkey, p.GetKey("   key :  value"));

   const string spaceykey = "spacey key";
   ASSERT_EQ(spaceykey, p.GetKey("spacey key :  value"));


   const string k = "k";
   const string v = "v";
   const string minimal = "k:v";
   ASSERT_EQ(k, p.GetKey("k : v  "));
   ASSERT_EQ(k, p.GetKey("k: v  "));
   ASSERT_EQ(k, p.GetKey("k :v "));

}



TEST(ParserTest, ParseSectionNames) { 
   AParser p("testdata/testdata.txt");
   const string expected = "Section demonstration";

   ASSERT_EQ(expected, p.SectionName("[Section demonstration]"));
   ASSERT_EQ(expected, p.SectionName("[   Section demonstration]"));
   ASSERT_EQ(expected, p.SectionName("[ Section demonstration     ]"));

   const string boundaryCond1 = "";
   ASSERT_EQ(boundaryCond1, p.SectionName("[]"));
   ASSERT_EQ(boundaryCond1, p.SectionName(" []"));
   ASSERT_EQ(boundaryCond1, p.SectionName("[] "));

   const string boundaryCond2 = "a";
   ASSERT_EQ(boundaryCond2, p.SectionName("[a]"));
   ASSERT_EQ(boundaryCond2, p.SectionName("[a] "));
   ASSERT_EQ(boundaryCond2, p.SectionName("[ a] "));
   ASSERT_EQ(boundaryCond2, p.SectionName("[ a ] "));
   ASSERT_EQ(boundaryCond2, p.SectionName("[ a  ]"));

   const string malformed = "";
   ASSERT_EQ(malformed, p.SectionName("]["));
}

TEST(ParserTest, DuplicateKeys)
{
   AParser p("testdata/dupekeys.txt");
   ASSERT_EQ((int) AParser::kErrorDuplicateKey, p.Parse() );
   ASSERT_EQ( 0, p.NumSections() );
}

TEST(ParserTest, DuplicateSections)
{
   AParser p("testdata/dupesecs.txt");
   ASSERT_EQ((int) AParser::kErrorDuplicateSection, p.Parse() );
   ASSERT_EQ( 0, p.NumSections() );

   // even though invalid, can we cause any trouble
   p.SetInt("header", "accessed", 0);
   ASSERT_EQ( 0, p.GetInt("header", "accessed"));
}


TEST(ParserTest, OpenABogusFile)
{
   AParser p("bogus.txt");
   ASSERT_EQ( (int) AParser::kErrorCantOpenFile, p.Parse());
}

TEST(ParserTest, OpenValidFile)
{
   AParser p("testdata/testdata.txt");
   ASSERT_EQ( (int) AParser::kErrorSuccess, p.Parse());
}


TEST(ParserTest, FirstNonBlankStartsSection)
{
   AParser p("testdata/nosec.txt");
   ASSERT_EQ( (int) AParser::kErrorDoesntStartWithSection, p.Parse());

   // even though invalid, can we cause any trouble
   p.SetInt("header", "accessed", 0);
   ASSERT_EQ( 0, p.GetInt("header", "accessed"));


   AParser p2("testdata/simplesec.txt");
   ASSERT_EQ( (int) AParser::kErrorSuccess, p2.Parse());
}

TEST(ParserTest, GetString)
{
   AParser p("testdata/testdata.txt");

   p.Parse();
   

   ASSERT_EQ( 3, p.NumSections() );

   string expected = "all out of budget.";
   ASSERT_EQ( expected, p.GetString("trailer", "budget"));

   expected = "I meant 'moderately,' not 'tediously,' above.";
   ASSERT_EQ( expected, p.GetString("meta data", "correction text"));

   expected = "This is a tediously long description of the Art & Logic" ;
   expected += " programming test that you are taking. Tedious isn't the right word, but" ;
   expected += " it's the first word that comes to mind.";

   ASSERT_EQ( expected, p.GetString("meta data", "description"));
}

TEST(ParserTest, GetInt)
{
   AParser p("testdata/testdata.txt");
   p.Parse();

   int expected = 205;
   ASSERT_EQ( expected, p.GetInt("header", "accessed"));
}

TEST(ParserTest, GetFloat)
{
   AParser p("testdata/testdata.txt");
   p.Parse();

   float expected = 4.5;
   ASSERT_EQ( expected, p.GetFloat("header", "budget"));
}

TEST(ParserTest, SetString)
{
   // copy to a temp file for testing
   CopyFile("testdata/testdata.txt","/tmp/temp1.txt");

   AParser p("/tmp/temp1.txt");
   p.Parse();

   ASSERT_EQ( 3, p.NumSections() );

   string expected = "all out of budget.";
   ASSERT_EQ( expected, p.GetString("trailer", "budget"));
   p.SetString("trailer", "budget", "replacement");

   expected = "replacement";
   ASSERT_EQ( expected, p.GetString("trailer", "budget"));

   // make a new parser to verify disk has the new stuff.
   AParser p2("/tmp/temp1.txt");
   p2.Parse();
   ASSERT_EQ( expected, p2.GetString("trailer", "budget"));

   unlink("temp1.txt");
}

TEST(ParserTest, SetFloat)
{
   // copy to a temp file for testing
   CopyFile("testdata/testdata.txt","/tmp/temp1.txt");

   AParser p("/tmp/temp1.txt");

   p.Parse();

   float f = 4.5;
   ASSERT_EQ( f, p.GetFloat("header", "budget"));

   f = 12342.7;
   p.SetFloat("header", "budget", f);
   ASSERT_EQ( f, p.GetFloat("header", "budget"));

   // make a new parser to verify disk has the new stuff.
   AParser p2("/tmp/temp1.txt");
   p2.Parse();
   ASSERT_EQ( f, p2.GetFloat("header", "budget"));

   unlink ("/tmp/temp1.txt");
}

TEST(ParserTest, SetInt)
{
   // copy to a temp file for testing
   CopyFile("testdata/testdata.txt","temp2.txt" );

   AParser p("temp2.txt");

   p.Parse();

   int d = 205;
   ASSERT_EQ( d, p.GetInt("header", "accessed"));

   d = -23421;
   p.SetInt("header", "accessed", d);
   ASSERT_EQ( d, p.GetInt("header", "accessed"));

   // make a new parser to verify disk has the new stuff.
   AParser p2("temp2.txt");
   p2.Parse();
   ASSERT_EQ( d, p2.GetInt("header", "accessed"));

   unlink("temp2.txt");
}

TEST(ParserTest, SectionStartsWithoutHeader)
{
   AParser p("testdata/sansheader.txt");
   ASSERT_EQ( (int) AParser::kErrorDoesntStartWithSection, p.Parse());
   ASSERT_EQ(0, p.NumSections());

   // even though invalid, can we cause any trouble
   p.SetInt("header", "accessed", 0);
   ASSERT_EQ( 0, p.GetInt("header", "accessed"));
}

void CopyFile( const string& src, const string& dst )
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
