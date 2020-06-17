#include "lexeme.h" 
 
using namespace std;
			

 /////////////////////////  Класс Lex  //////////////////////////
 

Lex::Lex ( type_of_lex t, int v, string v_str, int str, int chr): t_lex (t), v_lex (v), str_v_lex(v_str), str_lex (str), chr_lex (chr) { }
type_of_lex  Lex::get_type ()  
{ 
	return t_lex; 
}
int 
Lex::get_value () 
{ 
	return v_lex; 
}
int 
Lex::get_str () 
{ 
	return str_lex; 
}
int 
Lex::get_chr () 
{ 
	return chr_lex; 
}
string 
Lex::get_string ()
{
	return str_v_lex;
}



/////////////////////  Класс Ident  ////////////////////////////
 

Ident::Ident() 
{ 
	declare = false; 
	assign = false; 
}
char* 
Ident::get_name () 
{ 
	return name; 
}
void         
Ident::put_name (const char *n)
{
	name = new char [ strlen(n)+1];
	strcpy(name,n);
}
bool         
Ident::get_declare () 
{ 
	return declare; 
}
void         
Ident::put_declare () 
{ 
	declare = true; 
}
type_of_lex  
Ident::get_type    () 
{ 
	return type; 
}
void         
Ident::put_type    (type_of_lex t) 
{ 
	type = t; 
}
bool         
Ident::get_assign  () 
{ 
	return assign; 
}
void         
Ident::put_assign  ()
{ 
	assign = true; 
}
int          
Ident::get_value   () 
{ 
	return value; 
}
string        
Ident::get_string   () 
{ 
	return str_value; 
}
void         
Ident::put_value   (int v)
{ 
	value = v; 
}
void         
Ident::put_value   (string v)
{ 
	str_value = v; 
}


//////////////////////  Класс Tabl_ident  ///////////////////////
 

Tabl_ident::Tabl_ident ( int max_size )
{
	p = new Ident [size = max_size];
	top = 1;
}
Tabl_ident::~Tabl_ident () 
{ 
	delete [] p; 
}
Ident& 
Tabl_ident::operator[] ( int k ) 
{ 
	return p[k]; 
}
int 
Tabl_ident::put ( const char *buf )
{
	for ( int j = 1; j < top; j++ )
	{
		if ( !strcmp ( buf, p[j].get_name() ) )
		{
			return j;
		}
	}
	p[top].put_name(buf);
	++top;
	return top-1;
}


/////////////////////  Класс Scanner  //////////////////////////////
 

void         
Scanner::clear ()
{
	buf_top = 0;
	for ( int j = 0; j < 80; j++ )
	{
		buf[j] = '\0';
	}
}

void         
Scanner::add ()
{	
	if ( buf_top < 79 )
	{
		buf [ buf_top++ ] = c;
	}
}

int          
Scanner::look ( const char *buf, const char **list )
{
	int i = 0;
	while (list[i])
	{
		if ( !strcmp(buf, list[i]) )
		{
			return i;	
		}
	    ++i;
	}
	return 0;
}

void         
Scanner::gc ()
{
	c = fgetc (fp);
	if (c == '\n' || c == '\r') 
	{
		str1 = str;
		chr1 = chr;
		str += 1;
		chr = 0;
	}
	else if (c== '\t')
	{
		str1 = str;
		chr1 = chr;
		chr += 4;
	}
	else 
	{
		str1 = str;
		chr1 = chr;
		chr +=1;
	}
}

Scanner::Scanner ( const char * program )
{
	fp = fopen ( program, "r" );
	CS = H;
	clear();
	str = 1;
	chr = 0;
	gc();
}


////////////////////////////////////////////////////////////////////

const char *
Scanner::TW    [] = {"", "program", "int", "string", "if", "else", "while", "read", "write", "not", 
					 "and", "or", "do", "boolean", "true", "false", "break", NULL};

const char *
TW1   [] = {"LEX_NULL", "LEX_PROGRAM", "LEX_INT", "LEX_STRING", "LEX_IF", "LEX_ELSE", "LEX_WHILE", "LEX_READ", "LEX_WRITE", "LEX_NOT",
                     "LEX_AND", "LEX_OR", "LEX_DO", "LEX_BOOLEAN", "LEX_TRUE", "LEX_FALSE", "LEX_BREAK", "LEX_NULL"};

type_of_lex
Scanner::words [] = {LEX_NULL, LEX_PROGRAM, LEX_INT, LEX_STRING, LEX_IF, LEX_ELSE, LEX_WHILE, LEX_READ, LEX_WRITE, LEX_NOT,
                     LEX_AND, LEX_OR, LEX_DO, LEX_BOOLEAN, LEX_TRUE, LEX_FALSE, LEX_BREAK, LEX_NULL};

 ////////////////////////////////////////////////////////////////////
const char *
Scanner::TD    [] = {"", "",  ";", ",", "(", ")", "=", "<", ">", "%",
					"+", "-", "*", "/", "<=", "!=", ">=", "==", "{", "}", NULL};

const char *
TD1   [] = {"LEX_NULL", "LEX_FIN", "LEX_SEMICOLON", "LEX_COMMA", "LEX_LPAREN", "LEX_RPAREN", "LEX_EQ", "LEX_LSS", "LEX_GTR", "LEX_PERCENT",
                    "LEX_PLUS", "LEX_MINUS", "LEX_TIMES", "LEX_SLASH", "LEX_LEQ", "LEX_NEQ", "LEX_GEQ", "LEX_EQEQ", "LEX_LCBR", "LEX_RCBR", "LEX_NULL"};

type_of_lex
Scanner::dlms  [] = {LEX_NULL, LEX_FIN, LEX_SEMICOLON, LEX_COMMA, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, LEX_GTR, LEX_PERCENT,
                    LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_EQEQ, LEX_LCBR, LEX_RCBR, LEX_NULL};

 ////////////////////////////////////////////////////////////////////

 Tabl_ident TID ( 100 );

 ////////////////////////////////////////////////////////////////////

Lex 
Scanner::get_lex () 
{
	int d, j;
	CS = H;
	
	do
	{
		switch(CS)
		{
			case H:
				if ( c ==' ') 
				{
					gc();
				}
				else if (c == '\n' || c == '\r') 
				{
					gc();
				}
				else if (c == '\t')
				{
					gc();
				}
				else if ( isalpha(c) )
				{
					clear();
					add();
					gc();
					CS = IDENT;
				}
				else if ( isdigit(c) )
				{
					d = c - '0';
					gc();
					CS = NUMB;
				}
				else if ( c == '<' || c == '>' || c == '=')
				{ 
					clear(); 
					add(); 
					gc();
					CS = ALE; 
				}
				else if ( c == '"')
				{ 
					clear(); 
					// add(); 
					gc(); 
					CS = STR; 
				}
				else if (c == EOF)
				{
					return Lex(LEX_FIN, 0, st, str, chr);
				}
				else if (c == '!')
				{
					clear();
					add();
					gc();
					CS = NEQ;
				}
				else if (c == '/')
				{
					clear();
					gc();
					CS = COMMENT_1;
				}
				else 
					CS = DELIM;
				break;

			case IDENT:
				j = look (buf, TW);
				if ( isalpha(c) || isdigit(c) ) 
				{
					add(); 
					gc();
				}
				else if (j)
				{
					cout.width(25);
					cout << buf;
					cout.width(20);
					cout << TW1[j];
					cout.width(25);
					cout << j;
					cout.width(5);
					cout << str1;
					cout.width(5);
					cout << chr1 - buf_top + 1<< endl;
					return Lex (words[j], j, st, str1, chr1 - buf_top + 1);
				}
				else
				{
					j = TID.put(buf);
					cout.width(25);
					cout << buf;
					cout.width(20);
					cout << "LEX_ID";
					cout.width(25);
					cout << j;
					cout.width(5);
					cout << str1;
					cout.width(5);
					cout << chr1 - buf_top + 1<< endl;
					return Lex (LEX_ID, j, st, str1, chr1 - buf_top + 1);
				}
				break;

			case NUMB:
				if ( isdigit(c) ) 
				{
					d = d * 10 + (c - '0'); gc();
				}
				else
				{
					cout.width(25);
					cout << d;
					cout.width(20);
					cout <<  "LEX_NUM" ;
					cout.width(25);
					cout <<  d;
					cout.width(5);
					cout << str1;
					cout.width(5);
					cout << chr1 - buf_top + 1<< endl;
					return Lex (LEX_NUM, d, st, str1, chr1 - buf_top + 1);
				}
				break;

			case ALE:
				if ( c == '=')
				{
					add();
					gc();
					j = look ( buf, TD );
					cout.width(25);
					cout << buf;
					cout.width(20);
					cout << TD1[j];
					cout.width(25);
					cout << j;
					cout.width(5);
					cout << str1;
					cout.width(5);
					cout << chr1 - buf_top + 1<< endl;
					return Lex ( dlms[j], j, st, str1, chr1 - buf_top + 1);
				}
				else
				{
					j = look ( buf, TD );
					cout.width(25);
					cout << buf;
					cout.width(20);
					cout << TD1[j];
					cout.width(25);
					cout << j;
					cout.width(5);
					cout << str1;
					cout.width(5);
					cout << chr1 - buf_top + 1<< endl;
					return Lex ( dlms[j], j, st,str1, chr1 - buf_top + 1);
				}
				break;

			case STR:
				if ( c == EOF) 
				{
					throw " LEX ERROR: no end of string";
				} else if ( c != '"' ) 
				{
					add();
					gc();
				}
				else
				{
					// add();
					gc();
					cout.width(25);
					cout << buf;
					cout.width(20);
					cout <<  "LEX_STR" ;
					cout.width(25);
					cout <<  buf;
					cout.width(5);
					cout << str1;
					cout.width(5);
					cout << chr1 - buf_top + 1<< endl;
					return Lex (LEX_STR, 0, buf, str1, chr1 - buf_top + 1);
				}
				break;

			case COMMENT_1:
				if (c == EOF) 
				{
					throw " LEX ERROR: unexpected '/' in the end";
				} else if ( c == '*' ) 
				{
					gc();
					CS = COMMENT_2;
				}
				else
				{
					throw " LEX ERROR: expected '*' after '/'";
				}
				break;

			case COMMENT_2:
				if (c == EOF) 
				{
					throw " LEX ERROR: no end of comment";
				} else if ( c == '*' ) 
				{
					gc();
					CS = COMMENT_3;
				}
				else
				{
					gc();
				}
				break;

			case COMMENT_3:
				if (c == EOF) 
				{
					throw " LEX ERROR: no end of comment";
				} else if ( c == '/' ) 
				{
					gc();
					CS = H;
				}
				else if ( c == '*')
				{
					gc();
				}
				else
				{
					gc();
					CS = COMMENT_2;
				}
				break;

			case NEQ:
				if (c == '=')
				{
					add();
					gc();
					j = look ( buf, TD );
					cout.width(25);
					cout << buf;
					cout.width(20);
					cout << "LEX_NEQ";
					cout.width(25);
					cout << j;
					cout.width(5);
					cout << str1;
					cout.width(5);
					cout << chr1 - buf_top + 1<< endl;
					return Lex ( LEX_NEQ, j, st, str1, chr1 - buf_top + 1);
				}
				else
				{
					throw '!';
				}
				break;

			case DELIM:
				clear();
				add();
				j = look ( buf, TD);
				if (j)
				{
					gc();
					cout.width(25);
					cout << buf;
					cout.width(20);
					cout << TD1[j];
					cout.width(25);
					cout << j;
					cout.width(5);
					cout << str1;
					cout.width(5);
					cout << chr1 - buf_top + 1<< endl;
					return Lex ( dlms[j], j, st, str1, chr1 - buf_top + 1);
				}
				else
				{
					throw c;
				}
				break;
		}//end switch
	} while (true);
}
