#include "day02.h"

bool debug = false;

// results (input.1.txt)
//  265 - too low
//  274 - too low

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
            s << "," << r;
        else
            s << r;
    return s.str();
}

int numSafeReports( const string& input )
{
    vector<vector<int>> reports;
    readInput( input, reports );

    int safe = 0;

    for( const auto& report : reports )
    {
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

        if( i == report.size() - 1 )
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

    for( const auto& report : reports )
    {
        int i = 0;
        int j = 0;
        bool ltz = false;
        auto is_safe = [&ltz,&i]( int a, int b ) -> bool
        {
            int d = a - b;
            if( d > 3 || d < -3 || d == 0 )
                return false;
            if( i == 0 )
                ltz = d < 0;
            else if( ltz && d > 0 )
                return false;
            else if( !ltz && d < 0 )
                return false;
            return true;
        };

        bool dampen = true;
        while( ++j < report.size() )
        {
            if( !is_safe( report[i], report[j]))
            {
                if( dampen )
                {
                    dampen = false;
                    continue;
                }
                else
                {
                    if( debug ) cout << "{" << tostring(report) << "}" << " @ " << j << endl;
                    break;
                }
            }

            i = j;
        }

        if( j == report.size() )
            safe++;
    }

    return safe;
}

TEST(numSafeReportsWithTolerance,Cases)
{
    EXPECT_EQ( _numSafeReportsWithTolerance( {{3,3,4,7,10,11}} ), 1 );
    EXPECT_EQ( _numSafeReportsWithTolerance( {{2,4,4,7,10,11}} ), 1 );
}