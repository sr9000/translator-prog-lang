#ifndef TranslatorString
#define TranslatorString

namespace TrStringOperations
{
    #define nil 0
    struct TrString
    {
        int len;
        char *str;
        void Init()
        {
            len = 0;
            str = nil;
        }
        void Dest()
        {
            len = 0;
            if (str != nil)
                delete[] str;
            str = nil;
        }
        TrString()
        {
            Init();
        }
        ~TrString()
        {
            Dest();
        }
    };

    int strcompar(TrString* a, TrString* b)
    {
        if (a->len > b->len)
            return 1;
        if (a->len < b->len)
            return -1;
        for (int i = 0; i < a->len; ++i)
            if(a->str[i] < b->str[i])
                return -1;
            else if (a->str[i] > b->str[i])
                return 1;
        return 0;
    }

    void strassign(TrString* a, TrString* b)
    {
        a->Dest();
        a->len = b->len;
        a->str = new char[a->len];
        memcpy(a->str, b->str, a->len);
    }
    void strconcatenation(TrString* a, TrString* b, TrString *con)
    {
        con->Dest();
        con->len = a->len + b->len;
        con->str = new char[con->len];
        memcpy(con->str, a->str, a->len);
        memcpy(&con->str[a->len], b->str, b->len);
    }
    void strsplit(TrString* a, TrString* head, TrString* tail, int x)
    {
        ++x;
        head->Dest();
        tail->Dest();
        head->len = x;
        tail->len = a->len - x;
        head->str = new char[head->len];
        tail->str = new char[tail->len];
        memcpy(head->str, a->str, head->len);
        memcpy(tail->str, &a->str[x], tail->len);
    }
    void strdelete(TrString* a, int x, int l)
    {
        char *t = new char[a->len - l];
        memcpy(t, a->str, x);
        memcpy(&t[x], &a->str[x + l], a->len - l - x);
        delete[] a->str;
        a->str = t;
        a->len = a->len - l;
    }
    void strfind_substring_KMP (TrString* a, TrString* b, int **mas, int *cnt)
    {
        int i, j, N, M;
        *mas = new int[a->len];
        int cn = 0;
        N = a->len;
        M = b->len;
        int *d = new int[M]; // динамический массив длины М
        // Вычисление префикс-функции
        d[0] = 0;
        for(i = 1, j = 0; i < M; ++i)
        {
            while(j > 0 && b->str[j] != b->str[i])
                j = d[j - 1];
            if(b->str[j] == b->str[i])
                    j++;
            d[i] = j;
        }
        // поиск
        for(i = 0, j = 0; i < N; ++i)
        {
            while(j > 0 && b->str[j] != a->str[i])
                j = d[j - 1];
            if(b->str[j] == a->str[i])
                    j++;
            if (j == M)
            {
                (*mas)[cn++] = i - j + 1;
                j = d[j - 1];
            }
        }
        delete[] d;
        *cnt = cn;
    }
    void cstr_to_TrString(TrString *a, char* str)
    {
        a->Dest();
        a->len = strlen(str);
        a->str = new char[a->len];
        memcpy(a->str, str, a->len);
    }
    void TrString_to_cstr(char **str, TrString *a)
    {
        *str = new char[a->len + 1];
        memcpy(*str, a->str, a->len);
        (*str)[a->len] = 0;
    }
    TrString* strclone(TrString *src)
    {
        TrString *dst = new TrString();
        dst->len = src->len;
        dst->str = new char[dst->len];
        for (int i = 0; i < dst->len; ++i)
            dst->str[i] = src->str[i];
        return dst;
    }
}
#endif
