#include "common.h"

int calcListDistance( vector<int>& list1, vector<int>& list2 ) {
    int distance = 0;
    sort(list1.begin(), list1.end());
    sort(list2.begin(), list2.end());
    for (int i = 0; i < list1.size(); i++) {
        distance += abs(list1[i] - list2[i]);
    }
    return distance;
}

//  time = O(N+K)
// space = O(K)
int calcListSimilarity1(vector<int>& list1, vector<int>& list2){
    int similarity = 0;
    // space = O(K), worst case no duplicates, so map has a single occurance for all unique elemeents
    unordered_map<int,int> occurances;
    // time = O(K)
    for( auto i : list2 )
        occurances[i]++;
    // time = O(N)
    for( auto i : list1 )
    {
        auto res = occurances.find(i);
        if( res != occurances.end() )
            similarity += i * occurances[i];
    }
    return similarity;
}

//  time = 2*O(logN)+O(N+K)
// space = O(1)
int calcListSimilarity2(vector<int>& list1, vector<int>& list2)
{
    int similarity = 0;
    int l2_start = 0;

    sort(list1.begin(),list1.end()); // time = O(logN)
    sort(list2.begin(),list2.end()); // time = O(logK)

    // time = O(N+K)
    for( int i = 0; i < list1.size(); i++ )
    {
        // @note: Not iterating over list2 for each element in list1;
        // starts @ l2_start and breaks when list2 element is greater
        for( int j = l2_start; j < list2.size(); j++ )
        {
            auto l1 = list1[ i ];
            auto l2 = list2[ j ];
            if( l2 < l1 )
            {
                l2_start = j + 1;
                continue;
            }
            if( l2 == l1 )
            {
                similarity += l1;
                continue;
            }
            break;
        }
    }
    return similarity;
}

void readInputLists(const string& fname, vector<int>& list1, vector<int>& list2) {
    ifstream file(fname);
    string line;
    while (getline(file, line)) {
        auto parts = split(line, ' ');
        list1.push_back(stoi(parts[0]));
        list2.push_back(stoi(parts[1]));
    }
}

TEST(Day1,Part1)
{
    vector<int> list1;
    vector<int> list2;
    readInputLists("input/day01.txt", list1, list2);

    int answer = calcListDistance(list1, list2);
    EXPECT_EQ( answer, 1320851 );
    cout << "Answer = " << answer << endl;
}

TEST(Day1,Part2)
{
    vector<int> list1;
    vector<int> list2;
    readInputLists("input/day01.txt", list1, list2);

    int answer1 = calcListSimilarity1(list1,list2);
    int answer2 = calcListSimilarity2(list1,list2);
    EXPECT_EQ( answer1, 26859182 );
    EXPECT_EQ( answer2, 26859182 );
    cout << "Answer = " << answer1 << endl;
}