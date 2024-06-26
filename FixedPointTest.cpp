#include <iostream>
#include "gtest/gtest.h"
#include "FixedPoint.h"

TEST(FixedPointTest, Addition) { 

   float value = 1.0;
   FixedPoint fp(value);
   ASSERT_EQ( (int)  0x00008000, fp.GetValue() );
   ASSERT_EQ( (float) 1.0, fp.GetFloat() );

   value = -1.0;
   FixedPoint fp2(value);
   ASSERT_EQ( (int)  0x80008000, fp2.GetValue() );
   ASSERT_EQ( (float) -1.0, fp2.GetFloat() );

   value = 2.0;
   FixedPoint fp3(value );
   ASSERT_EQ( (int ) 0x00010000, fp3.GetValue() );
   ASSERT_EQ( (float) 2.0, fp3.GetFloat() );

   value = -2.5;
   FixedPoint fp4(value);
   ASSERT_EQ( (int)  0x80014000, fp4.GetValue() );
   ASSERT_EQ( (float) -2.5, fp4.GetFloat() );

   value =  -2.25;
   FixedPoint fp5(value);
   ASSERT_EQ( (int)  0x80012000, fp5.GetValue() );
   ASSERT_EQ( (float) -2.25, fp5.GetFloat() );

   value = -2.00002;
   FixedPoint fp6( value );
   ASSERT_EQ( (float) -2.000, fp6.GetFloat() );
}
