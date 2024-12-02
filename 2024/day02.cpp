#include "day02.h"

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