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

void AParser::GetLine(ifstream& ifs, string& line) const
{
    string buffer;
    std::getline(ifs, buffer);



    if ( '\r' == buffer[buffer.size() - 1] )
    {
        line = buffer.substr(0, buffer.size() - 1 );
    }
    else
    {
        line = buffer;
    }

}

const int AParser::Parse() {
    ifstream ifs;
    string line;
    int linenum = 0;
    string sectionname;

    ifs.open(fFileName.c_str(), ios::in);
    if (ifs.fail())        return kErrorCantOpenFile;

    try {
        while (! ifs.eof())
        {
            this->GetLine(ifs, line);
            sectionname = SectionName(line);
            if (kEmpty == sectionname ) {
                handleNonSectionName(linenum, line) ;
            }
            else {
                handleSectionName(sectionname);
            }
            linenum++;
        }
        storePendingSection();
    }

    catch (kErrors e) {
        return e;
    }
    return kErrorSuccess;
}

const bool AParser::alreadySeen(string& newKey) const {
    for (auto p: tempPairs ) {
        if ( p.fKey == newKey ) {
            return true;
        }
    }
    return false;
}
const int AParser::handleSectionName (string& sectionname ) {
    if ( sectionname == prevSectionName ) {
        throw AParser::kErrorDuplicateSection;

    }

    for (auto& s: AParser::fSections)
    {
        if ( s.name == sectionname ) {
            throw kErrorDuplicateSection;
        }
    }

    storePendingSection();
    prevSectionName = sectionname;

    return kErrorSuccess;
}


void AParser::clearAllSection()
{
    fSections.clear();
}

const int AParser::handleNonSectionName(int linenum, string& line){

    if ( 0 == linenum ) {
        throw kErrorDoesntStartWithSection;
    }

    string newKey = GetKey(line);
    string newVal = GetValue(line);

    if ( newKey.size() > 0 ) {
        if ( alreadySeen(newKey))
        {
            clearAllSection();
            throw kErrorDuplicateKey;
        }

        if ( newVal.size() > 0 ) {
            APair newpair;
            newpair.fKey = newKey;
            newpair.fValue = newVal;
            tempPairs.push_back(newpair);
        }
    }
    else {

        handlePossibleContinuationLine(line);
    }
    return kErrorSuccess;
}

void AParser::handlePossibleContinuationLine(string& buf)
{
    if ( buf.size() > 1 )
        if (buf[0] == ' ' )
            if ( tempPairs.size() > 0 )
            {
                APair& p = tempPairs.back();
                p.fValue = p.fValue + buf;
            }

}




void
AParser::storePendingSection ()
{
    if ( prevSectionName.size() > 0 ) {
        ASection section;
        section.name = prevSectionName;
        for (auto p: tempPairs ) {
            section.push_back(p);
        }
        fSections.push_back(section);
        tempPairs.clear();
    }
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
