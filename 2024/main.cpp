#include "day01.h"
#include "day02.h"

void day01();
void day02();

int main() {
    cout << "Advent of Code 2024" << endl;
    day01();    // Day 1: Historian Hysteria
    day02();    // Day 2: Red-Nosed Reports
    cout << endl;
    return 0;
}

// Day 1: Historian Hysteria
void day01() {
    cout << endl << "Day 01 - **" << endl;
    vector<int> list1, list2;
    readInputLists("input/day01.input.0.txt", list1, list2);
    cout << "input/day01.input.0.txt" << endl;
    cout << "       distance = " << calcListDistance(list1, list2) << endl;
    cout << "    similarity1 = " << calcListSimilarity1(list1,list2) << endl;
    cout << "    similarity2 = " << calcListSimilarity2(list1,list2) << endl;
    list1.clear();
    list2.clear();
    readInputLists("input/day01.input.1.txt", list1, list2);
    cout << "input/day01.input.1.txt" << endl;
    cout << "       distance = " << calcListDistance(list1, list2) << endl;
    cout << "    similarity1 = " << calcListSimilarity1(list1,list2) << endl;
    cout << "    similarity2 = " << calcListSimilarity2(list1,list2) << endl;
}

// Day 2: Red-Nosed Reports
void day02()
{
    cout << endl << "Day 02 - " << endl;
    cout << "input/day02.input.0.txt" << endl;
    cout << "       safe = " << numSafeReports("input/day02.input.0.txt") << endl;
    cout << "input/day02.input.1.txt" << endl;
    cout << "       safe = " << numSafeReports("input/day02.input.1.txt") << endl;
}