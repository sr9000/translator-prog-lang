#ifndef TranslatorArr
#define TranslatorArr

namespace TrArrOperations
{
    #include "TrString.h"
    #define nil 0
    struct TAnode;
    struct BTnode
    {
        BTnode *left, *right;
        char sym;
        TAnode *val;
        BTnode()
        {
            left = right = nil;
            val = nil;
        }
        BTnode(char x)
        {
            left = right = nil;
            sym = x;
            val = nil;
        }
        ~BTnode()
        {
            if (left != nil)
                delete left;
            if (right != nil)
                delete right;
            if (val != nil)
                delete val;
        }
    };
    struct BinTree
    {
        BTnode *head;
        BinTree()
        {
            head = nil;
        }
        ~BinTree()
        {
            if (head != nil)
                delete head;
        }
    };
    BTnode * BTadd(BinTree *T, char x)
    {
        BTnode **p = &T->head;
        while (*p != nil)
        {
            if ((*p)->sym == x)
                return (*p);
            else if ((*p)->sym > x)
                p = &(*p)->left;
            else if ((*p)->sym < x)
                p = &(*p)->right;
        }
        *p = new BTnode(x);
        return (*p);
    }
    void BTremove(BinTree *T, char x)
    {
        BTnode **p = &T->head;
        while (*p != nil)
        {
            if ((*p)->sym > x)
                p = &(*p)->left;
            if ((*p)->sym < x)
                p = &(*p)->right;
            if ((*p)->sym == x)//удал€ем найденный элемент
            {
                BTnode **l;
                if ((*p)->left != nil)//если есть левый сын попытаемс€ поставить его правейшего потомка
                {
                    l = &(*p)->left;
                    if ((*l)->right == nil)//раз нет правейшего потомка, ставим его самого
                    {
                        (*l)->right = (*p)->right;
                        BTnode *n = *l;
                        (*p)->right = nil;
                        (*p)->left = nil;
                        (*p)->val = nil;
                        delete (*p);
                        *p = n;
                        return;
                    }
                    while ((*l)->right->right != nil)//находим того, кто нам нужен
                        l = &(*l)->right;
                    BTnode *n =(*l)->right;//фиксируем найденный элемент
                    (*l)->right = n->left;//заполн€ем место, откуда мы вз€ли найденный элемент
                    //переназнаим потомков зафиксированного элемента
                    n->left = (*p)->left;
                    n->right = (*p)->right;
                    //удалим более ненужный элемент
                    (*p)->right = nil;
                    (*p)->left = nil;
                    (*p)->val = nil;
                    delete (*p);
                    //подменим удалЄнный элемент зафиксированным
                    (*p) = n;
                    //конец
                }
                else if ((*p)->right != nil)//тогда может быть есть правый сын? попытаенмс€ поставить его левейшего потомка
                {
                    l = &(*p)->right;
                    if ((*l)->left == nil)//раз нет правейшего потомка, ставим его самого
                    {
                        (*l)->left = (*p)->left;
                        BTnode *n = *l;
                        (*p)->left = nil;
                        (*p)->right = nil;
                        (*p)->val = nil;
                        delete (*p);
                        *p = n;
                        return;
                    }
                    while ((*l)->left->left != nil)//находим того, кто нам нужен
                        l = &(*l)->left;
                    BTnode *n =(*l)->left;//фиксируем найденный элемент
                    (*l)->left = n->right;//заполн€ем место, откуда мы вз€ли найденный элемент
                    //переназнаим потомков зафиксированного элемента
                    n->left = (*p)->left;
                    n->right = (*p)->right;
                    //удалим более ненужный элемент
                    (*p)->right = nil;
                    (*p)->left = nil;
                    (*p)->val = nil;
                    delete (*p);
                    //подменим удалЄнный элемент зафиксированным
                    (*p) = n;
                    //конец
                }
                else
                {
                    //никого не и не будет ;)
                    (*p)->val = nil;
                    delete (*p);
                    *p = nil;
                }
                return;//уиииииииииии........
            }
        }
    }
    struct TAnode
    {
        BinTree *descendants;
        TAnode *prev;
        int len;
        char sym;
        int key;
        int minimalkey;
        int version;
        TAnode()
        {
            descendants = new BinTree();
            prev = nil;
            len = -1;
            key = -1;
            minimalkey = 2147483647;
        }
        ~TAnode()
        {
            delete descendants;
        }
    };
    struct TrArr
    {
        BinTree *head;
        TAnode **mas;
        int mascnt;
        int count;
        int ver;
        TrArr()
        {
            head = new BinTree();
            count = 0;
            mascnt = 10;
            mas = new TAnode* [mascnt];
            ver = 0;
        }
        ~TrArr()
        {
            delete head;
            delete[] mas;
        }
    };
    using namespace TrStringOperations;
    int arrgetnumber(TrArr *B, TrString *str)
    {
        if (str->len == 0)
            return -1;
        BTnode *q = BTadd(B->head, str->str[0]);
        if (q->val == nil)
        {
            q->val = new TAnode();
            q->val->version = B->ver - 1;
        }
        TAnode *p = q->val;
        p->sym = str->str[0];
        for (int i = 1; i < str->len; ++i)
        {
            q = BTadd(p->descendants, str->str[i]);
            if (q->val == nil)
            {
                q->val = new TAnode();
                q->val->prev = p;
                p->version = B->ver - 1;
            }
            p = q->val;
            p->sym = str->str[i];
        }
        if (p->key > -1)
            return p->key;
        p->key = (B->count)++;
        p->len = str->len;
        if (B->count > B->mascnt)
        {
            int l = B->mascnt;
            B->mascnt = l * 2;
            TAnode ** data = new TAnode* [B->mascnt];
            for (int i = 0; i < l; i++)
                data[i] = B->mas[i];
            data[p->key] = p;
            delete[] B->mas;
            B->mas = data;
        }
        else
        {
            B->mas[p->key] = p;
        }
        //обновл€€ем минимумы
        int key = p->key;
        int len = p->len;
        for (int i = 0; i < len; ++i)
        {
            if (B->ver != p->version)
            {
                p->version = B->ver;
                p->minimalkey = key;
                p = p->prev;
            }
            else if (p->minimalkey > key)
            {
                p->minimalkey = key;
                p = p->prev;
            }
            else
                break;
        }
        return key;
    }
    TrString* arrgetstring(TrArr *B, int n)
    {
        if (n >= B->count)
            return nil;
        TAnode *p = B->mas[n];
        TrString *str = new TrString();
        str->len = p->len;
        str->str = new char[str->len];
        for (int i = str->len - 1; i > -1; --i)
        {
            str->str[i] = p->sym;
            p = p->prev;
        }
        return str;
    }
    void arrremove(TrArr *B, int n)
    {
        if (n > B->count)
            return;
        TAnode *p = B->mas[n];
        for (int i = n + 1; i < B->count; ++i)
        {
            --(B->mas[i]->key);
            B->mas[i - 1] = B->mas[i];
        }
        --(B->count);
        int len = p->len;
        TAnode *tmp;
        p->key = -1;
        int i;
        for (i = 1; i < len; ++i)
        {
            if (p->descendants->head == nil)
            {
                tmp = p->prev;
                BTremove(tmp->descendants, p->sym);
                delete p;
                p = tmp;
            }
            else
                break;
        }
        if (i == len)
        {
            if (p->descendants->head == nil)
            {
                BTremove(B->head, p->sym);
                delete p;
            }
        }
        //обновление минимумов.. оооочень долго(((((
        ++(B->ver);
        for (int j = 0; j < B->count; ++j)
        {
            p = B->mas[i];
            int key = p->key;
            len = p->len;
            for (int i = 0; i < len; ++i)
            {
                if (B->ver != p->version)
                {
                    p->version = B->ver;
                    p->minimalkey = key;
                    p = p->prev;
                }
                else if (p->minimalkey > key)
                {
                    p->minimalkey = key;
                    p = p->prev;
                }
                else
                    break;
            }
        }
    }
    TrArr* arrclone(TrArr *src)
    {
        TrArr *dst = new TrArr();
        TrString *str;
        for (int i = 0; i < src->count; ++i)
        {
            str = arrgetstring(src, i);
            arrgetnumber(dst, str);
            delete str;
        }
        return dst;
    }
}
#endif
