#include "day03.h"

int _calcMulls( const string& corrupted );

string readInput( const string& input )
{
    auto f = ifstream( input );
    stringstream stream;
    string s;
    while( getline(f,s) )
        stream << s;
    return stream.str();
}

int calcMulls( const string& input )
{
    string s = readInput(input);
    return _calcMulls(s);
}

class match_node
{
public:
    match_node( match_node* _head ) : head(_head) {};
    virtual ~match_node() = default;

    virtual match_node* match( const char v ) = 0;
    virtual bool        has_capture(){ return false; }
    virtual int         get_capture(){ return 0; }
    virtual void        reset(){};
    bool                is_head(){ return this == this->head; }

    match_node*            head = nullptr;
    shared_ptr<match_node> next = nullptr;
};

class exact_single : public match_node
{
public:
    char _c;

    exact_single( match_node* _head, char c ) : match_node(_head), _c( c ){};
    ~exact_single() = default;

    match_node* match( const char v )
    {
        if( this->_c == v )
            return this->next.get();
        return this->head;
    }
};

class capture_num : public match_node
{
public:
    char _c;
    int num = 0;

    bool has_data = false;

    capture_num( match_node* _head, char c ) : match_node(_head), _c( c ){};
    ~capture_num() = default;

    match_node* match( const char v ) override
    {
        if( isdigit(v) )
        {
            num *= 10;
            num += v - '0';
            return this;
        }

        if( this->_c == v )
        {
            has_data = true;
            return this->next.get();
        }

        this->reset();
        return this->head;
    }

    virtual bool has_capture() override
    {
        return has_data;
    }

    virtual int get_capture() override
    {
        return num;
    }

    virtual void reset() override
    {
        has_data = 0;
        num = 0;
    }

    int operator*( const capture_num& lhs )
    {
        return lhs.num * this->num;
    }
};

int _calcMulls( const string& corrupted )
{
    int total = 0;

    // create graph
    auto head = make_shared<exact_single>(nullptr,'m');
    head->head = head.get();

    match_node* current = head.get();

    current->next = make_shared<exact_single>(head.get(),'u');
    current = current->next.get();
    current->next = make_shared<exact_single>(head.get(),'l');
    current = current->next.get();
    current->next = make_shared<exact_single>(head.get(),'(');
    current = current->next.get();
    auto num1 = make_shared<capture_num>(head.get(), ',');
    auto num2 = make_shared<capture_num>(head.get(), ')');
    current->next = num1;
    num1->next = num2;
    num2->next = head;

    // match input
    current = head.get();
    for( auto c : corrupted )
    {
        // get next element in the graph based on the current character
        auto next = current->match( c );

        // if we've captured 2 numbers, do the math
        if( num1->has_capture() &&
            num2->has_capture() )
        {
            total += *num1 * *num2;
        }

        // if we're going back to the start, reset any captured numbers
        if( next->is_head() )
        {
            num1->reset();
            num2->reset();
        }

        current = next;
    }

    return total;
}

TEST( calcMulls, Example )
{
    EXPECT_EQ( _calcMulls("mul(10,5)"), 50 );
    EXPECT_EQ( _calcMulls("xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))"), 161 );
}