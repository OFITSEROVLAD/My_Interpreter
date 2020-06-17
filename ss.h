using namespace std; 
 
template < class T, int max_size >
class Stack
{
         T            s [max_size];
         int          top;
public:
                      Stack () { top = 0; }
         void         reset () { top = 0; }
         void         push ( T i );
         T            pop ();
         bool         is_empty () { return top == 0; }
         bool         is_full  () { return top == max_size; }
};
 
/////////////////////////  Класс Poliz  /////////////////////////////
 
class Poliz
{
         Lex        * p;
         int          size;
         int          free;
public:
    Poliz (int max_size)
    {
        p = new Lex [size = max_size];
        free = 0;
    }
    ~Poliz() { delete [] p; }
    void         put_lex ( Lex l )
    {
        p [free] = l;
        free++;
    }
    void         put_lex ( Lex l, int place) { p [place] = l; }
    void         blank    () { free++; }
    int          get_free () { return free; }
    Lex        & operator[] (int index) 
    {
        if (index > size)
            throw "POLIZ ERROR: out of array";
        else 
        {
            if ( index > free )
                throw "POLIZ ERROR: indefinite element of array";
            else
                return p[index];
        }
    }
    void         print ()
    {

        cout << "--------------------------------" << endl;
        cout << "Poliz:" << endl;
        for (int i = 0; i < free; ++i)
            cout << i << ": " << p[i];
        cout << "--------------------------------" << endl;
    }
};
/////////////////////////////////////////////////////////////////

class Parser 
{
    Lex          curr_lex;
    type_of_lex  c_type;
    type_of_lex  tmp_type;
    int          c_val;
    string       c_string_val;   
    Scanner      scan;
    Stack < int, 100 > st_int;
    Stack < type_of_lex, 100 >  st_lex;
    void         P();
    void         D1();
    void         D();
    void         I1();
    void         CONST_();
    void         S();
    void         S1();
    void         E();
    void         E1();
    void         T();
    void         F();
    void         dec ( type_of_lex type);
    void         check_id ();
    void         check_op ();
    void         check_not ();
    void         eq_type ();
    void         eq_bool ();
    void         check_id_in_read ();
    void         gl ()
    {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
        c_string_val = curr_lex.get_string();
    }
    void         get_tmp_type();
public:
    Poliz       prog;
                Parser (const char *program ) : scan (program), prog (1000){};
    void        analyze();
};