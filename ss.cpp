#include "ss.h" 

using namespace std;


template < class T, int max_size >
void Stack < T, max_size > :: push (T i)
{

    if ( !is_full() ) 
    {   
        // cout << "+" << i << endl;
        s [top++] = i;
    }
    else
      throw "ERROR: Stack_is_full";
}
 
template <class T, int max_size >
T Stack < T, max_size > :: pop ()
{
    if ( !is_empty() )
    {
        // cout << "-" << s[top - 1] << endl;
        return s[--top];
    }
    else
      throw "ERROR: Stack_is_empty";
}

/////////////////////////////////////////////////////////////////

void 
Parser::analyze ()
{
    gl();
    P();
    prog.print();
    // cout << endl << "OK!!!" << endl;
}
 
void 
Parser::P ()
{
    // cout << "P\n";
    if (c_type == LEX_PROGRAM)
        gl();
    else
        throw curr_lex;
    if (c_type == LEX_LCBR)
        gl();
    else
        throw curr_lex;
    D1();

    S1();

    if (c_type == LEX_RCBR)
    {
        gl();
    }
    else
        throw curr_lex;


    if (c_type != LEX_FIN)
        throw curr_lex;
}
 
void 
Parser::D1 ()
{
    // cout << "D1\n";
    while (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOLEAN)
    {   
        D();
        if (c_type == LEX_SEMICOLON) 
        {   
            gl();
        }
        else
            throw curr_lex;
    }
}

void 
Parser::D ()
{
    // cout << "D\n";
    st_int.reset();
    if (c_type == LEX_INT)
    {
        tmp_type = LEX_INT;
    } else if (c_type == LEX_STRING)
    {
        tmp_type = LEX_STR;
    } else if (c_type == LEX_BOOLEAN)
    {
        tmp_type = LEX_BOOLEAN;
    }
    gl();
    I1();
    while (c_type == LEX_COMMA)
    {
        gl();
        I1();
    }
    dec(tmp_type);
}

void 
Parser::I1 ()
{
    // cout << "I1\n";
    int val = c_val;
    if (c_type == LEX_ID)
    {

        st_int.push (c_val);
        gl(); 
    } else 
    {
        throw curr_lex;
    } 
    if (c_type == LEX_EQ) {
        prog.put_lex (Lex (POLIZ_ADDRESS, val));
        gl();
        CONST_();
        if (tmp_type == LEX_STR) 
        {
            prog.put_lex (Lex(LEX_EQ_STR));
        }
        else
        {
            prog.put_lex (Lex(LEX_EQ));
        }
    }
}

void 
Parser::CONST_ ()
{
    // cout << "CONST\n";
    if (c_type == LEX_PLUS || c_type == LEX_MINUS)
    {
        gl();
        if (c_type == LEX_NUM)
        {
            if (tmp_type != LEX_INT)
            {
                throw "SEMANTIC ERROR: cannot initialize a variable of non type 'int' with an lvalue of type 'int'";
            }
        } else
        {
            throw curr_lex;
        } 
    } else if (c_type == LEX_STR)
    {
        if (tmp_type != LEX_STR)
        {
            throw "SEMANTIC ERROR: cannot initialize a variable of non type 'string' with an lvalue of type 'string'";
        }

    } else if (c_type == LEX_TRUE || c_type == LEX_FALSE)
    {
        if (tmp_type != LEX_BOOLEAN)
        {
            throw "SEMANTIC ERROR: cannot initialize a variable of non type 'boolean' with an lvalue of type 'boolean'";
        }

    }
    else  
    {
        throw curr_lex;
    }
    prog.put_lex (curr_lex);
    gl();
}

void 
Parser::S1 ()
{
    // cout << "S1\n";
    while (c_type == LEX_IF || c_type == LEX_WHILE || c_type == LEX_READ || \
        c_type == LEX_WRITE || c_type == LEX_LCBR ||  c_type == LEX_ID || \
        c_type == LEX_BREAK || c_type == LEX_DO)
    {
        S();
    } 
}

void 
Parser::S ()
{
    // cout << "S\n";
    int pl0, pl1, pl2, pl3;
    //-------------------------------------------------------------------------------
    if (c_type == LEX_IF)
    {
        gl();
        if ( c_type == LEX_LPAREN )
        {
            gl();
        }
        else
            throw curr_lex;
        E();
        eq_bool();
        pl2 = prog.get_free ();
        prog.blank();
        prog.put_lex (Lex(POLIZ_FGO));
        if ( c_type == LEX_RPAREN )
        {
            gl();
        }
        else
            throw curr_lex;
        S();
        
        if ( c_type == LEX_ELSE )
        {
            pl3 = prog.get_free();
            prog.blank();
            prog.put_lex (Lex(POLIZ_GO));
            prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()), pl2);
            gl();
            S();
            prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()), pl3);
        }
        else
        {
            prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()), pl2);
        }
    }//end_if
    //-------------------------------------------------------------------------------
    else if (c_type == LEX_WHILE)
    {
        pl0 = prog.get_free();
        gl();
        if ( c_type == LEX_LPAREN )
        {
            gl();
        }
        else
            throw curr_lex;
        E();
        eq_bool();
        pl1 = prog.get_free(); 
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        if ( c_type == LEX_RPAREN )
        {
            gl();
        }
        else
            throw curr_lex;
        S();
        prog.put_lex (Lex (POLIZ_LABEL, pl0));
        prog.put_lex (Lex (POLIZ_GO));
        prog.put_lex (Lex (POLIZ_LABEL, prog.get_free()), pl1);
    }//end_while
    //-------------------------------------------------------------------------------
    else if (c_type == LEX_READ)
    {
        gl();
        if ( c_type == LEX_LPAREN )
        {
            gl();
        }
        else
            throw curr_lex;
        if ( c_type == LEX_ID )
        {
            check_id_in_read();
            prog.put_lex(Lex(POLIZ_ADDRESS,c_val));
            gl();
        }
        else
            throw curr_lex;
        if ( c_type == LEX_RPAREN )
        {
            gl();
        }
        else
            throw curr_lex;
        if ( c_type == LEX_SEMICOLON )
        {
            gl();
            prog.put_lex(Lex(LEX_READ));
        }
        else
            throw curr_lex;
    }//end read
    //-------------------------------------------------------------------------------
    else if (c_type == LEX_WRITE)
    {
        gl();
        if ( c_type == LEX_LPAREN )
        {
            gl();
        }
        else
            throw curr_lex;
        E();
        prog.put_lex(Lex(LEX_WRITE));
        while (c_type == LEX_COMMA) 
        {
            gl();
            E();
            prog.put_lex(Lex(LEX_WRITE));
        }
        if ( c_type == LEX_RPAREN )
        {
            gl();
        }
        else
            throw curr_lex;
        if ( c_type == LEX_SEMICOLON )
        {
            gl();
        }
        else
            throw curr_lex;
    }//end write
    //-------------------------------------------------------------------------------
    else if ( c_type == LEX_ID )
    {
        check_id ();
        prog.put_lex (Lex (POLIZ_ADDRESS, c_val));
        gl();
        if ( c_type == LEX_EQ )
        {
            gl();
        }
        else
            throw curr_lex;
        E();
        eq_type();
        if (c_type == LEX_SEMICOLON)
        {
            gl();
        }
        else
            throw curr_lex;
    }//assign-end
    //-------------------------------------------------------------------------------
    else if (c_type == LEX_LCBR) 
    {
        gl();
        S1();
        if ( c_type == LEX_RCBR )
        {
            gl();
        }
        else
            throw curr_lex;

    } //составной оператор end
    //-------------------------------------------------------------------------------
    else if (c_type == LEX_BREAK)
    {
        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex (Lex(POLIZ_GO));
        gl();
    } // break
    //-------------------------------------------------------------------------------
    else if (c_type == LEX_DO) 
    {
        pl0 = prog.get_free();
        gl();
        S();
        if ( c_type == LEX_WHILE )
        {
            gl();
        }
        else
            throw curr_lex;
        if ( c_type == LEX_LPAREN )
        {
            gl();
        }
        else
            throw curr_lex;
        E();
        eq_bool();
        pl1 = prog.get_free(); 
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        prog.put_lex (Lex (POLIZ_LABEL, pl0));
        prog.put_lex (Lex (POLIZ_GO));
        if ( c_type == LEX_RPAREN )
        {
            gl();
        }
        else
            throw curr_lex;
        prog.put_lex (Lex (POLIZ_LABEL, prog.get_free()), pl1);
    } // do s while (E)
    //-------------------------------------------------------------------------------
    else
        throw curr_lex;


}
void 
Parser::E () 
{
    // cout << "E\n";
    E1();
    if ( c_type == LEX_EQEQ || c_type == LEX_LSS || c_type == LEX_GTR ||
       c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ )
    {
        st_lex.push (c_type);
        gl(); 
        E1();
        check_op(); 
    }
}
 
void 
Parser::E1 ()
{
    // cout << "E1\n";
    T();
    while ( c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR)
    {
        st_lex.push (c_type);
        gl();
        T();
        check_op();
    }
}
 
void 
Parser::T ()
{
    // cout << "T\n";
    F();
    while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND || c_type == LEX_PERCENT)
    {
        st_lex.push (c_type);
        gl();
        F();
        check_op();
    }
}
 
void 
Parser::F () 
{
    // cout << "F\n";
    if ( c_type == LEX_STR ) 
    {
        st_lex.push ( LEX_STR );
        prog.put_lex (curr_lex);
        gl();
    }
    else if ( c_type == LEX_ID ) 
    {
        check_id();
        if (TID[c_val].get_type() == LEX_STR)
        {
            prog.put_lex (Lex (LEX_ID_STR, c_val, c_string_val));
        } else {
            prog.put_lex (Lex (LEX_ID, c_val));
        }   
        gl();
    }
    else if ( c_type == LEX_NUM ) 
    {
        st_lex.push ( LEX_INT );
        prog.put_lex ( curr_lex );
        gl();
    }
    else if ( c_type == LEX_TRUE ) 
    {
        st_lex.push ( LEX_BOOLEAN );
        prog.put_lex (Lex (LEX_TRUE, 1) );
        gl();
    }
    else if ( c_type == LEX_FALSE)
    {
        st_lex.push ( LEX_BOOLEAN );
        prog.put_lex (Lex (LEX_FALSE, 0) );
        gl();
    }
    else if (c_type == LEX_NOT) 
    {
        gl(); 
        F(); 
        check_not();
    }
    else if ( c_type == LEX_LPAREN ) 
    {
        gl(); 
        E();
        if ( c_type == LEX_RPAREN)
            gl();
        else 
            throw curr_lex;
    }
    else 
        throw curr_lex;
}

////////////////////////////////////////////////////////////////
void Parser::dec ( type_of_lex type ) 
{
    int i;
    while ( !st_int.is_empty()) 
    {
        i = st_int.pop();
        if ( TID[i].get_declare() ) 
            throw "SEMANTIC ERROR: second time declaration";
        else 
        {
            TID[i].put_declare();
            TID[i].put_type(type);
        }
    }
}
 
void Parser::check_id () 
{
    if ( TID[c_val].get_declare() ) 
    {
        st_lex.push ( TID[c_val].get_type() );
    }
    else 
        throw "SEMANTIC ERROR: ID wasn't declared";
}
 
void Parser::check_op () 
{
    type_of_lex t1, t2, op;

    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();


    if (t1 != t2) 
    {
        throw "SEMANTIC ERROR: wrong types are in operation";
    }
    else
    {
        if (t1 == LEX_STR) 
        {
            if (op == LEX_EQEQ)
            {
                st_lex.push(LEX_BOOLEAN);
                prog.put_lex (Lex(LEX_EQEQ_STR));
            } 
            else if (op == LEX_LSS)
            {
                st_lex.push(LEX_BOOLEAN);
                prog.put_lex (Lex(LEX_LSS_STR));
            }
            else if (op == LEX_GTR)
            {
                st_lex.push(LEX_BOOLEAN);
                prog.put_lex (Lex(LEX_GTR_STR));
            } 
            else if (op == LEX_NEQ)
            {
                st_lex.push(LEX_BOOLEAN);
                prog.put_lex (Lex(LEX_NEQ_STR));
            }  

            else if (op == LEX_PLUS)
            {
                st_lex.push(LEX_STR);
                prog.put_lex (Lex(LEX_PLUS_STR));
            } 
            else
                throw "SEMANTIC ERROR: inccorect operation with strings";
        }
        else if (t1 == LEX_BOOLEAN) 
        {
            if (op == LEX_AND|| op == LEX_OR)
            {
                st_lex.push(LEX_BOOLEAN);
                prog.put_lex (Lex(op));
            } 
            else
                throw "SEMANTIC ERROR: inccorect operation with boolean";

        }
        else if (t1 == LEX_INT) 
        {
            if (op == LEX_LSS || op == LEX_GTR || op == LEX_LEQ || \
             op == LEX_NEQ || op == LEX_GEQ || op == LEX_EQEQ)
            {
                st_lex.push(LEX_BOOLEAN);
            } 
            else
            {
                st_lex.push(LEX_INT);
            }
            prog.put_lex (Lex(op));
        } 
    }
}
 
void Parser::check_not () 
{
    if (st_lex.pop() != LEX_BOOLEAN)
        throw "SEMANTIC ERROR: incorrect type, operation \"NOT\"";
    else 
    {
        st_lex.push (LEX_BOOLEAN);
        prog.put_lex (Lex (LEX_NOT));
    }
}
 
void Parser::eq_type () 
{
    type_of_lex t1;
    t1 = st_lex.pop();
    if (t1 != st_lex.pop())
        throw "SEMANTIC ERROR: inccorect types, operation \"=\" ";
    if (t1 == LEX_STR) 
    {
        prog.put_lex(Lex(LEX_EQ_STR));
    }
    else
    {
        prog.put_lex(Lex(LEX_EQ));
    }
}
 
void Parser::eq_bool () 
{
    if ( st_lex.pop() != LEX_BOOLEAN )
        throw "SEMANTIC ERROR: expression is not BOOLEAN";
}
 
void Parser::check_id_in_read ()
{
    if ( !TID[c_val].get_declare() )
        throw "SEMANTIC ERROR: ID is not declared";
}
////////////////////////////////////////////////////////////////
 