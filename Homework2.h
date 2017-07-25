#ifndef HOMEWORK2_H
#define HOMEWORK2_H
#include <vector>
#include "SelectExtremes.h"

class Homework2 : public SelectExtremes {
public:
  ~Homework2();
  int findIndex(int value, vector<int>vec);
protected:
  int *doSelect(int *ans);
};

#endif
