#ifndef h_FixedPoint
#define h_FixedPoint

using namespace std;
#include <string>

class FixedPoint {
public:
  FixedPoint(float);
  FixedPoint(int);
  ~FixedPoint();

  int GetValue();
  float GetFloat();
  string GetString();

private:
  int fData;
};

#endif // h_FixedPoint
