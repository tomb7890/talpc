#include <cppunit/extensions/HelperMacros.h>
#include <iostream>

#include "FixedPoint.h"

class FixedPointTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( FixedPointTest );
   CPPUNIT_TEST ( DoTesting );
   CPPUNIT_TEST_SUITE_END();

   void DoTesting();
};

CPPUNIT_TEST_SUITE_REGISTRATION( FixedPointTest );

void FixedPointTest::DoTesting()
{
   float value = 1.0;
   FixedPoint fp(value);
   CPPUNIT_ASSERT_EQUAL( (int)  0x00008000, fp.GetValue() );
   CPPUNIT_ASSERT_EQUAL( (float) 1.0, fp.GetFloat() );

   value = -1.0;
   FixedPoint fp2(value);
   CPPUNIT_ASSERT_EQUAL( (int)  0x80008000, fp2.GetValue() );
   CPPUNIT_ASSERT_EQUAL( (float) -1.0, fp2.GetFloat() );

   value = 2.0;
   FixedPoint fp3(value );
   CPPUNIT_ASSERT_EQUAL( (int ) 0x00010000, fp3.GetValue() );
   CPPUNIT_ASSERT_EQUAL( (float) 2.0, fp3.GetFloat() );

   value = -2.5;
   FixedPoint fp4(value);
   CPPUNIT_ASSERT_EQUAL( (int)  0x80014000, fp4.GetValue() );
   CPPUNIT_ASSERT_EQUAL( (float) -2.5, fp4.GetFloat() );

   value =  -2.25;
   FixedPoint fp5(value);
   CPPUNIT_ASSERT_EQUAL( (int)  0x80012000, fp5.GetValue() );
   CPPUNIT_ASSERT_EQUAL( (float) -2.25, fp5.GetFloat() );

   value = -2.00002;
   FixedPoint fp6( value );
   CPPUNIT_ASSERT_EQUAL( (float) -2.000, fp6.GetFloat() );
}
