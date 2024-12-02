#include "day02.h"

bool debug = false;

void readInput( const string& input, vector<vector<int>>& reports )
{
    auto f = ifstream(input);
    string line;
    while(getline(f,line))
    {
        auto elems = split(line,' ');
        vector<int>& report = reports.emplace_back();
        for( auto e : elems )
            report.emplace_back( stoi(e));
    }
}

string tostring( const vector<int>& report )
{
    stringstream s;
    for( auto r : report )
        if( s.str().length() )
            s << ", " << r;
        else
            s << r;
    return s.str();
}

vector<int> compute_grad( const vector<int>& report )
{
    vector<int> g;
    for( int i = 0; i < report.size() - 1; i++ )
        g.push_back( report[i+1]-report[i]);
    return g;
}

std::tuple<bool,int> isSafe( const vector<int>& report )
{
    bool safe = false;

    bool ltz = false;
    int i = 0;

    for( i = 0; i < report.size() - 1; i++ )
    {
        int grad = report[i] - report[i+1];
        
        // check1: gradient magnitude
        if( grad > 3 || grad < -3 || grad == 0 )
            break;

        // check1: direction
        if( i == 0 )
            ltz = grad < 0;
        else if( ltz && grad > 0 )
            break;
        else if( !ltz && grad < 0 )
            break;
    }

    safe = ( i == report.size() - 1 );

    return {safe,i};
}

int numSafeReports( const string& input )
{
    vector<vector<int>> reports;
    readInput( input, reports );

    int safe = 0;

    for( const auto& report : reports )
    {
        auto [is_safe,i] = isSafe(report);
        if( is_safe )
            safe++;
    }

    return safe;
}

int numSafeReportsWithTolerance( const string& input )
{
    vector<vector<int>> reports;
    readInput(input,reports);
    return _numSafeReportsWithTolerance(reports);
}

int _numSafeReportsWithTolerance( const vector<vector<int>>& reports )
{
    int safe = 0;

    int tmp;
    for( const auto& report : reports )
    {
        auto [is_safe,i] = isSafe(report);
        if( !is_safe )
        {
            // opportunity window = [i-1,i,i+1]

            // corner case: removing i-1, but only for i=1 when removing the 0th element might make a safe gradient
            if( i == 1 )
            {
                vector<int> a = report;
                a.erase( a.begin() - 1 );
                tie( is_safe, tmp ) = isSafe(a);
            }

            // common case 1: removing i makes i-1 -> i+1 safe
            if( !is_safe )
            {
                vector<int> a = report;
                a.erase( a.begin() + i );
                tie( is_safe, tmp ) = isSafe(a);
            }
            
            // common case 2: removing i+1 makes i-1 -> i safe
            if( !is_safe )
            {
                vector<int> a = report;
                a.erase( a.begin() + i + 1 );
                tie( is_safe, tmp ) = isSafe(a);
            }
            
            if( is_safe )
                safe++;
            if( debug )
                cout << is_safe << " = [" << tostring(report) << "]" << endl;
        }
        else
            safe++;
    }

    return safe;
}

TEST(numSafeReportsWithTolerance,Examples)
{
    EXPECT_EQ( _numSafeReportsWithTolerance( {{7,6,4,2,1}} ), 1 ); // Safe without removing any level.
    EXPECT_EQ( _numSafeReportsWithTolerance( {{1,2,7,8,9}} ), 0 ); // Unsafe regardless of which level is removed.
    EXPECT_EQ( _numSafeReportsWithTolerance( {{9,7,6,2,1}}), 0);   // Unsafe regardless of which level is removed.
    EXPECT_EQ( _numSafeReportsWithTolerance( {{1,3,2,4,5}}), 1);   // Safe by removing the second level, 3.
    EXPECT_EQ( _numSafeReportsWithTolerance( {{8,6,4,4,1}}), 1);   // Safe by removing the third level, 4.
    EXPECT_EQ( _numSafeReportsWithTolerance( {{1,3,6,7,9}}), 1);   // Safe without removing any level.
}

TEST(numSafeReportsWithTolerance,Cases)
{
    EXPECT_EQ( _numSafeReportsWithTolerance( {{49,52,55,57,58,60,65}} ), 1 );
    EXPECT_EQ( _numSafeReportsWithTolerance( {{81,85,88,89,91,93}}), 1 );
    EXPECT_EQ( _numSafeReportsWithTolerance( {{47, 50, 48, 45, 42, 39, 38}}), 1); // Removing the first element makes safe gradient
}