#include "execute.h"

void 
Executer::execute ( Poliz & prog )
{
    Stack < int, 100 > args;
    Stack < string, 100 > string_args;
    int i, j, index = 0, size = prog.get_free();
    string s;

    while ( index < size )
    {
        pc_el = prog [ index ];
        // cout << index << ": ";
        switch ( pc_el.get_type () )
        {
            //-------------------------------------------------------------------------------
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL:
                args.push (pc_el.get_value());
                break;
            //-------------------------------------------------------------------------------
            case LEX_STR:
                string_args.push(pc_el.get_string());
                break;
            //-------------------------------------------------------------------------------
            case LEX_ID:
                i = pc_el.get_value();
                if (TID[i].get_assign())
                {
                    args.push ( TID[i].get_value ());
                    break;
                }
                else
                    throw "EXECUTE ERROR: indefinite identifier";
            //-------------------------------------------------------------------------------
            case LEX_ID_STR:
                i = pc_el.get_value();
                if (TID[i].get_assign())
                {
                    string_args.push ( TID[i].get_string () );
                    break;
                }
                else
                    throw "EXECUTE ERROR: indefinite identifier";
            //-------------------------------------------------------------------------------
            case LEX_NOT: 
                args.push( !args.pop() );
                break;
            //-------------------------------------------------------------------------------
            case LEX_OR:
                i = args.pop();
                args.push ( args.pop() || i );
                break;
            //-------------------------------------------------------------------------------
            case LEX_AND:
                i = args.pop();
                args.push ( args.pop() && i );
                break;
            //-------------------------------------------------------------------------------
            case POLIZ_GO:
                index = args.pop() - 1;
                break;
            //-------------------------------------------------------------------------------
            case POLIZ_FGO:
                i = args.pop();
                if ( !args.pop() ) 
                {
                    index = i-1;
                }
                break;
            //-------------------------------------------------------------------------------
            case LEX_WRITE:
                if (string_args.is_empty()) 
                {
                    cout << args.pop ();
                } 
                else
                {
                    s = string_args.pop ();
                    if (s == "\\n") 
                    {
                        cout << endl;
                    } 
                    else 
                    {
                        cout << s;
                    }
                }
                
                break;
            //-------------------------------------------------------------------------------
            case LEX_READ:
            {
                int k;
                i = args.pop ();
                if ( TID[i].get_type () == LEX_INT )
                {
                    cout << "Input int value for " << TID[i].get_name () << ": ";
                    cin >> k;
                    TID[i].put_value(k);
                }
                else if (TID[i].get_type () == LEX_STR)
                {
                    string s;
                    getline(cin, s);
                    TID[i].put_value(s);
                }
                else
                {
                    char j[20];
                    rep:
                    cin >> j;
                    if (!strcmp(j, "true"))
                    {
                        k = 1;
                    }
                    else if (!strcmp(j, "false"))
                    {
                        k = 0;
                    }
                    else
                    {
                        cout << "Error in input: \"Input boolean value (\"true\" or \"false\")" << endl;
                        goto rep;
                    }
                    TID[i].put_value(k);
                }
                TID[i].put_assign();
                break;
            }
            //-------------------------------------------------------------------------------
            case LEX_PLUS:
                args.push ( args.pop() + args.pop() );
                break;
            //-------------------------------------------------------------------------------
            case LEX_PLUS_STR:
                string_args.push ( string_args.pop() + string_args.pop() );
                break;
            //-------------------------------------------------------------------------------
            case LEX_TIMES:
                args.push ( args.pop() * args.pop() );
                break;
            //-------------------------------------------------------------------------------
            case LEX_MINUS:
                i = args.pop();
                args.push ( args.pop() - i );
                break;
            //-------------------------------------------------------------------------------
            case LEX_SLASH:
                i = args.pop();
                if (!i)
                {
                    args.push(args.pop() / i);
                    break;
                }
                else
                    throw "EXECUTE ERROR: divide by zero";
            //-------------------------------------------------------------------------------
            case LEX_EQEQ:
                args.push ( args.pop() == args.pop() );
                break;
            //-------------------------------------------------------------------------------
            case LEX_EQEQ_STR:
                args.push ( string_args.pop() == string_args.pop() );
                break;
            //-------------------------------------------------------------------------------
            case LEX_LSS:
                i = args.pop();
                args.push ( args.pop() < i);
                break;
            //-------------------------------------------------------------------------------
            case LEX_LSS_STR:
                s = string_args.pop();
                args.push ( string_args.pop() < s);
                break;
            //-------------------------------------------------------------------------------
            case LEX_GTR:
                i = args.pop();
                args.push ( args.pop() > i );
                break;
            //-------------------------------------------------------------------------------
            case LEX_GTR_STR:
                s = string_args.pop();
                args.push ( string_args.pop() > s );
                break;
            //-------------------------------------------------------------------------------
            case LEX_LEQ:
                i = args.pop();
                args.push ( args.pop() <= i );
                break;
            //-------------------------------------------------------------------------------
            case LEX_GEQ:
                i = args.pop();
                args.push ( args.pop() >= i );
                break;
            //-------------------------------------------------------------------------------
            case LEX_NEQ:
                i = args.pop();
                args.push ( args.pop() != i );
                break;
                //-------------------------------------------------------------------------------
            case LEX_NEQ_STR:
                s = string_args.pop();
                args.push(string_args.pop() != s );
                break;
            //-------------------------------------------------------------------------------
            case LEX_EQ:
                i = args.pop();
                j = args.pop();
                TID[j].put_value(i);
                TID[j].put_assign(); 
                break;
            //-------------------------------------------------------------------------------
            case LEX_EQ_STR:
                s = string_args.pop();
                j = args.pop();
                TID[j].put_value(s);
                TID[j].put_assign(); 
                break;
            //-------------------------------------------------------------------------------
            case LEX_PERCENT:
                i = args.pop();
                args.push ( args.pop() % i );
                break;

            default:
                throw "EXECUTE ERROR: unexpected elem in poliz";
        }//end of switch
        ++index;
    };//end of while
    cout << endl << "### Finish of executing ###" << endl;
}


void 
Interpretator::interpretation ()
{
    pars.analyze ();
    E.execute ( pars.prog );
}