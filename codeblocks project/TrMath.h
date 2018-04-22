#ifndef TranslatorMath
#define TranslatorMath

namespace TrMathOperations
{
    #include <stdlib.h>

    #define MAX_LEN 128
    //don't change BASE value
    #define BASE 1000000000

    struct TrMath
    {
        char sign;//-1; 0; 1;
        int len;
        long long digits[MAX_LEN];
        void Init()
        {
            sign = 0;
            for (int i = 0; i < MAX_LEN; ++i)
                digits[i] = 0;
            len = 0;
        }
        TrMath()
        {
            Init();
        }
        ~TrMath()
        {
        }
    };
    char mthabscmp(TrMath *a, TrMath *b)
    {
        if (a->len < b->len)
           return -1;
        else if (a->len > b->len)
             return 1;
        for (int i = a->len - 1; i > -1; --i)
            if (a->digits[i] < b->digits[i])
                return -1;
            else if (a->digits[i] > b->digits[i])
                return 1;
        return 0;
    }
    void mthstandartization(TrMath *c)
    {
        int l = c->len;
        while (c->digits[l] == 0 && l > 0)
            --l;
        if (c->digits[l] == 0)
        {
            c->len = 1;
            c->sign = 0;
        }
        else
            c->len = l + 1;
    }
    char mthcmp(TrMath *a, TrMath *b)
    {
        mthstandartization(a);
        mthstandartization(b);
        if (a->sign - 1 > b->sign)
            return 1;
        else if (b->sign - 1 > a->sign)
            return -1;
        return mthabscmp(a, b);
    }
    void cstr_to_TrMath(TrMath *a, char *str)
    {
        int l = strlen(str);
        int end = 0;
        long long num = 0;
        a->Init();
        if (str[0] == '-')
        {
            a->sign = -1;
            end = 1;
        }
        else if (str[0] == '+')
        {
            a->sign = 1;
            end = 1;
        }
        else a->sign = 1;
        int i = l - 1;
        for (; i >= end + 8; i -= 9)
        {
            a->digits[a->len++] = str[i] - '0' +
                    10 * (str[i - 1] - '0') +
                    100 * (str[i - 2] - '0') +
                    1000 * (str[i - 3] - '0') +
                    10000 * (str[i - 4] - '0') +
                    100000 * (str[i - 5] - '0') +
                    1000000 * (str[i - 6] - '0') +
                    10000000 * (str[i - 7] - '0') +
                    100000000 * (str[i - 8] - '0');
        }
        if (i >= end)
        {
            for (int j = end; j <= i; ++j)
                num = 10 * num + str[j] - '0';
            if (num) a->digits[a->len++] = num;

        }
        if (a->len == 0)
            a->sign = 0;
    }
    int length_long_long(long long x)
    {
        if (x > 9999999999)
            return 11;
        if (x < 10)
            return 1;
        if (x > 99999)//6-10
            if (x > 9999999)//8-10
                if (x > 999999999)//10
                    return 10;
                else//8-9
                    if (x > 99999999)
                        return 9;
                    else
                        return 8;
            else//6-7
                if (x > 999999)
                    return 7;
                else
                    return 6;
        else//2-5
            if (x > 999)//4-5
                if (x > 9999)
                    return 5;
                else
                    return 4;
            else//2-3
                if (x > 99)
                    return 3;
                else
                    return 2;
    }
    void TrMath_to_cstr(char **str, TrMath *a)
    {
        mthstandartization(a);
        int l = a->len * 9 + 2;
        *str = new char[l];
        for (int i = 0; i < l; ++i)
            (*str)[i] = '0';
        int p = 0;
        if (a->sign < 0)
        {
            (*str)[0] = '-';
            p++;
        }
        lltoa(a->digits[a->len - 1], &(*str)[p], 10);
        p += length_long_long(a->digits[a->len - 1]);
        for (int i = a->len - 2; i > -1; --i)
        {
            (*str)[p] = '0';
            int n = length_long_long(a->digits[i]);
            lltoa(a->digits[i], &(*str)[p + 9 - n], 10);
            p += 9;
        }
        (*str)[p] = 0;
    }
    long long abs(long long x)
    {
        if (x < 0)
            return -x;
        return x;
    }
    void mthalgebricsum(TrMath *a, TrMath *b, TrMath *c)
    {
        mthstandartization(a);
        mthstandartization(b);
        c->Init();
        if (a->sign - b->sign <= 1)//сложение по модулю
        {
            c->sign = a->sign + b->sign;
            if (c->sign < 0)
                c->sign = -1;
            else if (c->sign > 0)
                c->sign = 1;
            else
                c->sign = 0;
            long long tmp = 0;
            int l;
            if (a->len > b->len)
                l = a->len + 1;
            else
                l = b->len + 1;
            for (int i = 0; i < l; ++i)
            {
                tmp += a->digits[i] + b->digits[i];
                c->digits[i] = tmp % BASE;
                tmp /= BASE;
            }
            --l;
            while (c->digits[l] == 0 && l > 0)
                --l;
            if (c->digits[l] == 0)
            {
                c->len = 0;
                c->sign = 0;
            }
            else
                c->len = l + 1;
        }
        else
        {
            //разность
            int cmp = mthabscmp(a, b);
            c->Init();
            if (cmp == 0)
                return;
            if (cmp < 0)
            {
                c->sign = b->sign;
                TrMath *t = a;
                a = b;
                b = t;
            }
            else
                c->sign = a->sign;
            long long tmp = 0;
            int l;
            if (a->len > b->len)
                l = a->len + 1;
            else
                l = b->len + 1;
            for (int i = 0; i < l; ++i)
            {
                tmp = a->digits[i] - b->digits[i];
                if (tmp >= 0)
                {
                    c->digits[i] = tmp;
                    tmp = 0;
                }
                else
                {
                    c->digits[i] = BASE + tmp;
                    tmp = -1;
                }
            }
            --l;
            while (c->digits[l] == 0 && l > 0)
                --l;
            if (c->digits[l] == 0)
            {
                c->len = 0;
                c->sign = 0;
            }
            else
                c->len = l + 1;
        }
    }
    void mthmultiplication(TrMath *a, TrMath *b, TrMath *c)
    {
        mthstandartization(a);
        mthstandartization(b);
        c->Init();
        c->sign = a->sign * b->sign;
        int l = a->len + b->len;
        long long tmp = 0;
        for (int i = 0; i < a->len; ++i)
        {
            tmp = 0;
            for(int j = 0; j <= b->len; ++j)
            {
                tmp += a->digits[i] * b->digits[j] + c->digits[i + j];
                c->digits[i + j] = tmp % BASE;
                tmp /= BASE;
            }
        }
        while (c->digits[l] == 0 && l > 0)
            --l;
        if (c->digits[l] == 0)
        {
            c->len = 0;
            c->sign = 0;
        }
        else
            c->len = l + 1;
    }
    void mthdivision(TrMath *a, TrMath *b, TrMath *div, TrMath *mod)
    {
        mthstandartization(a);
        mthstandartization(b);
        if (b->len < 2 && b->digits[0] == 0)
            return;
        div->Init();
        *mod = *a;
        char cmp = mthabscmp(a, b);
        if (cmp < 0)
        {
            return;
        }
        if (cmp == 0)
        {
            div->digits[0] = 1;
            div->len = 1;
            return;
        }
        //честное деление
        div->sign = a->sign * b->sign;
        long long up[1000], down[1000], l, r, mid[1000], m;
        long long tmp;
        for (int i = mod->len - b->len; i > -1; --i)
        {
            cmp = 0;
            for (int j = b->len; j > -1; --j)
                if (mod->digits[i + j] < b->digits[j])
                {
                    cmp = -1;
                    break;
                }
                else if (mod->digits[i + j] > b->digits[j])
                {
                    cmp = 1;
                    break;
                }
            if (cmp == -1)
                continue;
            up[0] = 0;
            down[0] = 0;
            for (int j = 1; j <= mod->len; ++j)
            {
                down[j] = 0;
                up[j] = b->digits[j - 1];
            }
            l = 0;
            r = BASE;
            while (l < r)
            {
                //coder
                m = (l + r) / 2;
                //multiplication Привет, Стёпка!
                if (m == l)
                {
                    r = l;
                    break;
                }
                tmp = 0;
                for (int j = 0; j <= b->len; ++j)
                {
                    tmp = tmp + b->digits[j] * m;
                    mid[j] = tmp % BASE;
                    tmp /= BASE;
                }
                //comparation
                cmp = 0;
                for (int j = b->len; j > -1; --j)
                    if (mid[j] > mod->digits[i + j])
                    {
                        cmp = 1;
                        break;
                    }
                    else if (mid[j] < mod->digits[i + j])
                    {
                        cmp = -1;
                        break;
                    }
                if (cmp == 0)
                {
                    l = r = m;
                    for (int j = 0; j <= b->len; ++j)
                        down[j] = mid[j];
                }
                else if (cmp == -1)
                {
                    l = m;
                    for (int j = 0; j <= b->len; ++j)
                        down[j] = mid[j];
                }
                else
                {
                    r = m;
                    /*for (int j = 0; j <= b->len; ++j)
                        up[j] = mid[j];*/
                }
            }
            //l is numberchumber
            div->digits[i] = l;
            //substraction
            tmp = 0;
            for (int j = 0; j <= b->len; ++j)
            {
                tmp = mod->digits[i + j] - down[j];
                if (tmp >= 0)
                {
                    mod->digits[i + j] = tmp;
                    tmp = 0;
                }
                else
                {
                    mod->digits[i + j] = BASE + tmp;
                    tmp = -1;
                }
            }
            l = b->len;
            while (mod->digits[l] == 0 && l > 0)
                --l;
            if (mod->digits[l] == 0)
            {
                mod->len = 0;
                mod->sign = 0;
            }
            else
                mod->len = l + 1;
            l = a->len;
            while (div->digits[l] == 0 && l > 0)
                --l;
            if (div->digits[l] == 0)
            {
                div->len = 0;
                div->sign = 0;
            }
            else
                div->len = l + 1;
        }
    }
    TrMath* mthclone(TrMath *src)
    {
        TrMath *dst = new TrMath();
        *dst = *src;
        return dst;
    }
}
#endif
