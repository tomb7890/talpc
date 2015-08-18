#include "FixedPoint.h"

#include <iostream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>

using namespace std;

FixedPoint::FixedPoint(int val)
{
   // Create Fixed Point object from packed 32-bit value
   fData = val;
}


FixedPoint::FixedPoint(float f)
{
   // Create Fixed Point object from floating point value
   fData = 0;
   int dec = (int) f;
   if ( f < 0 )
   {
      fData = 0x80000000;
      dec = -dec;
   }

   dec = dec << 15;
   fData = fData | ( dec );

   float fraction = fabs(f) - floor(fabs(f));
   int scaled_up = (int) ( fraction * 32768 );

   fData = fData |  scaled_up;
}

FixedPoint::~FixedPoint()
{}

int FixedPoint::GetValue ()
{
   return fData;
}

float FixedPoint::GetFloat ()
{
   // Convert value to closest floating point equivalent
   float retvalue = 0;

   unsigned decimal = ( fData & 0x7FFF8000 );
   retvalue = retvalue + ( decimal >> 15 );

   unsigned fractional = ( fData & 0x00007FFF );
   retvalue += ( fractional / 32768.0 ) ;

   if ( 0x80000000 & fData )
      retvalue = -retvalue;

   return retvalue;
}


string FixedPoint::GetString ()
{
   // Idiomatic conversion to string (as
   // <<optional sign>><<integer part>>.<<fractional part>>)
   ostringstream stream;
   stream << this->GetFloat() << endl ;
   return stream.str();
}
