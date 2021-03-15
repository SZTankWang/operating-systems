#ifndef ALGORITHM_H
#define ALGORITHM_H

void swap(Taxpayer * ptr1, Taxpayer * ptr2);

int doSorting(Taxpayer ** payerList,int sortAttr,int sortFlag,int start,int end, int swapped);

void BubbleSort(Taxpayer ** payerList,int length,int flag,int sortAttr);


void insertionSort(Taxpayer ** payerList, int length, int flag, int sortAttr);

#endif
