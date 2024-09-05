#ifndef h_Parser
#define h_Parser

#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class AParser {

public:
  // Create a parser object given a file name.
  AParser(string s);

  // Get a string value associated with a given section and key names.
  const string GetString(const string &section, const string &key) const;

  // Get an integer value associated with a given section and key names.
  const int GetInt(const string &section, const string &key) const;

  // Get a floating point value associated with a given section and key names.
  const float GetFloat(const string &section, const string &key) const;

  // Set a string value for a given section and key names, writing the new file
  // to disk.
  void SetString(const string &section, const string &key,
                 const string &newvalue);

  // Set an integer value for a given section and key names, writing the new
  // file to disk.
  void SetInt(const string &section, const string &key, const int value);

  // Set a floating point value for a given section and key names, writing the
  // new file to disk.
  void SetFloat(const string &section, const string &key, const float value);

  void GetLine(ifstream &, string &) const;
  void Erase();

  const bool HasSection(const string &) const;
  const int NumSections() const { return fSections.size(); }
  const string SectionName(const string &) const;
  const string GetKey(const string &) const;
  const string GetValue(const string &) const;

public:
  const int Parse();
  ~AParser();
  class APair {
  public:
    string fKey;
    string fValue;
  };

  class ASection : public map<std::string, std::string> {
  public:
    string name;
  };

  class ASectionVector : public vector<ASection> {
  public:
    void Store(ASection *section);
  };

  ASectionVector &Sections() { return fSections; }

  map<std::string, std::string>  tentativeSectionPairs;

public:
  enum kErrors {
    kErrorSuccess = 0,
    kErrorCantOpenFile,
    kErrorDuplicateSection,
    kErrorDuplicateKey,
    kErrorDoesntStartWithSection
  };

private:
  void Save();

  const int handleNonSectionName(int linenum, string &buf);
  const int handleSectionName(string &);
  void handlePossibleContinuationLine(string &buf);
  void storePendingSection();
  const bool alreadySeen(string &) const;
  void clearAllSection();

  string prevSectionName;

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
