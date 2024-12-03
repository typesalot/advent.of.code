#include "day03.h"

int _calcMulls( const string& corrupted, bool use_enables );

string readInput( const string& input )
{
    auto f = ifstream( input );
    stringstream stream;
    string s;
    while( getline(f,s) )
        stream << s;
    return stream.str();
}

int calcMullsAll( const string& input )
{
    string s = readInput(input);
    return _calcMulls(s,false);
}

int calcMullsEnables( const string& input )
{
    string s = readInput(input);
    return _calcMulls(s,true);
}

class match_node
{
public:
    match_node() = default;
    virtual ~match_node() = default;

    virtual bool match( const char v ){
        return find_edge(v) == nullptr;
    }

    virtual void    consume( const char v ){};

    virtual bool    has_capture(){ return false; }
    virtual int     get_capture(){ return 0; }
    virtual void    reset(){};
    virtual bool    is_enabled(){ return true; }

    match_node* find_edge( const char v )
    {
        for( const auto& edge : edges )
            if( edge->is_enabled() && edge->match(v) )
                return edge.get();
        return nullptr;
    }

    vector<shared_ptr<match_node>> edges;
};

class match_keyword : public match_node
{
public:
    bool enabled = true;
    string keyword;
    int pos = 0;
    std::function<void(void)> oncomplete;

    match_keyword( const string& kw ) : keyword( kw ), oncomplete( [](){} ){};
    match_keyword( const string& kw, decltype(oncomplete) cb ) : keyword( kw ), oncomplete( cb ){};
    ~match_keyword() = default;

    bool match( const char v ) override
    {
        if( pos < keyword.length() && 
            keyword[pos] == v )
            return true;
        return false;
    }

    void consume( const char v ) override
    {
        pos++;
        if( pos == keyword.length() )
            oncomplete();
    }

    virtual void reset() override {
        pos = 0;
    }

    bool is_enabled() override{
        return this->enabled;
    }

    void enable(){
        this->enabled = true;
    }

    void disable(){
        this->enabled = false;
    }
};

class capture_num : public match_node
{
public:
    int num = 0;

    bool has_data = false;

    capture_num() = default;
    ~capture_num() = default;

    bool match( const char v ) override {
        return isdigit(v);
    }

    void consume( const char v ) override {
        num *= 10;
        num += v - '0';
    }

    virtual bool has_capture() override {
        return has_data;
    }

    virtual int get_capture() override {
        return num;
    }

    virtual void reset() override {
        has_data = 0;
        num = 0;
    }

    int operator*( const capture_num& lhs ){
        return lhs.num * this->num;
    }
};

int _calcMulls( const string& corrupted, bool use_enables )
{
    int total = 0;

    // create graph nodes
    auto head = make_shared<match_node>();
    auto kw_mul = make_shared<match_keyword>("mul(");
    auto kw_comma = make_shared<match_keyword>(",");
    auto kw_cparen = make_shared<match_keyword>(")");
    auto kw_do = make_shared<match_keyword>("do()", [&kw_mul](){
        kw_mul->enable();
    });
    auto kw_nt = make_shared<match_keyword>("n't()", [&kw_mul](){
        kw_mul->disable();
    });
    auto num1 = make_shared<capture_num>();
    auto num2 = make_shared<capture_num>();

    // create graph edges
    head->edges.push_back(kw_mul);      // mul(

    if( use_enables )
    {
        head->edges.push_back(kw_do);   // do()
        kw_do->edges.push_back(kw_nt);  // don't()
    }

    kw_mul->edges.push_back(num1);      // mul(12
    num1->edges.push_back(kw_comma);    // mul(12,
    kw_comma->edges.push_back(num2);    // mul(12,54
    num2->edges.push_back(kw_cparen);   // mul(12,54)

    auto do_reset = [&]()
    {
        kw_mul->reset();
        kw_comma->reset();
        kw_cparen->reset();
        kw_do->reset();
        kw_nt->reset();
        num1->reset();
        num2->reset();
    };

    // match input
    match_node* current = head.get();
    for( auto c : corrupted )
    {
        while( !current->match(c) ){
            current = current->find_edge(c);

            if( current == nullptr ){
                current = head.get();
                do_reset();
            }
        }

        current->consume( c );

        // if we've captured 2 numbers, do the math
        if( current == kw_cparen.get() )
            total += *num1 * *num2;

        // if we're going back to the start, reset any captured numbers
        if( current == head.get() )
            do_reset();
    }

    return total;
}

TEST( calcMulls, ExampleAll )
{
    EXPECT_EQ( _calcMulls("mul(10,5)",false), 50 );
    EXPECT_EQ( _calcMulls("xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))", false), 161 );
}

TEST( calcMulls, ExampleEnables )
{
    //                     ---------xxxxxxx.........
    EXPECT_EQ( _calcMulls("mul(10,5)don't()mull(2,4)",true), 50 );
    //                      --------           xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx++++ -------- --> 2*4 + 8*5 = 48
    EXPECT_EQ( _calcMulls("xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))", true), 48 );
}