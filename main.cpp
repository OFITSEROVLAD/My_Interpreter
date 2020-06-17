#include "lexeme.cpp"
#include "ss.cpp" 
#include "execute.cpp"
int 
main (int argc, char* argv[])
{
	try
	{
		// cout << "The lexical stage:" << endl;
		cout.width(25);
		cout << "_code_";
		cout.width(20);
		cout << "type_of_lex";
		cout.width(25);
		cout << "val";
		cout.width(5);
		cout << "str";
		cout.width(5);
		cout << "chr"<< endl << endl;
		// Lex curr_lex;
		// Scanner scan (argv[1]);
		// curr_lex = scan.get_lex();
		// while( curr_lex.get_type() != 17 )
		// {
		// 	cout << endl;
		// 	curr_lex = scan.get_lex();
		// }

		// cout << "The syntactic/semantic stage:" << endl;
		// Parser   pars(argv[1]);
		// pars.analyze ();

		// cout << "\n EXECUTE:  " << endl;

		Interpretator I (argv[1]);
		I.interpretation ();
		
		return 0;
	}
	catch (char c)
	{
		cout << argv[1] <<": ERROR: unexpected symbol: " << c << endl;
		return 1;
	}
	catch (Lex l)
	{
		cout << argv[1] << ", " << l.get_str() << ", " << l.get_chr();
		cout << ": SYNTAX ERROR: unexpected lexeme: " << l;
		return 1;
	}
	catch ( const char *source )
	{
		cout << argv[1] << ": " << source << endl;
		return 1;
	}
}