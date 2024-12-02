#pragma once
#include "common.h"

/*

*/
int calcListDistance(vector<int>& list1, vector<int>& list2);
int calcListSimilarity1(vector<int>& list1, vector<int>& list2);
int calcListSimilarity2(vector<int>& list1, vector<int>& list2);

void readInputLists(const string& fname, vector<int>& list1, vector<int>& list2);