#include "Parser.h"

#include <iostream>
#include <sstream>

const string AParser::kDelimiter = ":";
const string AParser::kCloseSection = "]";
const string AParser::kOpenSection = "[";
const string AParser::kSpace = " ";
const string AParser::kEmpty = "";

AParser::AParser(string f)
{
   fFileName = f;
   //   initialize terminator
   ostringstream ostr;
   ostr << (char) 0xD << (char) 0xA;
   kTerminator = ostr.str();
}

AParser::~AParser()
{}

const string AParser::GetKey(const string& line) const
{
   size_t pos = line.find(kDelimiter);
   if ( string::npos == pos )
   {
      return kEmpty;
   }

   if ( ' ' == line[0] )
   {
      return kEmpty;
   }

   while (' ' == line[pos-1])
   {
      pos--;
   }

   return line.substr(0, pos);
}

const string AParser::GetValue(const string& line) const
{
   size_t pos = line.find(kDelimiter);
   if ( string::npos == pos )
   {
      return kEmpty;
   }

   pos++;

   while (' ' == line[pos])
   {
      pos++;
   }

   int len = line.size() - pos;
   return line.substr(pos, len);
}

const string AParser::SectionName(const string& buf) const
{
   string retvalue = kEmpty;
   if ( 0 != buf.find( kOpenSection ))
   {
      return retvalue;
   }

   size_t start = buf.find_first_not_of(kSpace,1);

   if ( string::npos == start  )
   {
      return retvalue;
   }

   size_t end = buf.find(kCloseSection, start);

   if ( string::npos == end )
   {
      return retvalue;
   }

   while (' ' == buf[end-1])
   {
      end--;
   }

   if ( end > start )
   {
      int len = end - start;
      retvalue = buf.substr(start, len);
   }

   return retvalue;
}

const string AParser::GetString(const string& sec, const string& key ) const
{
    for (auto& s: fSections)
    {
        if ( s.name == sec ) 
        {
            for (auto& p: s) 
            {

                if ( key == p.fKey )
                {
                    return p.fValue;
                }
            }
        }
    }
    return kEmpty;
}

const float AParser::GetFloat(const string& sec, const string& key ) const
{
   const string stringval = this->GetString(sec, key);

   std::istringstream iss(stringval);
   float f(0);

   if(!(iss >> f))
   {
      return 0;
   }
   return f;
}

const int AParser::GetInt( const string& sec, const string& key ) const
{
   const string stringval = this->GetString(sec, key);

   std::istringstream iss(stringval);
   int i(0);

   if(!(iss >> i))
   {
      return 0;
   }
   return i;
}

void AParser::SetString( const string& sec, const string& key, const string& value )
{
    for (auto& s: fSections)
    {
        if ( s.name == sec ) 
        {
            for (auto& p: s) 
            {
                if ( key == p.fKey )
                {
                    p.fValue = value;
                    this->Save();
                }
            }
        }
    }
}


void AParser::SetFloat( const string& sec, const string& key, const float value )
{
   ostringstream ostr ;
   ostr << value ;
   string floatstring  ( ostr.str () ) ;
   this->SetString(sec, key, floatstring );
}

void AParser::SetInt( const string& sec, const string& key, const int value )
{
   ostringstream ostr ;
   ostr << value ;
   string intstring  ( ostr.str () ) ;
   this->SetString(sec, key, intstring );
}

void AParser::Save()
{
    if ( !fSections.empty())
    {
        ofstream ofs(fFileName.c_str(), ios::out);

        for (auto& section: fSections)
        {

            ofs << kOpenSection << section.name << kCloseSection << kTerminator;
            {
                for (auto& p: section ) 
                {
                    ofs << p.fKey << kDelimiter << p.fValue << kTerminator;
                }
            }
            ofs << kTerminator;
        }

        ofs.flush();
        ofs.close();
    }
}

void AParser::Erase()
{
   this->fSections.erase(fSections.begin(), fSections.end());
}

void AParser::GetLine(ifstream& ifs, string& buffer) const
{
    string temp;
    std::getline(ifs, temp);

    if ( temp.size() > 0 ) { 
     
        if ( '\r' == temp[temp.size() - 1] )
        {
            buffer = temp.substr(0, temp.size() - 1 );
        }
        else
        {
            buffer = temp;
        }
    }
}

const int AParser::Parse()
{
   ifstream ifs(fFileName.c_str(), ios::in);

   if (! ifs )
   {
      return kErrorCantOpenFile;
   }

   ASection* pSec = NULL;

   string buf;
   while ( !( ifs.eof()))
   {
     this->GetLine(ifs, buf);
     
      if ( kEmpty != this->SectionName( buf ))
      {
         fSections.Store( pSec );

         for (auto& section: fSections)
         {
           if (this->SectionName(buf) == section.name )
            {
               this->Erase();
               return kErrorDuplicateSection;
            }
         }

         pSec = new ASection();
         pSec->name = this->SectionName( buf );

      }
      else if ( kEmpty != this->GetKey( buf ))
      {
         if ( NULL == pSec && fSections.empty()  )
         {
            return kErrorDoesntStartWithSection;
         }

         ASection& section = *pSec;

         for (auto& pair: section) 
         {
            if ( this->GetKey(buf) == pair.fKey )
            {
               this->Erase();
               return kErrorDuplicateKey;
            }
         }

         APair pair;
         pair.fKey =  this->GetKey( buf );
         pair.fValue =  this->GetValue( buf );

         if ( NULL != pSec )
         {
            pSec->push_back( pair );
         }
      }
      else
      { // continuation

         // if its not a section header and its not a blank line, its an error.
         if ( kEmpty != buf  && fSections.empty() )
         {
            return kErrorDoesntStartWithSection;
         }

         if ( ' ' == buf[0] )
         {
            if ( NULL != pSec )
            {
               ASection& section = *pSec;
               APair& pair = section[ section.size() - 1 ];
               pair.fValue = pair.fValue + buf;
            }
         }
      }
   }

   fSections.Store( pSec );
   return kErrorSuccess;
}

void
AParser::ASectionVector::Store( ASection* pSec )
{
   if ( NULL != pSec )
   {
      this->push_back( *pSec );

      delete pSec;
      pSec = NULL;
   }
}
