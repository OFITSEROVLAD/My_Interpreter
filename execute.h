class Executer
{
	Lex          pc_el;
public:
    void         execute ( Poliz & prog );
};


class Interpretator
{
    Parser   pars;
    Executer E;
public:
             Interpretator  (char* program): pars (program) {}
    void     interpretation ();
};