typedef int (*skipfunctions)(TrMachine *M, char *code);

int _nop(TrMachine *M, char *code)
{
    M->executes[M->ptr].ptr += 2;
    return 0;
}

int _arr(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
        } else
        {
            ++i;
            i += 4;
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _arradd(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
        } else
        {
            ++i;
            i += 4;
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _arrrem(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
        } else
        {
            ++i;
            i += 4;
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _bor(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
        } else
        {
            ++i;
            i += 4;
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _boradd(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
        } else
        {
            ++i;
            i += 4;
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _borrem(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
        } else
        {
            ++i;
            i += 4;
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _borgtn(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _borgts(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _obj(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    int parts;
    read_int(&parts, &code[i]);
    i += 4;
    for (int j = 0; j < parts; ++j)
    {
        int size;
        read_int(&size, &code[i]);
        i += 4;
        for (int k = 0; k < size; ++k)
        {
            i += 4;
            i += 4;
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _run(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _chg(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _jmp(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _end(TrMachine *M, char *code)
{
    int i = 1;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _psh(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _cln(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _var(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _pop(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _mth(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    int count;
    int x;
    read_int(&count, &code[i]);
    i += 4;
    for (int j = 0; j < count; ++j)
    {
        switch(code[i])
        {
        case -1:
            ++i;
            i += 4;
            break;
        case -2:
            ++i;
            //read_big_int()
            i += 4;
            ++i;
            for (int j = 0; j < x; ++j)
            {
                i += 4;
            }
            break;
        case '+':
        case '-':
        case '/':
        case '%':
        case '*':
            //++i;
            ++i;
            break;
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _set(TrMachine *M, char *code)
{
    int  i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _get(TrMachine *M, char *code)
{
    int  i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _tsts(TrMachine *M, char *code)
{
    int i = 0;
    if (code[i] == 21)
    {//tstnil
        ++i;
        i += 4;
    } else if (code[i] == 25)
    {//tststk
        ++i;
    } else if (code[i] >= 26 && code[i] <= 32)
    {//tstis*
        ++i;
        i += 4;
    } else if (code[i] >= 22 && code[i] <= 24)
    {//tst mth
        ++i;
        i += 4;
        i += 4;
    } else if (code[i] >= 33 && code[i] <= 35)
    {//tst str
        ++i;
        i += 4;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _str(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    if (code[i] == 0)
    {
        ++i;
        TrString *sstr = new TrString();
        read_str(sstr, &code[i]);
        i += 4 + sstr->len;
    } else
    {
        ++i;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _mts(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _fts(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _stm(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _vct(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    if (code[i] == -1)
    {
        ++i;
        i += 4;
    } else
    {
        ++i;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _vctpshbck(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _vctpshfrt(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _vctpopbck(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _vctpopfrt(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _vctset(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    if (code[i] == -1)
    {
        ++i;
        i += 4;
    } else
    {
        ++i;
        i += 4;
    }
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _vctget(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    if (code[i] == -1)
    {
        ++i;
        i += 4;
    }
     else
    {
        ++i;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _vctrsz(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    if (code[i] == -1)
    {
        ++i;
        i += 4;
    } else
    {
        ++i;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _vctgsz(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _txtlen(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _txtctn(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _txtfnd(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _txtspl(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    if (code[i] == -1)
    {
        ++i;
        i += 4;
    } else
    {
        ++i;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _txtdel(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    if (code[i] == -1)
    {
        ++i;
        i += 4;
    } else
    {
        ++i;
        i += 4;
    }
    if (code[i] == -1)
    {
        ++i;
        i += 4;
    } else
    {
        ++i;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _prt(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    if (code[i] == -1)
    {
        ++i;
        i += 4;
    } else
    {
        ++i;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _opnwrf(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    if (code[i] == 0)
    {
        ++i;
        TrString str;
        read_str(&str, &code[i]);
        i += 4 + str.len;
    } else
    {
        ++i;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _opnrdf(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    if (code[i] == 0)
    {
        ++i;
        TrString str;
        read_str(&str, &code[i]);
        i += 4 + str.len;
    } else
    {
        ++i;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _cls(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _nxt(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    if (code[i] == -1)
    {
        ++i;
        i += 4;
    } else
    {
        ++i;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _prv(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    if (code[i] == -1)
    {
        ++i;
        i += 4;
    } else
    {
        ++i;
        i += 4;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _wrt(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _scndef(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    if (code[i] == -1)
    {
        ++i;
        i += 4;
    } else
    {
        ++i;
        i += 4;
    }
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}
/*scaning*/
int _scnfltstr(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int _scnfltfil(TrMachine *M, char *code)
{
    int i = 1;
    i += 4;
    i += 4;
    i += 4;
    M->executes[M->ptr].ptr += i;
    return 0;
}

skipfunctions skip_functions[] = {_nop,//0
    _arr, _arradd,_arrrem,//3
    _bor, _boradd,_borrem,//6
    _borgts,_borgtn,_obj,//9
    _run,//11
    _chg,_jmp,//14
    _end,_psh,_cln,//17
    _var,_pop,_mth,//20
    _set,_get,_tsts,//23
    _tsts,_tsts,_tsts,//26
    _tsts,_tsts,_tsts,//29
    _tsts,_tsts,_tsts,//32
    _tsts,_tsts,_tsts,//35
    _tsts,_tsts,_tsts,//38
    _str,_mts,_fts,//41
    _stm,_vct,_vctpshbck,//44
    _vctpshfrt,_vctpopbck,_vctpopfrt,//47
    _vctget,_vctset,_vctgsz,//50
    _vctrsz,_txtlen,_txtctn,//53
    _txtfnd,_txtspl,_txtdel,//56
    _prt,_opnrdf,_opnwrf,//59
    _cls,_nxt,_prv,//62
    _wrt,_scndef,_scnfltfil,//65
    _scnfltstr};//66
