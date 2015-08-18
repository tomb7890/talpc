#ifndef h_Parser
#define h_Parser

#include <fstream>
#include <string>
#include <vector>

using namespace std;

class AParser
{
private:

   class APair
   {
   public:
      string fKey;
      string fValue;
   };

   class ASection: public vector<APair>
   {
   public:
      string name;
   };

   class ASectionVector: public vector<ASection>
   {
   public:
      void Store(ASection* section);
   };

public:

   AParser(string s);
   ~AParser();

   const int Parse();

   const bool HasSection(const string& ) const;
   const int NumSections() const { return fSections.size(); }
   const string SectionName(const string& ) const ;
   const string GetKey(const string& ) const;
   const string GetValue(const string& ) const;

   const string GetString(const string& section, const string& key ) const;
   const int GetInt(const string& section, const string& key ) const;
   const float GetFloat(const string& section, const string& key ) const;

   void SetString(const string& section, const string& key, const string& newvalue );
   void SetInt(const string& section, const string& key, const int value );
   void SetFloat(const string& section, const string& key, const float value );

private:

   const bool GetLine(ifstream&, string& ) const;
   void Save();
   void Erase();

public:
   enum { kErrorSuccess = 0,
          kErrorCantOpenFile,
          kErrorDuplicateSection,
          kErrorDuplicateKey,
          kErrorDoesntStartWithSection };
private:
   ASectionVector fSections;
   string fFileName;
   string kTerminator;

   const static string kDelimiter;
   const static string kOpenSection;
   const static string kCloseSection;
   const static string kSpace;
   const static string kEmpty;

};

#endif // h_Parser
