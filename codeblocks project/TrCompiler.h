/*
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "TrBor.h"
using namespace TrBorOperations;*/

void init_Bor(TrBor *B)
{
    char a01[][13] = {"nop",//0
    "arr", "arradd","arrrem",//3
    "bor","boradd","borrem",//6
    "borgts","borgtn","obj",//9
    "env","run",//11
    "chg","met","jmp",//14
    "end","psh","cln",//17
    "var","pop","mth",//20
    "set","get","tstnil",//23
    "tstlrgoreql","tstlrg","tsteql",//26
    "tststk","tstisa","tstiso",//29
    "tstisi","tstisf","tstiss",//32
    "tstisc","tstisv","tstflg",//35
    "tstabvoridt","tstabv","tstidt",//38
    "str","mts","fts",//41
    "stm","vct","vctpshbck",//44
    "vctpshfrt","vctpopbck","vctpopfrt",//47
    "vctget","vctset","vctgsz",//50
    "vctrsz","txtlen","txtctn",//53
    "txtfnd","txtspl","txtdel",//56
    "prt","opnrdf","opnwrf",//59
    "cls","nxt","prv",//62
    "wrt","scndef","scnfltfil",//65
    "scnfltstr",//66
    "endenv"};//67
    TrString str;
    for (int i = 0; i < 68; ++i)
    {
        cstr_to_TrString(&str, a01[i]);
        borgetnumber(B, &str);
    }
}

#include <vector>

struct change
{
    int val;
    vector < int > shifts;
};

struct token
{
    char type;//string int comma  name endline math
    char * content;
};

void convert_to_tokens(vector < token > &code, FILE *f)
{
    char q;
    q = fgetc(f);
    while (!feof(f))
    {
        while (!feof(f) && (q == ' ' || q == '\t'))
            q = fgetc(f);
        if (q == '\n')
        {//endline
            token x;
            x.type = 'e';
            x.content = new char[2];
            x.content[0] = '\n';
            x.content[1] = 0;
            code.push_back(x);
        } else if (q == ',')
        {//comma
            token x;
            x.type = 'c';
            x.content = new char[2];
            x.content[0] = ',';
            x.content[1] = 0;
            code.push_back(x);
        } else if (q == ';')
        {//trash commentars
            while (q != '\n' && !feof(f))
                q = fgetc(f);
            continue;
        } else if (q == '\\')
        {//string
            token x;
            x.type = 's';
            vector < char > string;
            while (q == '\\')
            {
                q = fgetc(f);
                string.push_back(q);
                q = fgetc(f);
            }
            int s = string.size();
            x.content = new char[s + 1];
            for (int i = 0; i < s; ++i)
                x.content[i] = string[i];
            x.content[s] = 0;
            code.push_back(x);
            continue;
        } else if (q == '-' || q == '+' || (q >= '0' && q <= '9'))
        {//int?
            char fi = q;
            q = fgetc(f);
            if ((q >= '0' && q <= '9') || (fi >= '0' && fi <= '9'))
            {//int
                token x;
                x.type = 'i';
                vector < char > numb;
                numb.push_back(fi);
                while (q >= '0' && q <= '9')
                {
                    numb.push_back(q);
                    q = fgetc(f);
                }
                int s = numb.size();
                x.content = new char[s + 1];
                for (int i = 0; i < s; ++i)
                    x.content[i] = numb[i];
                x.content[s] = 0;
                code.push_back(x);
                continue;
            }
            else
            {//math
                token x;
                x.type = 'm';
                x.content = new char[2];
                x.content[0] = fi;
                x.content[1] = 0;
                code.push_back(x);
                continue;
            }
        } else if (q == '*' || q == '/' || q == '%')
        {//math
            token x;
            x.type = 'm';
            x.content = new char[2];
            x.content[0] = q;
            x.content[1] = 0;
            code.push_back(x);
        } else if (q == '.' || q =='_' || (q >= 'a' && q <= 'z') || (q >= 'A' && q <= 'Z'))
        {//name
            token x;
            x.type = 'n';
            vector < char > string;
            while (q == '.' || q =='_' || (q >= 'a' && q <= 'z') || (q >= 'A' && q <= 'Z') || (q >= '0' && q <= '9'))
            {
                string.push_back(q);
                q = fgetc(f);
            }
            int s = string.size();
            x.content = new char[s + 1];
            for (int i = 0; i < s; ++i)
                x.content[i] = string[i];
            x.content[s] = 0;
            code.push_back(x);
            continue;
        }
        q = fgetc(f);
    }
}

int write_variable(int EnvNumber, token &t, vector < char > &bytecode, vector < change > &sizes, TrBor *var, vector < change > *vars)
{
    if (t.type != 'n')
        return -1;
    TrString tmp;
    cstr_to_TrString(&tmp, t.content);
    int ntmp = borgetnumber(var, &tmp);
    change empty_change;
    if (ntmp == (*vars).size())
    {
        (*vars).push_back(empty_change);
        (*vars)[ntmp].val = ntmp * 4;
    }
    //push var
    (*vars)[ntmp].shifts.push_back(bytecode.size());
    bytecode.push_back(0);
    bytecode.push_back(0);
    bytecode.push_back(0);
    bytecode.push_back(0);
    sizes[EnvNumber].val += 4;
    return 0;
}

int write_int(int EnvNumber, token &t, vector < char > &bytecode, vector < change > &sizes)
{
    if (t.type != 'i')
        return -1;
    int a = atoi(t.content);
    char *p = (char *)&a;
    bytecode.push_back(p[0]);
    bytecode.push_back(p[1]);
    bytecode.push_back(p[2]);
    bytecode.push_back(p[3]);
    sizes[EnvNumber].val += 4;
    return 0;
}

void write_void(int EnvNumber, vector < char > &bytecode, vector < change > &sizes)
{
    bytecode.push_back(0);
    bytecode.push_back(0);
    bytecode.push_back(0);
    bytecode.push_back(0);
    sizes[EnvNumber].val += 4;
}

int write_string(int EnvNumber, token &t, vector < char > &bytecode, vector < change > &sizes)
{
    if (t.type != 's')
        return -1;
    int length = strlen(t.content);
    char *p = (char *)&length;
    bytecode.push_back(p[0]);
    bytecode.push_back(p[1]);
    bytecode.push_back(p[2]);
    bytecode.push_back(p[3]);
    sizes[EnvNumber].val += 4 + length;
    for (int j = 0; j < length; ++j)
        bytecode.push_back(t.content[j]);
    return 0;
}

int Compile(FILE *f, FILE *o, FILE *l, FILE *error)
{
//    if (argc != 3)
//        return -1;
//    FILE *f = fopen(argv[1], "r");
//    FILE *o = fopen(argv[2], "w");
//    FILE *l = fopen("log.txt", "w");
//convertation to tokens
    vector < token > code;
    convert_to_tokens(code, f);
    for (int i = 0; i < code.size(); ++i)
        fprintf(l, "token #%d\ntype: %c\ncontent: %s\n-------\n", i, code[i].type, code[i].content);
//begin

    TrBor commands;
    init_Bor(&commands);
    //int cmds = commands.count;

    TrBor env, *met = NULL, *var = NULL;
    vector < change > sizes, envs, *mets = NULL, *vars = NULL;
    vector < char > bytecode;
    //char* commands

    //syntaxis check
    int n = code.size();
    for (int i = 0; i < n; ++i)
    {
        while (code[i].type == 'e')
            ++i;
        if (i >= n)
            break;
        //read env
        TrString t;
        cstr_to_TrString(&t, code[i].content);
        ++i;
        //read name env
        cstr_to_TrString(&t, code[i].content);
        int EnvNumber = borgetnumber(&env, &t);
        change empty_change;
        if (envs.size() == EnvNumber)
        {//add him
            envs.push_back(empty_change);
            sizes.push_back(empty_change);
        }
        //init env
        ///
        envs[EnvNumber].val = bytecode.size();
        sizes[EnvNumber].val = 0;
        sizes[EnvNumber].shifts.push_back(bytecode.size());
        met = new TrBor();
        var = new TrBor();
        mets = new vector < change >();
        vars = new vector < change >();
        //char byte = 0;
        //push size bytes
        bytecode.push_back(0);
        bytecode.push_back(0);
        bytecode.push_back(0);
        bytecode.push_back(0);
        sizes[EnvNumber].val += 4;
        //skip space
        ++i;
        while (code[i].type == 'e')
            ++i;
        //read tokens in environment
        bool stop = false;
        bool not_command = false;
        while (!stop)
        {
            TrString current;
            TrString tmp;
            cstr_to_TrString(&current, code[i].content);
            int num = borgetnumber(&commands, &current);
            int ntmp, cn, cnt, paddr, eaddr, parts, elems, pcnt;
            char *p;
            switch(num)
            {
                case 0: //nop
                    bytecode.push_back(0);
                    sizes[EnvNumber].val += 1;
                    break;
                case 1: //arr
                case 2: //arradd
                case 3: //arrrem
                case 4: //bor
                case 5: //boradd
                case 6: //borrem
                    bytecode.push_back(num);
                    sizes[EnvNumber].val += 1;
                    ++i;//read name var
                    if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    //while not endline
                    ++i;
                    while (code[i].type != 'e')
                    {
                        if (code[i].type == 'c')
                        {//empty string
                            bytecode.push_back(0);//it is string
                            sizes[EnvNumber].val += 1;
                            //push zero size
                            write_void(EnvNumber, bytecode, sizes);
                            //++i
                        }
                        else if (code[i].type == 's')
                        {//string
                            bytecode.push_back(0);//it is string
                            sizes[EnvNumber].val += 1;
                            if (write_string(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'s\', but get \'%c\'.\n", i, code[i].type); return -1;}
                            //next
                            ++i;
                        } else
                        {//name
                            bytecode.push_back(1);//it is variable
                            sizes[EnvNumber].val += 1;
                            if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                            //next
                            ++i;
                        }
                        //check next
                        if (code[i].type == 'c')
                        {
                            //next
                            ++i;
                            //if next after comma is endline
                            if (code[i].type == 'e')
                            {
                                //empty string
                                bytecode.push_back(0);//it is string
                                sizes[EnvNumber].val += 1;
                                //push zero size
                                write_void(EnvNumber, bytecode, sizes);
                            }
                            //else cycle is repeating
                        }
                    }
                    break;
                //case for all comands, that need only var name.
                case 7: case 8:
                //borgts borgtn
                case 15: case 16: case 17: case 18: case 19:
                //end psh cln var pop
                case 23: case 24: case 25: case 26: case 27:
                //tstnil tstlss tstlrg tsteql tststk
                case 28: case 29: case 30: case 31: case 32: case 33: case 34:
                //tstisa tstiso tstisi tstisf tstiss tstisc tstisv
                case 36: case 37: case 38:
                //tstblw tstabv tstidt
                case 40: case 41: case 42:
                //mts fts stm
                case 44: case 45: case 46: case 47: case 50:
                //vctpshbck vctpshfrt vctpopbck vctpopfrt vctgsz
                case 52: case 53: case 54:
                //txtlen txtctn txtfnd
                case 60: case 63:
                //cls wrt
                case 65: case 66:
                //scnfltfil scnfltstr
                    //push code
                    if (num > 10)
                        bytecode.push_back(num - 2);
                    else
                        bytecode.push_back(num);
                    sizes[EnvNumber].val += 1;
                    //to vars
                    ++i;
                    while(code[i].type != 'e')
                    {
                        if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                        //next
                        ++i;
                    }
                    break;
                case 11: case 12:
                //run chg
                    bytecode.push_back(num - 1);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    //get env
                    cstr_to_TrString(&tmp, code[i].content);
                    ntmp = borgetnumber(&env, &tmp);
                    if (envs.size() == ntmp)
                    {//add him
                        envs.push_back(empty_change);
                        sizes.push_back(empty_change);
                    }
                    //write env
                    envs[ntmp].shifts.push_back(bytecode.size());
                    write_void(EnvNumber, bytecode, sizes);
                    break;
                case 22:
                //get
                    bytecode.push_back(num - 2);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    ++i;
                    if (write_int(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'i\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    break;
                case 21:
                //set
                    bytecode.push_back(num - 2);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    if (write_int(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'i\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    ++i;
                    if (write_int(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'i\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    break;
                case 35:
                //tstflg
                    bytecode.push_back(num - 2);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    if (write_int(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'i\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    break;
                case 43: case 51: case 61: case 62:
                //vct vctrsz nxt prv
                    bytecode.push_back(num - 2);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    //write var
                    if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    ++i;
                    //write numb
                    if (code[i].type == 'i')
                    {
                        bytecode.push_back(254);
                        sizes[EnvNumber].val += 1;
                        if (write_int(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'i\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    } else
                    {
                        bytecode.push_back(255);
                        sizes[EnvNumber].val += 1;
                        if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    }
                    break;
                case 49: case 64:
                //vctset scndef
                    bytecode.push_back(num - 2);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    ++i;
                    if (code[i].type == 'i')
                    {
                        bytecode.push_back(254);
                        sizes[EnvNumber].val += 1;
                        if (write_int(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'i\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    } else
                    {
                        bytecode.push_back(255);
                        sizes[EnvNumber].val += 1;
                        if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    }
                    ++i;
                    if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    break;
                case 48: case 55: case 57:
                //vctget tstspl prt
                    bytecode.push_back(num - 2);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    ++i;
                    if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    ++i;
                    if (code[i].type == 'i')
                    {
                        bytecode.push_back(254);
                        sizes[EnvNumber].val += 1;
                        if (write_int(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'i\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    } else
                    {
                        bytecode.push_back(255);
                        sizes[EnvNumber].val += 1;
                        if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    }
                    break;
                case 39: case 58: case 59:
                //str opnrdf opnwrf
                    bytecode.push_back(num - 2);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    ++i;
                    if (code[i].type == 's')
                    {//string
                        bytecode.push_back(0);//it is string
                        sizes[EnvNumber].val += 1;
                        if (write_string(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'s\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    } else
                    {//name
                        bytecode.push_back(1);//it is variable
                        sizes[EnvNumber].val += 1;
                        if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    }
                    break;
                case 56:
                //txtdel
                    bytecode.push_back(num - 2);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    ++i;
                    if (code[i].type == 'i')
                    {
                        bytecode.push_back(254);
                        sizes[EnvNumber].val += 1;
                        if (write_int(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'i\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    } else
                    {
                        bytecode.push_back(255);
                        sizes[EnvNumber].val += 1;
                        if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    }
                    ++i;
                    if (code[i].type == 'i')
                    {
                        bytecode.push_back(254);
                        sizes[EnvNumber].val += 1;
                        if (write_int(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'i\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    } else
                    {
                        bytecode.push_back(255);
                        sizes[EnvNumber].val += 1;
                        if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    }
                    break;
                case 13:
                //met
                    not_command = true;
                    ++i;
                    cstr_to_TrString(&tmp, code[i].content);
                    ntmp = borgetnumber(met, &tmp);
                    if ((*mets).size() == ntmp)
                        (*mets).push_back(empty_change);
                    (*mets)[ntmp].val = sizes[EnvNumber].val;
                    break;
                case 14:
                //jmp
                    bytecode.push_back(num - 2);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    cstr_to_TrString(&tmp, code[i].content);
                    ntmp = borgetnumber(met, &tmp);
                    if ((*mets).size() == ntmp)
                        (*mets).push_back(empty_change);
                    (*mets)[ntmp].shifts.push_back(bytecode.size());
                    write_void(EnvNumber, bytecode, sizes);
                    break;
                case 9:
                //obj
                    bytecode.push_back(num);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    ++i;
                    //part counter
                    parts = 0;
                    paddr = bytecode.size();//it adress
                    write_void(EnvNumber, bytecode, sizes);//it space
                    while (code[i].type != 'e')
                    {
                        parts += 1;//increment parts
                        elems = 0;//elements counter
                        eaddr = bytecode.size();//it adress
                        write_void(EnvNumber, bytecode, sizes);//it space
                        while (code[i].type == 'i')//while we have elements
                        {
                            elems += 1;//increment elements counter
                            //write element
                            if (write_int(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'i\', but get \'%c\'.\n", i, code[i].type); return -1;}
                            ++i;
                            if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                            ++i;
                        }
                        //write elements counter
                        p = (char *)&elems;
                        bytecode[eaddr] = p[0]; bytecode[eaddr + 1] = p[1];
                        bytecode[eaddr + 2] = p[2]; bytecode[eaddr + 3] = p[3];
                        if (code[i].type == 'c')
                            ++i;
                    }
                    p = (char *)&parts;
                    bytecode[paddr] = p[0]; bytecode[paddr + 1] = p[1];
                    bytecode[paddr + 2] = p[2]; bytecode[paddr + 3] = p[3];
                    break;
                case 20:
                //mth
                    bytecode.push_back(num - 2);
                    sizes[EnvNumber].val += 1;
                    ++i;
                    if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                    ++i;
                    cnt = 0;
                    pcnt = bytecode.size();//it adress
                    write_void(EnvNumber, bytecode, sizes);//it space
                    while (code[i].type != 'e')
                    {
                        cnt += 1;
                        if (code[i].type == 'n')
                        {
                            bytecode.push_back(255);
                            sizes[EnvNumber].val += 1;
                            if (write_variable(EnvNumber, code[i], bytecode, sizes, var, vars)){fprintf(error, "error in token #%d\nNeed \'n\', but get \'%c\'.\n", i, code[i].type); return -1;}
                        } else if (code[i].type == 'i')
                        {
                            bytecode.push_back(254);
                            sizes[EnvNumber].val += 1;
                            //write Big int
                            TrMath x;
                            cstr_to_TrMath(&x, code[i].content);
                            cn = x.len;
                            p = (char *)&cn;
                            //size
                            bytecode.push_back(p[0]);
                            bytecode.push_back(p[1]);
                            bytecode.push_back(p[2]);
                            bytecode.push_back(p[3]);
                            sizes[EnvNumber].val += 4;
                            //sign
                            bytecode.push_back(x.sign);
                            sizes[EnvNumber].val += 1;
                            //digits
                            for (int k = 0; k < x.len; ++k)
                            {
                                cn = x.digits[k];
                                bytecode.push_back(p[0]);
                                bytecode.push_back(p[1]);
                                bytecode.push_back(p[2]);
                                bytecode.push_back(p[3]);
                                sizes[EnvNumber].val += 4;
                            }
                            //if (write_int(EnvNumber, code[i], bytecode, sizes)){fprintf(error, "error in token #%d\nNeed \'i\', but get \'%c\'.\n", i, code[i].type); return -1;}
                        } else
                        {
                            bytecode.push_back(code[i].content[0]);
                            sizes[EnvNumber].val += 1;
                        }
                        ++i;
                    }
                    p = (char *)&cnt;
                    bytecode[pcnt] = p[0]; bytecode[pcnt + 1] = p[1];
                    bytecode[pcnt + 2] = p[2]; bytecode[pcnt + 3] = p[3];
                    break;
                case 67:
                    //endenv
                    stop = true;
                    break;
                default :
                    fprintf(error, "unexpected command %s", code[i].content);
                    return -1;
                    break;
            }
            if (!stop)
            {
                if (!not_command)
                {
                    bytecode.push_back(253);
                    sizes[EnvNumber].val += 1;
                }
                not_command = false;
                ++i;
                while (code[i].type == 'e')
                    ++i;
            }
        }
        //end of environment
        //write variables, mets.
        int x;
        char *p = (char *)&x;
        for (int k = 0; k < (*mets).size(); ++k)
        {
            x = (*mets)[k].val;
            for (int j = 0; j < (*mets)[k].shifts.size(); ++j)
            {
                int xaddr = (*mets)[k].shifts[j];
                bytecode[xaddr] = p[0]; bytecode[xaddr + 1] = p[1];
                bytecode[xaddr + 2] = p[2]; bytecode[xaddr + 3] = p[3];
            }
        }
        x = var->count;
        bytecode.push_back(p[0]);
        bytecode.push_back(p[1]);
        bytecode.push_back(p[2]);
        bytecode.push_back(p[3]);
        sizes[EnvNumber].val += 4;
        for (int k = 0; k < (*vars).size(); ++k)
        {
            x = (*vars)[k].val + sizes[EnvNumber].val;
            for (int j = 0; j < (*vars)[k].shifts.size(); ++j)
            {
                int xaddr = (*vars)[k].shifts[j];
                bytecode[xaddr] = p[0]; bytecode[xaddr + 1] = p[1];
                bytecode[xaddr + 2] = p[2]; bytecode[xaddr + 3] = p[3];
            }
        }
        //free resources
        delete met;
        delete mets;
        delete var;
        delete vars;
        while (code[i].type == 'e')
            ++i;
    }
    //write sizes and envs
    int x;
    char *p = (char *)&x;
    for (int k = 0; k < sizes.size(); ++k)
    {
        x = sizes[k].val;
        for (int j = 0; j < sizes[k].shifts.size(); ++j)
        {
            int xaddr = sizes[k].shifts[j];
            bytecode[xaddr] = p[0]; bytecode[xaddr + 1] = p[1];
            bytecode[xaddr + 2] = p[2]; bytecode[xaddr + 3] = p[3];
        }
    }
    for (int k = 0; k < envs.size(); ++k)
    {
        x = envs[k].val;
        for (int j = 0; j < envs[k].shifts.size(); ++j)
        {
            int xaddr = envs[k].shifts[j];
            bytecode[xaddr] = p[0]; bytecode[xaddr + 1] = p[1];
            bytecode[xaddr + 2] = p[2]; bytecode[xaddr + 3] = p[3];
        }
    }
    //write in o-file
    for (int k = 0; k < bytecode.size(); ++k)
        fputc(bytecode[k], o);
    return 0;
}
