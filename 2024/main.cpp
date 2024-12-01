#include "day01.h"

void day01();

int main() {
    cout << "Advent of Code 2024" << endl;
    day01();
    cout << endl;
    return 0;
}

void day01() {
    cout << endl << "Day 01 - **" << endl;
    vector<int> list1, list2;
    readInputLists("input/day01.input.0.txt", list1, list2);
    cout << "input/day01.input.0.txt" << endl;
    cout << "      distance = " << calcListDistance(list1, list2) << endl;
    cout << "    similarity = " << calcListSimilarity(list1,list2) << endl;
    list1.clear();
    list2.clear();
    readInputLists("input/day01.input.1.txt", list1, list2);
    cout << "input/day01.input.1.txt" << endl;
    cout << "      distance = " << calcListDistance(list1, list2) << endl;
    cout << "    similarity = " << calcListSimilarity(list1,list2) << endl;
}