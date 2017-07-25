#ifndef HOMEWORK1_H
#define HOMEWORK1_H

#include "MedianSelection.h"

class Homework1 : public MedianSelection {
public:
  ~Homework1();
protected:
  int doSelectMedian(void);
  
  int select(int *list, int left, int right, int index);
  int partition(int *list, int left, int right, int pivotindex);
  int pivot(int *list, int left, int right);
  int insertionSort(int *list, int left, int right);
  int selection(int *list, int len, int index);
  int quickSort(int *list, int left, int right);
  int ceil(float num);
  int floor(float num);
  
  
};

#endif
