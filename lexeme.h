#include <iostream>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h> 
 
using namespace std;

enum type_of_lex
{
	LEX_NULL, /*0*/
	LEX_PROGRAM, 
	LEX_INT, 
	LEX_STRING, 
	LEX_IF, 
	LEX_ELSE, 
	LEX_WHILE, 
	LEX_READ, 
	LEX_WRITE, 
	LEX_NOT, /*9*/
    
    LEX_AND, 
    LEX_OR, 
    LEX_DO, 
    LEX_BOOLEAN, 
    LEX_TRUE, 
    LEX_FALSE, 
    LEX_BREAK, /*16*/

	LEX_FIN, /*17*/
	
	LEX_SEMICOLON, 
	LEX_COMMA, 
	LEX_LPAREN, 
	LEX_RPAREN, 
	LEX_EQ, 
	LEX_LSS, 
	LEX_GTR, 
	LEX_PERCENT,/*25*/
	LEX_PLUS, 
	LEX_MINUS, 
	LEX_TIMES, 
	LEX_SLASH, 
	LEX_LEQ, 
	LEX_NEQ, 
	LEX_GEQ, 
	LEX_EQEQ, 
	LEX_LCBR, 
	LEX_RCBR, /*35*/

	LEX_NUM, /*36*/
	LEX_ID, /*37*/
	LEX_STR, /*38*/

	POLIZ_LABEL, /*39*/
  	POLIZ_ADDRESS, /*40*/
  	POLIZ_GO, /*41*/
  	POLIZ_FGO, /*42*/

  	LEX_EQ_STR, /*43*/
  	LEX_PLUS_STR,/*44*/
  	LEX_LSS_STR, /*45*/
  	LEX_GTR_STR, /*46*/
  	LEX_EQEQ_STR, /*47*/
  	LEX_NEQ_STR, /*48*/
  	LEX_READ_STR, /*49*/

  	LEX_ID_STR /*50*/
 };				
 /////////////////////////  Класс Lex  //////////////////////////
 string st = " ";
class Lex
{
	type_of_lex t_lex;
	int v_lex;
	string str_v_lex;
	int str_lex;
	int chr_lex;
public:
	Lex ( type_of_lex t = LEX_NULL, int v = 0, string v_str = "@@@@@@", int str = 0, int chr = 0);
	type_of_lex  get_type ();
	int get_value ();
	int get_str ();
	int get_chr ();
	string get_string ();
	friend ostream& operator << (ostream &s, Lex l)
	{
		if (l.get_type() == LEX_STR)
		{
			s << "(" << l.t_lex << "," << l.str_v_lex << ")"  << endl;;
		} else 
		{
			s << "(" << l.t_lex << "," << l.v_lex << ")"  << endl;
		}
	return s;
	}
};
/////////////////////  Класс Ident  ////////////////////////////
class Ident
{
	char*        name;
	bool         declare;
	type_of_lex  type;
	bool         assign;
	int          value;
	string       str_value;
public:
	Ident();
	char       * get_name ();
	void         put_name (const char *n);
	bool         get_declare ();
	void         put_declare ();
	type_of_lex  get_type    ();
	void         put_type    (type_of_lex t);
	bool         get_assign  ();
	void         put_assign  ();
	int          get_value   ();
	string       get_string  ();
	void         put_value   (int v);
	void         put_value   (string v);
};

//////////////////////  Класс Tabl_ident  ///////////////////////
class Tabl_ident
{
	Ident      * p;
	int          size;
	int          top;
public:
	Tabl_ident ( int max_size );
	~Tabl_ident ();
	Ident      & operator[] ( int k );
	int          put ( const char *buf );
};

/////////////////////  Класс Scanner  //////////////////////////////
class Scanner
{
public:
	enum state 
	{ 
		H, IDENT, NUMB, STR, ALE, DELIM, NEQ, COMMENT_1, COMMENT_2, COMMENT_3
	};
	static const char       * TW    [];
	static type_of_lex  words [];
	static const char       * TD    [];
	static type_of_lex  dlms  [];
	state        CS;
	FILE       * fp;
	char         c;
	char         buf [ 80 ];
	int          buf_top;
	int 	     str;
	int          chr;
	int 	     str1;
	int          chr1;			
	
	void         clear ();	
	void         add ();
	int          look ( const char *buf, const char **list );
	void         gc ();
public:
	Scanner ( const char * program );
	Lex get_lex ();
};