#include "common.h"
#include "day01.h"
#include "day02.h"
#include "day03.h"

void day01();
void day02();
void day03();

int main(int argc, char** argv) {
    cout << "Advent of Code 2024" << endl << endl;

    cout << "Running unit tests..." << endl;
    ::testing::InitGoogleTest(&argc,argv);
    int test_val = RUN_ALL_TESTS();
    if( !test_val )
    {
        cout << "Testing passed. Running main scenarios." << endl;

        day01();    // Day 1: Historian Hysteria
        day02();    // Day 2: Red-Nosed Reports
        day03();    // Day 3: Mull It Over
    }
    else
    {
        cout << "Testing failed, skipping main scenarios." << endl;
    }

    cout << endl;

    return test_val;
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
    cout << endl << "Day 02 - **" << endl;
    cout << "input/day02.input.txt" << endl;
    cout << "    safe          = " << numSafeReports("input/day02.input.txt") << endl;
    cout << "    safe (dampen) = " << numSafeReportsWithTolerance("input/day02.input.txt") << endl;
}

// Day 3: Mull It Over
void day03()
{
    cout << endl << "Day 03 - " << endl;
    cout << "input/day03.txt" << endl;
    cout << "    mull(x,y) total = " << calcMulls("input/day03.txt") << endl;
}