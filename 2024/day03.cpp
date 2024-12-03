#include "common.h"

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

class graph;

class match_node
{
public:
    match_node() = default;
    virtual ~match_node() = default;

    virtual bool match( const char v ){
        return find_edge(v) == nullptr;
    }

    virtual void consume( const char v ){};
    virtual void reset(){};
    virtual bool is_enabled(){ return true; }

    match_node* find_edge( const char v )
    {
        for( const auto& edge : edges )
            if( edge->is_enabled() && edge->match(v) )
                return edge.get();
        return nullptr;
    }

private:
    friend class graph;
    vector<shared_ptr<match_node>> edges;
};

class match_keyword : public match_node
{
private:
    bool enabled = true;
    string keyword;
    int pos = 0;
    std::function<void(void)> oncomplete;

public:
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

    capture_num() = default;
    ~capture_num() = default;

    bool match( const char v ) override {
        return isdigit(v);
    }

    void consume( const char v ) override {
        num *= 10;
        num += v - '0';
    }

    int get_capture() {
        return num;
    }

    virtual void reset() override {
        num = 0;
    }
};

class graph
{
public:
    shared_ptr<match_node>  head = nullptr;

    std::vector<shared_ptr<match_node>> nodes;

    template<typename T, typename ... ArgsT>
    auto add_node( ArgsT&&... args ) -> shared_ptr<T>
    {
        auto new_node = make_shared<T>( std::forward<ArgsT>( args )... );
        nodes.emplace_back( new_node );
        return new_node;
    }

    template<typename T1,typename T2>
    void add_edge( shared_ptr<T1>& from, shared_ptr<T2>& to )
    {
        from->edges.emplace_back( to );
    }

    void reset()
    {
        for( auto& node : nodes )
            node->reset();
    }
};

int _calcMulls( const string& corrupted, bool use_enables )
{
    int total = 0;

    // create graph
    graph g;
    g.head = g.add_node<match_node>();

    auto kw_mul = g.add_node<match_keyword>("mul(");
    auto   num1 = g.add_node<capture_num>();
    auto   num2 = g.add_node<capture_num>();
    auto  comma = g.add_node<match_keyword>(",");

    auto cparen = g.add_node<match_keyword>(")",[&num1,&num2,&total](){
        int a = num1->get_capture();
        int b = num2->get_capture();
        total += a * b;
    });
    
    auto _do = g.add_node<match_keyword>("do()", [&kw_mul](){
        kw_mul->enable();
    });

    auto nt = g.add_node<match_keyword>("n't()", [&kw_mul](){
        kw_mul->disable();
    });

    // create graph edges
    g.add_edge( g.head, kw_mul );   // mul()

    if( use_enables )
    {
        g.add_edge( g.head, _do );  // do()
        g.add_edge( _do, nt );      // don't()
    }

    g.add_edge( kw_mul, num1 ); // mul(25
    g.add_edge( num1, comma );  // mul(25,
    g.add_edge( comma, num2 );  // mul(25,48
    g.add_edge( num2, cparen ); // mul(25,48)

    // match input
    match_node* current = g.head.get();
    for( auto c : corrupted )
    {
        while( !current->match(c) ){
            current = current->find_edge(c);

            if( current == nullptr ){
                current = g.head.get();
                g.reset();
            }
        }

        current->consume( c );

        // if we're going back to the start, reset any captured numbers
        if( current == g.head.get() )
            g.reset();
    }

    return total;
}

TEST( Day3, Part1Examples )
{
    EXPECT_EQ( _calcMulls("mul(10,5)",false), 50 );
    EXPECT_EQ( _calcMulls("xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))", false), 161 );
}

TEST( Day3, Part2Examples )
{
    //                     ---------xxxxxxx.........
    EXPECT_EQ( _calcMulls("mul(10,5)don't()mull(2,4)",true), 50 );
    //                      --------           xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx++++ -------- --> 2*4 + 8*5 = 48
    EXPECT_EQ( _calcMulls("xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))", true), 48 );
}

TEST( Day3, Part1 )
{
    string s = readInput("input/day03.txt");
    int ans = _calcMulls(s,false);
    EXPECT_EQ( ans, 192767529 );
    cout << "Answer = " << ans << endl;
}

TEST( Day3, Part2 )
{
    string s = readInput("input/day03.txt");
    int ans = _calcMulls(s,true);
    EXPECT_EQ( ans, 104083373 );
    cout << "Answer = " << ans << endl;
}