
/*struct TrMFile
{
    TrString data;
    int ptr;
    TrVar varfile;
    ~TrMFile()
    {
        varclear(&varfile);
    }
};*/

struct TrMExecute
{
    char *source;
    int ptr;
    TrMExecute()
    {
        source = 0;
        ptr = -1;
    }
    ~TrMExecute()
    {
        //if (source != 0)
        //    delete[] source;
    }
};

struct TrMachine
{
    FILE *error;
    FILE *errenv;
    bool skip;
    TrVector *stack;
    int Flags[100];
    char *source;
    vector < TrMExecute > executes;
    int ptr;
    TrMachine()
    {
        error = 0;
        source = 0;
        stack = 0;
        errenv = 0;

        skip = false;
    }
    void clear()
    {
        if (source != 0)
            delete[] source;
//        if (filenames != 0)
//            delete filenames;
//        if (files != 0)
//            delete[] files;
        if (error)
        {
            fclose(error);
            error = 0;
        }
        if (errenv)
        {
            fclose(errenv);
            error = 0;
        }
        if (stack != 0)
            delete stack;
    }
    ~TrMachine()
    {
        this->clear();
    }
};

void read_int(int *x, char *code)
{
    char* p = (char*)x;
    p[0] = code[0];
    p[1] = code[1];
    p[2] = code[2];
    p[3] = code[3];
}

void read_str(TrString *str, char *code)
{
    //str->Dest();
    int len;
    read_int(&len, code);
    char *s = new char[len + 1];
    memcpy(s, &code[4], len);
    s[len] = 0;
    cstr_to_TrString(str, s);
    delete[] s;
}

int read_var(TrMachine *M, TrVar **var, char *code, char type = '?')
{

    int shift;
    //читаем смещение по которому находится переменная
    read_int(&shift, code);
    //смещаемся и присваеваем переменную
    code = &(M->executes[M->ptr].source[0]);
    *var = (TrVar*)&code[shift];
    //проверка типов
    if (type == '?')
        return 0;
    if ((*var)->shell != 0)
        if ((*var)->shell->type == type)
            return 0;
        else//ошибка несовпадния типов
            fprintf(M->error, "Error in byte #%d: need \'%c\', but have \'%c\'", M->executes[M->ptr].ptr, type, (*var)->shell->type);
    else//ошибка нулевой переменной
        fprintf(M->error, "Error in byte #%d: NULL variable. Impossible to check type.", M->executes[M->ptr].ptr);
    //записываем целиком код environment'а
    //используя размер энв узнаём колличество переменных
    int size;
    read_int(&size, M->executes[M->ptr].source);
    char* info = &code[size - 4];
    int varcnt;
    read_int(&varcnt, info);
    //суммируем
    size += varcnt * 4;
    //пишем
    for (int i = 0; i < size; ++i)
        fputc(code[i], M->errenv);
    return -1;
}

void read_big_int(TrMath &m, char *code)
{
    m.Init();
    read_int(&m.len, code);
    m.sign = code[4];
    for (int i = 0; i < m.len; ++i)
    {
        int x;
        read_int(&x, &code[4 * i + 4]);
        m.digits[i] = x;
    }
}

void init_TrMExecute(TrMExecute *E, char *code)
{
    int size;
    read_int(&size, code);
    char* info = &code[size - 4];
    int varcnt;
    read_int(&varcnt, info);
    E->source = new char[size + varcnt * 4];
    E->ptr = 4;
    memcpy(E->source, code, size);
    for (int i = size; i < size + varcnt * 4; ++i)
        E->source[i] = 0;
}

void dest_TrMExecute(TrMExecute *E)
{
    delete[] E->source;
    E->source = 0;
}

int nop(TrMachine *M, char *code)
{
    M->executes[M->ptr].ptr += 2;
    return 0;
}

int arr(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *Arr;
    if (read_var(M, &Arr, &code[i])){return -1;}
    i += 4;
    varclear(Arr);
    varcreate(Arr, 'a');
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
            arrgetnumber(((TrArr*)(Arr->shell->value)), &str);
        } else
        {
            ++i;
            TrVar *var;
            if (read_var(M, &var, &code[i], 's')){return -1;}
            i += 4;
            TrString *str = (TrString*)(var->shell->value);
            arrgetnumber(((TrArr*)(Arr->shell->value)), str);
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int arradd(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *Arr;
    if (read_var(M, &Arr, &code[i], 'a')){return -1;}
    i += 4;
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
            arrgetnumber(((TrArr*)(Arr->shell->value)), &str);
        } else
        {
            ++i;
            TrVar *var;
            if (read_var(M, &var, &code[i], 's')){return -1;}
            i += 4;
            TrString *str = (TrString*)(var->shell->value);
            arrgetnumber(((TrArr*)(Arr->shell->value)), str);
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int arrrem(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *Arr;
    if (read_var(M, &Arr, &code[i], 'a')){return -1;}
    i += 4;
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
            int n = arrgetnumber(((TrArr*)(Arr->shell->value)), &str);
            arrremove(((TrArr*)(Arr->shell->value)), n);
        } else
        {
            ++i;
            TrVar *var;
            if (read_var(M, &var, &code[i], 's')){return -1;}
            i += 4;
            TrString *str = (TrString*)(var->shell->value);
            int n = arrgetnumber(((TrArr*)(Arr->shell->value)), str);
            arrremove(((TrArr*)(Arr->shell->value)), n);
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int bor(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *Bor;
    if (read_var(M, &Bor, &code[i])){return -1;}
    i += 4;
    varclear(Bor);
    varcreate(Bor, 'b');
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
            borgetnumber(((TrBor*)(Bor->shell->value)), &str);
        } else
        {
            ++i;
            TrVar *var;
            if (read_var(M, &var, &code[i], 's')){return -1;}
            i += 4;
            TrString *str = (TrString*)(var->shell->value);
            borgetnumber(((TrBor*)(Bor->shell->value)), str);
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int boradd(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *Bor;
    if (read_var(M, &Bor, &code[i], 'b')){return -1;}
    i += 4;
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
            borgetnumber(((TrBor*)(Bor->shell->value)), &str);
        } else
        {
            ++i;
            TrVar *var;
            if (read_var(M, &var, &code[i], 's')){return -1;}
            i += 4;
            TrString *str = (TrString*)(var->shell->value);
            borgetnumber(((TrBor*)(Bor->shell->value)), str);
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int borrem(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *Bor;
    if (read_var(M, &Bor, &code[i], 'b')){return -1;}
    i += 4;
    while (code[i] != -3)
    {
        if (code[i] == 0)
        {
            ++i;
            TrString str;
            read_str(&str, &code[i]);
            i += 4 + str.len;
            int n = borgetnumber(((TrBor*)(Bor->shell->value)), &str);
            borremove(((TrBor*)(Bor->shell->value)), n);
        } else
        {
            ++i;
            TrVar *var;
            if (read_var(M, &var, &code[i], '?')){return -1;}
            if (var->shell == 0)
            {
                return -1;
                fprintf(M->error, "Error in bor: null variable");
            }
            i += 4;
            if (var->shell->type == 's')
            {
                TrString *str = (TrString*)(var->shell->value);
                int n = borgetnumber(((TrBor*)(Bor->shell->value)), str);
                borremove(((TrBor*)(Bor->shell->value)), n);
            } else if (var->shell->type == 'i')
            {
                int n = ((TrMath*)(var->shell->value))->digits[0] * ((TrMath*)(var->shell->value))->sign;
                borremove(((TrBor*)(Bor->shell->value)), n);
            } else
            {
                fprintf(M->error, "Error in bor: variable type if %c", var->shell->type);
                return -1;
            }
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int borgtn(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *Bor;
    if (read_var(M, &Bor, &code[i], 'b')){return -1;}
    i += 4;
    TrVar *num, *str;
    if (read_var(M, &num, &code[i], '?')){return -1;}
    i += 4;
    varclear(num);
    varcreate(num, 'i');
    if (read_var(M, &str, &code[i], 's')){return -1;}
    i += 4;
    int n = borgetnumber(((TrBor*)(Bor->shell->value)), ((TrString*)(str->shell->value)));
    ((TrMath*)(num->shell->value))->digits[0] = n;
    ((TrMath*)(num->shell->value))->len = 1;
    ((TrMath*)(num->shell->value))->sign = 1;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int borgts(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *Bor;
    if (read_var(M, &Bor, &code[i], 'b')){return -1;}
    i += 4;
    TrVar *num, *str;
    if (read_var(M, &str, &code[i], '?')){return -1;}
    i += 4;
    varclear(str);
    //varcreate(num, 's');
    if (read_var(M, &num, &code[i], 'i')){return -1;}
    i += 4;
    varinit(str, 's', borgetstring(((TrBor*)(Bor->shell->value)), ((TrMath*)(num->shell->value))->sign * ((TrMath*)(num->shell->value))->digits[0]));
    M->executes[M->ptr].ptr += i;
    return 0;
}

int obj(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *Obj;
    if (read_var(M, &Obj, &code[i], '?')){return -1;}
    varclear(Obj);
    varcreate(Obj, 'o');
    i += 4;
    int parts;
    read_int(&parts, &code[i]);
    i += 4;
    ((TrObject*)(Obj->shell->value))->parts = parts;
    ((TrObject*)(Obj->shell->value))->sizes = new int[parts];
    ((TrObject*)(Obj->shell->value))->types = new TrFragment*[parts];
    for (int j = 0; j < parts; ++j)
    {
        int size;
        read_int(&size, &code[i]);
        i += 4;
        ((TrObject*)(Obj->shell->value))->sizes[j] = size;
        ((TrObject*)(Obj->shell->value))->types[j] = new TrFragment[size];
        for (int k = 0; k < size; ++k)
        {
            int maxcnt;
            read_int(&maxcnt, &code[i]);
            i += 4;
            TrVar *var;
            if (read_var(M, &var, &code[i], '?')){return -1;}
            i += 4;
            if (var->shell->type == 'a' || var->shell->type == 'o')
            {
                ((TrObject*)(Obj->shell->value))->types[j][k].maxcount = maxcnt;
                ++(var->shell->counter);
                ((TrObject*)(Obj->shell->value))->types[j][k].type.shell = var->shell;
            }
            else
            {
                fprintf(M->error, "Error when create object: it\'s not a 'a' and 'o'");
                return -1;
            }
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int run(TrMachine *M, char *code)
{
    int i = 1;
    int addr;
    read_int(&addr, &code[i]);
    i += 4;
    M->executes[M->ptr].ptr += i;
    TrMExecute empty;
    M->executes.push_back(empty);
    ++(M->ptr);
    init_TrMExecute(&M->executes[M->ptr], &M->source[addr]);
    return 0;
}

int chg(TrMachine *M, char *code)
{
    int addr;
    read_int(&addr, &code[1]);

    TrMExecute *E = &M->executes[M->ptr];
    int n;
    read_int(&n, &E->source[0]);
    int k;
    read_int(&k, &E->source[n - 4]);
    for (int j = 0; j < k; ++j)
    {
        varclear((TrVar*)(&E->source[n + j * 4]));
    }
    dest_TrMExecute(&M->executes[M->ptr]);
    M->executes.pop_back();

    TrMExecute empty;
    M->executes.push_back(empty);
    //++(M->ptr);
    init_TrMExecute(&M->executes[M->ptr], &M->source[addr]);
    return 0;
}

int jmp(TrMachine *M, char *code)
{
    int addr;
    read_int(&addr, &code[1]);
    M->executes[M->ptr].ptr = addr;
    return 0;
}

int end(TrMachine *M, char *code)
{
    TrMExecute *E = &M->executes[M->ptr];
    //TrMExecute *Q = &M->executes[M->ptr - 1];
    int n;
    read_int(&n, &E->source[0]);
    int k;
    read_int(&k, &E->source[n - 4]);
    for (int j = 0; j < k; ++j)
    {
        varclear((TrVar*)(&E->source[n + j * 4]));
    }
    dest_TrMExecute(&M->executes[M->ptr]);
    M->executes.pop_back();
    --(M->ptr);
    return 0;
}

int psh(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    TrVar x;
    if (var->shell == 0)
    {
        fprintf(M->error, "Error int psh: null variable!");
        return -1;
    }
    varassign(&x, var);
    vctpushback(M->stack, var);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int cln(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    TrVar *src;
    if (read_var(M, &src, &code[i], '?')){return -1;}
    i += 4;
    if (src->shell == 0)
    {
        fprintf(M->error, "Error int cln: null variable!");
        return -1;
    }
    varclear(var);
    typclone(var, src);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int var(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    TrVar *src;
    if (read_var(M, &src, &code[i], '?')){return -1;}
    i += 4;
    if (src->shell == 0)
    {
        fprintf(M->error, "Error int cln: null variable!");
        return -1;
    }
    varclear(var);
    varassign(var, src);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int pop(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    TrVar *back = vctpopback(M->stack);
    varclear(var);
    varassign(var, back);
    varclear(back);
    delete back;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int mth(TrMachine *M, char *code)
{
    vector < TrMath* > calcstack;
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    TrVar *mat;
    TrMath *a, *b, *c, *d;
    int x, y;
    int count;
    read_int(&count, &code[i]);
    i += 4;
    for (int j = 0; j < count; ++j)
    {
        switch(code[i])
        {
        case -1:
            ++i;
            if (read_var(M, &mat, &code[i], 'i')){return -1;}
            i += 4;
            a = mthclone((TrMath*)(mat->shell->value));
            calcstack.push_back(a);
            break;
        case -2:
            ++i;
            //read_big_int()
            a = new TrMath();
            read_int(&x, &code[i]);
            i += 4;
            a->len = x;
            a->sign = code[i];
            ++i;
            for (int j = 0; j < x; ++j)
            {
                read_int(&y, &code[i]);
                i += 4;
                a->digits[j] = y;
            }
            calcstack.push_back(a);
            break;
        case '+':
        case '-':
        case '/':
        case '%':
        case '*':
            //++i;
            b = calcstack[calcstack.size() - 1];
            calcstack.pop_back();
            a = calcstack[calcstack.size() - 1];
            calcstack.pop_back();
            c = new TrMath();
            if (code[i] == '+')
            {
                mthalgebricsum(a, b, c);
            } else if (code[i] == '-')
            {
                b->sign *= -1;
                mthalgebricsum(a, b, c);
            } else if (code[i] == '*')
            {
                mthmultiplication(a, b, c);
            } else if (code[i] == '/')
            {
                d = new TrMath();
                mthdivision(a, b, c, d);
                delete d;
            } else if (code[i] == '%')
            {
                d = new TrMath();
                mthdivision(a, b, d, c);
                delete d;
            }
            calcstack.push_back(c);
            delete a;
            delete b;
            ++i;
            break;
        }
    }
    varinit(var, 'i', calcstack[0]);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int set(TrMachine *M, char *code)
{
    int  i = 1;
    int fl, vl;
    read_int(&fl, &code[i]);
    i += 4;
    read_int(&vl, &code[i]);
    i += 4;
    M->Flags[fl] = vl;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int sign(int x)
{
    if (x < 0)
        return -1;
    if (x > 0)
        return 1;
    return 0;
}

int get(TrMachine *M, char *code)
{
    int  i = 1;
    int fl;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    read_int(&fl, &code[i]);
    i += 4;
    fl = M->Flags[fl];
    TrMath *mat = new TrMath();
    mat->sign = sign(fl);
    fl *= mat->sign;
    mat->len = 1;
    mat->digits[0] = fl;
    varinit(var, 'i', mat);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int tsts(TrMachine *M, char *code)
{
    int i = 0;
    if (code[i] == 21)
    {//tstnil
        ++i;
        TrVar *var;
        if (read_var(M, &var, &code[i], '?')){return -1;}
        i += 4;
        if (var->shell != 0)
            M->skip = true;
    } else if (code[i] == 25)
    {//tststk
        ++i;
        if (vctsize(M->stack) == 0)
            M->skip = true;
    } else if (code[i] >= 26 && code[i] <= 32)
    {//tstis*
        char type;
        switch(code[i])
        {
        case 26:
            type = 'a';
            break;
        case 27:
            type = 'o';
            break;
        case 28:
            type = 'i';
            break;
        case 29:
            type = 'f';
            break;
        case 30:
            type = 's';
            break;
        case 31:
            type = 'c';
            break;
        case 32:
            type = 'v';
            break;
        }
        ++i;
        TrVar *var;
        if (read_var(M, &var, &code[i], '?')){return -1;}
        i += 4;
        if (var->shell == 0)
        {
            fprintf(M->error, "Error: nll reference in test of type.");
            return -1;
        }
        if (var->shell->type != type)
            M->skip = true;
    } else if (code[i] >= 22 && code[i] <= 24)
    {//tst mth
        char x = code[i];
        ++i;
        TrVar *a, *b;
        if (read_var(M, &a, &code[i], 'i')){return -1;}
        i += 4;
        if (read_var(M, &b, &code[i], 'i')){return -1;}
        i += 4;
        switch(x)
        {
        case 22:
            if (mthcmp((TrMath*)a->shell->value, (TrMath*)b->shell->value) == -1)// >=
                M->skip = true;
            break;
        case 23:
            if (mthcmp((TrMath*)a->shell->value, (TrMath*)b->shell->value) != 1)//>
                M->skip = true;
            break;
        case 24:
            if (mthcmp((TrMath*)a->shell->value, (TrMath*)b->shell->value) != 0)//==
                M->skip = true;
            break;
        }
    } else if (code[i] >= 33 && code[i] <= 35)
    {//tst str
        char x = code[i];
        ++i;
        TrVar *a, *b;
        if (read_var(M, &a, &code[i], 's')){return -1;}
        i += 4;
        if (read_var(M, &b, &code[i], 's')){return -1;}
        i += 4;
        switch(x)
        {
        case 33:
            if (strcompar((TrString*)a->shell->value, (TrString*)b->shell->value) == -1)//>=
                M->skip = true;
            break;
        case 34:
            if (strcompar((TrString*)a->shell->value, (TrString*)b->shell->value) != 1)//>
                M->skip = true;
            break;
        case 35:
            if (strcompar((TrString*)a->shell->value, (TrString*)b->shell->value) != 0)//==
                M->skip = true;
            break;
        }
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int str(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    if (code[i] == 0)
    {
        ++i;
        TrString *sstr = new TrString();
        read_str(sstr, &code[i]);
        i += 4 + sstr->len;
        varinit(var, 's', sstr);
    } else
    {
        ++i;
        TrVar *vstr;
        if (read_var(M, &vstr, &code[i], 's')){return -1;}
        i += 4;
        typclone(var, vstr);
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int mts(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    TrVar *mat;
    if (read_var(M, &mat, &code[i], 'i')){return -1;}
    i += 4;
    char *str;
    TrMath_to_cstr(&str, (TrMath*)mat->shell->value);
    varcreate(var, 's');
    cstr_to_TrString((TrString*)var->shell->value, str);
    delete[] str;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int fts(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    TrVar *frame;
    if (read_var(M, &frame, &code[i], 'f')){return -1;}
    i += 4;
    char *puff;
    TrFrame * fr = (TrFrame*)frame->shell->value;
    int l = 0;
    for (int j = 0; j < fr->parts; ++j)
        l += fr->segments[j].len;
    varcreate(var, 's');
    ((TrString*)var->shell->value)->str = new char[l];
    ((TrString*)var->shell->value)->len = l;
    puff = ((TrString*)var->shell->value)->str;
    l = 0;
    for (int j = 0; j < fr->parts; ++j)
    {
        memcpy(&puff[l] , fr->segments[j].str, fr->segments[j].len);
        l += fr->segments[j].len;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int stm(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    TrVar *mat;
    if (read_var(M, &mat, &code[i], 's')){return -1;}
    i += 4;
    char *tmp;
    TrString_to_cstr(&tmp, (TrString*)mat->shell->value);
    varcreate(var, 'i');
    cstr_to_TrMath((TrMath*)var->shell->value, tmp);
    delete[] tmp;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int vct(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    int l;
    if (code[i] == -1)
    {
        ++i;
        TrVar *mat;
        if (read_var(M, &mat, &code[i], 'i')){return -1;}
        i += 4;
        l = ((TrMath*)mat->shell->value)->digits[0];
    } else
    {
        ++i;
        read_int(&l, &code[i]);
        i += 4;
    }
    varclear(var);
    varcreate(var, 'v');
    vctresize((TrVector*)var->shell->value, l);
    ((TrVector*)var->shell->value)->f = ((TrVector*)var->shell->value)->count - 1;
    ((TrVector*)var->shell->value)->b = ((TrVector*)var->shell->value)->count - 2;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int vctpshbck(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'v')){return -1;}
    i += 4;
    TrVar *vl;
    if (read_var(M, &vl, &code[i], '?')){return -1;}
    i += 4;
    if (vl->shell == 0)
    {
        fprintf(M->error, "Error: null variable in vctpshbck");
    }
    TrVar x;
    varassign(&x, vl);
    vctpushback((TrVector*)var->shell->value, &x);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int vctpshfrt(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'v')){return -1;}
    i += 4;
    TrVar *vl;
    if (read_var(M, &vl, &code[i], '?')){return -1;}
    i += 4;
    if (vl->shell == 0)
    {
        fprintf(M->error, "Error: null variable in vctpshfrt");
    }
    TrVar x;
    varassign(&x, vl);
    vctpushfront((TrVector*)var->shell->value, &x);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int vctpopbck(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'v')){return -1;}
    i += 4;
    TrVar *vl;
    if (read_var(M, &vl, &code[i], '?')){return -1;}
    i += 4;
    varclear(vl);
    TrVar *x;
    x = vctpopback((TrVector*)var->shell->value);
    varassign(vl, x);
    varclear(x);
    delete x;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int vctpopfrt(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'v')){return -1;}
    i += 4;
    TrVar *vl;
    if (read_var(M, &vl, &code[i], '?')){return -1;}
    i += 4;
    varclear(vl);
    TrVar *x;
    x = vctpopfront((TrVector*)var->shell->value);
    varassign(vl, x);
    varclear(x);
    delete x;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int vctset(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'v')){return -1;}
    i += 4;
    int n;
    if (code[i] == -1)
    {
        ++i;
        TrVar *mat;
        if (read_var(M, &mat, &code[i], 'i')){return -1;}
        i += 4;
        n = ((TrMath*)mat->shell->value)->sign * ((TrMath*)mat->shell->value)->digits[0];
    } else
    {
        ++i;
        read_int(&n, &code[i]);
        i += 4;
    }
    TrVar *vl;
    if (read_var(M, &vl, &code[i], '?')){return -1;}
    i += 4;
    if (vl->shell == 0)
    {
        fprintf(M->error, "Error: null variable in vctpshfrt");
    }
    vctset((TrVector*)var->shell->value, n, vl);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int vctget(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'v')){return -1;}
    i += 4;
    TrVar *vl;
    if (read_var(M, &vl, &code[i], '?')){return -1;}
    i += 4;
    varclear(vl);
    int n;
    if (code[i] == -1)
    {
        ++i;
        TrVar *mat;
        if (read_var(M, &mat, &code[i], 'i')){return -1;}
        i += 4;
        n = ((TrMath*)mat->shell->value)->sign * ((TrMath*)mat->shell->value)->digits[0];
    }
     else
    {
        ++i;
        read_int(&n, &code[i]);
        i += 4;
    }
    TrVar *r = vctget((TrVector*)var->shell->value, n);
    varassign(vl, r);
    varclear(r);
    delete r;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int vctrsz(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'v')){return -1;}
    i += 4;
    int n;
    if (code[i] == -1)
    {
        ++i;
        TrVar *mat;
        if (read_var(M, &mat, &code[i], 'i')){return -1;}
        i += 4;
        n = ((TrMath*)mat->shell->value)->sign * ((TrMath*)mat->shell->value)->digits[0];
    } else
    {
        ++i;
        read_int(&n, &code[i]);
        i += 4;
    }
    vctresize((TrVector*)var->shell->value, n);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int vctgsz(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'v')){return -1;}
    i += 4;
    TrVar *vl;
    if (read_var(M, &vl, &code[i], '?')){return -1;}
    i += 4;
    varclear(vl);
    varcreate(vl, 'i');
    int sz = vctsize((TrVector*)var->shell->value);
    ((TrMath*)vl->shell->value)->len = 1;
    ((TrMath*)vl->shell->value)->sign = 1;
    ((TrMath*)vl->shell->value)->digits[0] = sz;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int txtlen(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    TrVar *vl;
    if (read_var(M, &vl, &code[i], 's')){return -1;}
    i += 4;
    varcreate(var, 'i');
    ((TrMath*)var->shell->value)->len = 1;
    ((TrMath*)var->shell->value)->sign = 1;
    ((TrMath*)var->shell->value)->digits[0] = ((TrString*)vl->shell->value)->len;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int txtctn(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    TrVar *a;
    if (read_var(M, &a, &code[i], 's')){return -1;}
    i += 4;
    TrVar *b;
    if (read_var(M, &b, &code[i], 's')){return -1;}
    i += 4;
    varcreate(var, 's');
    strconcatenation((TrString*)a->shell->value, (TrString*)b->shell->value, (TrString*)var->shell->value);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int txtfnd(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    TrVar *a;
    if (read_var(M, &a, &code[i], 's')){return -1;}
    i += 4;
    TrVar *b;
    if (read_var(M, &b, &code[i], 's')){return -1;}
    i += 4;
    int *mas;
    int n;
    strfind_substring_KMP((TrString*)a->shell->value, (TrString*)b->shell->value, &mas, &n);
    TrVar x;
    varcreate(var, 'v');
    for (int j = 0; j < n; ++j)
    {
        varcreate(&x, 'i');
        ((TrMath*)x.shell->value)->len = 1;
        ((TrMath*)x.shell->value)->sign = 1;
        ((TrMath*)x.shell->value)->digits[0] = mas[j];
        vctpushback((TrVector*)var->shell->value, &x);
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int txtspl(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    TrVar *a;
    if (read_var(M, &a, &code[i], 's')){return -1;}
    i += 4;
    int n;
    if (code[i] == -1)
    {
        ++i;
        TrVar *mat;
        if (read_var(M, &mat, &code[i], 'i')){return -1;}
        i += 4;
        n = ((TrMath*)mat->shell->value)->sign * ((TrMath*)mat->shell->value)->digits[0];
    } else
    {
        ++i;
        read_int(&n, &code[i]);
        i += 4;
    }
    varcreate(var, 'v');
    TrVar x, y;
    varcreate(&x, 's');
    varcreate(&y, 's');
    strsplit((TrString*)a->shell->value, (TrString*)x.shell->value, (TrString*)y.shell->value, n);
    vctpushback((TrVector*)var->shell->value, &x);
    vctpushback((TrVector*)var->shell->value, &y);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int txtdel(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 's')){return -1;}
    i += 4;
    int n;
    if (code[i] == -1)
    {
        ++i;
        TrVar *mat;
        if (read_var(M, &mat, &code[i], 'i')){return -1;}
        i += 4;
        n = ((TrMath*)mat->shell->value)->sign * ((TrMath*)mat->shell->value)->digits[0];
    } else
    {
        ++i;
        read_int(&n, &code[i]);
        i += 4;
    }
    int l;
    if (code[i] == -1)
    {
        ++i;
        TrVar *mat;
        if (read_var(M, &mat, &code[i], 'i')){return -1;}
        i += 4;
        l = ((TrMath*)mat->shell->value)->sign * ((TrMath*)mat->shell->value)->digits[0];
    } else
    {
        ++i;
        read_int(&l, &code[i]);
        i += 4;
    }
    strdelete((TrString*)var->shell->value, n, l);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int prt(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    TrVar *fr;
    if (read_var(M, &fr, &code[i], 'f')){return -1;}
    i += 4;
    int n;
    if (code[i] == -1)
    {
        ++i;
        TrVar *mat;
        if (read_var(M, &mat, &code[i], 'i')){return -1;}
        i += 4;
        n = ((TrMath*)mat->shell->value)->sign * ((TrMath*)mat->shell->value)->digits[0];
    } else
    {
        ++i;
        read_int(&n, &code[i]);
        i += 4;
    }
    TrString *xstr = new TrString();
    strassign(xstr, &(((TrFrame*)fr->shell->value)->segments[n]));
    varinit(var, 's', xstr);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int opnwrf(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    varcreate(var, 'c');
    if (code[i] == 0)
    {
        ++i;
        TrString str;
        read_str(&str, &code[i]);
        i += 4 + str.len;
        char *cstr;
        TrString_to_cstr(&cstr, &str);
        filopenwrite((TrFile*)var->shell->value, cstr);
    } else
    {
        ++i;
        TrVar *nf;
        if (read_var(M, &nf, &code[i], 's')){return -1;}
        i += 4;
        TrString *str = (TrString*)(nf->shell->value);
        char *cstr;
        TrString_to_cstr(&cstr, str);
        filopenwrite((TrFile*)var->shell->value, cstr);
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int opnrdf(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    varcreate(var, 'c');
    if (code[i] == 0)
    {
        ++i;
        TrString str;
        read_str(&str, &code[i]);
        i += 4 + str.len;
        char *cstr;
        TrString_to_cstr(&cstr, &str);
        filopenread((TrFile*)var->shell->value, cstr);
    } else
    {
        ++i;
        TrVar *nf;
        if (read_var(M, &nf, &code[i], 's')){return -1;}
        i += 4;
        TrString *str = (TrString*)(nf->shell->value);
        char *cstr;
        TrString_to_cstr(&cstr, str);
        filopenread((TrFile*)var->shell->value, cstr);
        delete cstr;
    }
    M->executes[M->ptr].ptr += i;
    return 0;
}

int cls(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'c')){return -1;}
    i += 4;
    filclose((TrFile*)var->shell->value);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int nxt(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'c')){return -1;}
    i += 4;
    int n;
    if (code[i] == -1)
    {
        ++i;
        TrVar *mat;
        if (read_var(M, &mat, &code[i], 'i')){return -1;}
        i += 4;
        n = ((TrMath*)mat->shell->value)->sign * ((TrMath*)mat->shell->value)->digits[0];
    } else
    {
        ++i;
        read_int(&n, &code[i]);
        i += 4;
    }
    filshift((TrFile*)var->shell->value, n);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int prv(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'c')){return -1;}
    i += 4;
    int n;
    if (code[i] == -1)
    {
        ++i;
        TrVar *mat;
        if (read_var(M, &mat, &code[i], 'i')){return -1;}
        i += 4;
        n = ((TrMath*)mat->shell->value)->sign * ((TrMath*)mat->shell->value)->digits[0];
    } else
    {
        ++i;
        read_int(&n, &code[i]);
        i += 4;
    }
    filshift((TrFile*)var->shell->value, -n);
    M->executes[M->ptr].ptr += i;
    return 0;
}

int wrt(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], 'c')){return -1;}
    i += 4;
    TrVar *vl;
    if (read_var(M, &vl, &code[i], 's')){return -1;}
    i += 4;
    char *cstr;
    TrString_to_cstr(&cstr, (TrString*)vl->shell->value);
    filwritecstr((TrFile*)var->shell->value, cstr);
    delete[] cstr;
    M->executes[M->ptr].ptr += i;
    return 0;
}

int scndef(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    int n;
    if (code[i] == -1)
    {
        ++i;
        TrVar *mat;
        if (read_var(M, &mat, &code[i], 'i')){return -1;}
        i += 4;
        n = ((TrMath*)mat->shell->value)->sign * ((TrMath*)mat->shell->value)->digits[0];
    } else
    {
        ++i;
        read_int(&n, &code[i]);
        i += 4;
    }
    TrVar *fl;
    if (read_var(M, &fl, &code[i], 'c')){return -1;}
    i += 4;
    TrFile *f = (TrFile*)fl->shell->value;
    vector < char > newstr;
    for(int j = 0; j < n && !fileof(f); ++j)
        newstr.push_back(filreadchar(f));
    char *str = new char[newstr.size()];
    for(int j = 0; j < newstr.size(); ++j)
        str[j] = newstr[j];
    TrString* res = new TrString();
    res->str = str;
    res->len = newstr.size();
    varinit(var, 's', res);
    M->executes[M->ptr].ptr += i;
    return 0;
}
/*scaning*/

int scnarray(char **result, TrArr *array, char *str, int strlen, int &ptr)
{
    if (array->count == 0)//нулевой аррай
    {
        *result = new char[1];
        **result = 0;
        ptr = 0;
        return 0;
    }
    if (strlen == 0)
        return -1;//ненулевой аррай и ноль доступных символов
    int min = array->count + 1;
    TAnode *panode;
    BTnode *p = array->head->head;
    int i = 0;
    do
    {
        char x = str[i];
        while (p != 0)
        {
            if (p->sym == x)
                break;
            else if (p->sym > x)
                p = p->left;
            else if (p->sym < x)
                p = p->right;
        }
        if (!p)
            break;
        panode = p->val;
        p = panode->descendants->head;
        if (panode->key >= 0)
            if (panode->key < min)
                min = panode->key;
        ++i;
    } while (panode->minimalkey < min && i < strlen);
    if (min < array->count)
    {
        TrString *stroka = arrgetstring(array, min);
        TrString_to_cstr(result, stroka);
        ptr = stroka->len;
        delete stroka;
        return 0;
    }
    return -1;//Не нашли то, что искали(
}

int scnframe(TrFrame *fr, TrObject *o, char *str, int strlen, int &ptr)
{
    frmclear(fr);
    fr->parts = o->parts;
    fr->segments = new TrString[fr->parts];
    int i = 0;
    char *data;
    for (int j = 0; j < o->parts; ++j)
    {
        int success = -1;
        for (int k = 0; k < o->sizes[j] && success < 0; ++k)
        {
            TrString *readed = new TrString();
            char *tmp = 0;
            TrObject *ro;
            TrArr *ra;
            int x;//0 - object 1 - arr
            if (o->types[j][k].type.shell->type == 'o')
            {
                x = 0;
                ro = (TrObject*)o->types[j][k].type.shell->value;
            }
            else
            {
                x = 1;
                ra = (TrArr*)o->types[j][k].type.shell->value;
            }
            int max = o->types[j][k].maxcount;
            int size=0;
            if (i >= strlen)
                data = str;
            else
                data = &str[i];
            if (x)
            {
                success = scnarray(&tmp, ra, data, strlen - i, size);
            }
            else
            {
                TrFrame *rf = new TrFrame();
                success = scnframe(rf, ro, data, strlen - i, size);
                //translate frame to cstr;
                if (success == 0)
                {
                    tmp = new char[size + 1];
                    int l = 0;
                    for (int q = 0; q < rf->parts; ++q)
                    {
                        memcpy(&tmp[l] , rf->segments[q].str, rf->segments[q].len);
                        l += rf->segments[q].len;
                    }
                    tmp[size] = 0;
                }
            }
            //first read is in [past
            if (success == 0)
            {
                int success2 = success;
                int count = 0;
                do
                {
                    ++count;
                    i += size;
                    //concatenation
                    TrString *b = new TrString();
                    cstr_to_TrString(b, tmp);
                    delete tmp;
                    TrString *a = new TrString();
                    strassign(a, readed);
                    strconcatenation(a, b, readed);
                    delete a;
                    delete b;
                    //new read
                    if (i >= strlen)
                        data = str;
                    else
                        data = &str[i];
                    if (x)
                    {
                        success2 = scnarray(&tmp, ra, data, strlen - i, size);
                    }
                    else
                    {
                        TrFrame *rf = new TrFrame();
                        success2 = scnframe(rf, ro, data, strlen - i, size);
                        //translate frame to cstr;
                        if (success2 == 0)
                        {
                            tmp = new char[size + 1];
                            int l = 0;
                            for (int q = 0; q < rf->parts; ++q)
                            {
                                memcpy(&tmp[l] , rf->segments[q].str, rf->segments[q].len);
                                l += rf->segments[q].len;
                            }
                            tmp[size] = 0;
                        }
                    }
                    //chek condition
                } while (success2 == 0 && count < max);
                //assign
                strassign(&(fr->segments[j]), readed);
                delete readed;
            }
        }
        if (success)
            return -1;
    }
    ptr = i;
    return 0;
}

int scnfltstr(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    TrVar *obj;
    if (read_var(M, &obj, &code[i], 'o')){return -1;}
    i += 4;
    TrVar *str;
    if (read_var(M, &str, &code[i], 's')){return -1;}
    i += 4;
    char *stri;
    TrString_to_cstr(&stri, (TrString*)str->shell->value);
    TrFrame *fr = new TrFrame();
    int q;
    M->executes[M->ptr].ptr += i;
    if (scnframe(fr, (TrObject*)obj->shell->value, stri, ((TrString*)str->shell->value)->len, q)) {delete[] stri; return 0;}
    delete[] stri;
    varinit(var, 'f', fr);
    M->skip = true;
    return 0;
}

int scnfltfil(TrMachine *M, char *code)
{
    int i = 1;
    TrVar *var;
    if (read_var(M, &var, &code[i], '?')){return -1;}
    i += 4;
    varclear(var);
    TrVar *obj;
    if (read_var(M, &obj, &code[i], 'o')){return -1;}
    i += 4;
    TrVar *fil;
    if (read_var(M, &fil, &code[i], 'c')){return -1;}
    i += 4;
    char *stri;
    stri = &(((TrFile*)fil->shell->value)->data[((TrFile*)fil->shell->value)->ptr]);
    TrFrame *fr = new TrFrame();
    int q;
    M->executes[M->ptr].ptr += i;
    if (scnframe(fr, (TrObject*)obj->shell->value, stri, ((TrFile*)fil->shell->value)->data.size() - ((TrFile*)fil->shell->value)->ptr, q)) {return 0;}
    ((TrFile*)fil->shell->value)->ptr += q;
    varinit(var, 'f', fr);
    M->skip = true;
    return 0;
}
/*///scaning*/
void initmachine(TrMachine *M, FILE *s)
{
    if (M->error != 0)
    {
        fclose(M->error);
        M->error = 0;
    }

    if (M->errenv != 0)
    {
        fclose(M->errenv);
        M->errenv = 0;
    }

    M->clear();
    M->error = fopen(".\\tmp\\TrMachineError.txt", "w");
    M->errenv = fopen(".\\tmp\\TrMachineEnv.ptrobj", "wb");
    M->stack = new TrVector();
    M->skip = false;
    for (int i = 0; i < 100; ++i)
        M->Flags[i] = 0;
    vector < char > str;
    char x;
    x = fgetc(s);
    while (!feof(s))
    {
        str.push_back(x);
        x = fgetc(s);
    }
    M->source = new char[str.size()];
    for (int i = 0; i < str.size(); ++i)
        M->source[i] = str[i];
    M->ptr = 0;
    TrMExecute empty;
    M->executes.push_back(empty);
    init_TrMExecute(&M->executes[0], M->source);
}

#include "assign_functions.h"
#include "skip_functions.h"

int LoopMachineCycle(TrMachine *M)
{
    while (M->executes.size() != 0)
    {
        while (M->executes[M->ptr].source[M->executes[M->ptr].ptr] == -3)
        {
            ++(M->executes[M->ptr].ptr);
        }
        //test skip
        //char *t = &M->executes[M->ptr].source[M->executes[M->ptr].ptr];
        if (M->skip)
        {
            M->skip = false;
            skip_functions[M->executes[M->ptr].source[M->executes[M->ptr].ptr]](M, &(M->executes[M->ptr].source[M->executes[M->ptr].ptr]));
        }
        else
        {
            if (active_functions[M->executes[M->ptr].source[M->executes[M->ptr].ptr]](M, &(M->executes[M->ptr].source[M->executes[M->ptr].ptr])))
                return -1;
        }
    }
}

void ClearResources(TrMachine *M)
{
    for (int i = 0; i < M->executes.size(); ++i)
    {
        TrMExecute *E = &M->executes[i];
        int n;
        read_int(&n, &E->source[0]);
        int k;
        read_int(&k, &E->source[n - 4]);
        for (int j = 0; j < k; ++j)
        {
            varclear((TrVar*)(&E->source[n + j * 4]));
        }
    }
}
