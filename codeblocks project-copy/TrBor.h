#ifndef TranslatorBor
#define TranslatorBor

namespace TrBorOperations
{
    #include "TrString.h"
    #define nil 0
    struct TBnode;
    struct BBTnode
    {
        BBTnode *left, *right;
        char sym;
        TBnode *val;
        BBTnode()
        {
            left = right = nil;
            val = nil;
        }
        BBTnode(char x)
        {
            left = right = nil;
            sym = x;
            val = nil;
        }
        ~BBTnode()
        {
            if (left != nil)
                delete left;
            if (right != nil)
                delete right;
            if (val != nil)
                delete val;
        }
    };
    struct BBinTree
    {
        BBTnode *head;
        BBinTree()
        {
            head = nil;
        }
        ~BBinTree()
        {
            if (head != nil)
                delete head;
        }
    };
    BBTnode * BTadd(BBinTree *T, char x)
    {
        BBTnode **p = &T->head;
        while (*p != nil)
        {
            if ((*p)->sym == x)
                return (*p);
            else if ((*p)->sym > x)
                p = &(*p)->left;
            else if ((*p)->sym < x)
                p = &(*p)->right;
        }
        *p = new BBTnode(x);
        return (*p);
    }
    void BTremove(BBinTree *T, char x)
    {
        BBTnode **p = &T->head;
        while (*p != nil)
        {
            if ((*p)->sym > x)
                p = &(*p)->left;
            if ((*p)->sym < x)
                p = &(*p)->right;
            if ((*p)->sym == x)//удаляем найденный элемент
            {
                BBTnode **l;
                if ((*p)->left != nil)//если есть левый сын попытаемся поставить его правейшего потомка
                {
                    l = &(*p)->left;
                    if ((*l)->right == nil)//раз нет правейшего потомка, ставим его самого
                    {
                        (*l)->right = (*p)->right;
                        BBTnode *n = *l;
                        (*p)->right = nil;
                        (*p)->left = nil;
                        (*p)->val = nil;
                        delete (*p);
                        *p = n;
                        return;
                    }
                    while ((*l)->right->right != nil)//находим того, кто нам нужен
                        l = &(*l)->right;
                    BBTnode *n =(*l)->right;//фиксируем найденный элемент
                    (*l)->right = n->left;//заполняем место, откуда мы взяли найденный элемент
                    //переназнаим потомков зафиксированного элемента
                    n->left = (*p)->left;
                    n->right = (*p)->right;
                    //удалим более ненужный элемент
                    (*p)->right = nil;
                    (*p)->left = nil;
                    (*p)->val = nil;
                    delete (*p);
                    //подменим удалённый элемент зафиксированным
                    (*p) = n;
                    //конец
                }
                else if ((*p)->right != nil)//тогда может быть есть правый сын? попытаенмся поставить его левейшего потомка
                {
                    l = &(*p)->right;
                    if ((*l)->left == nil)//раз нет правейшего потомка, ставим его самого
                    {
                        (*l)->left = (*p)->left;
                        BBTnode *n = *l;
                        (*p)->left = nil;
                        (*p)->right = nil;
                        (*p)->val = nil;
                        delete (*p);
                        *p = n;
                        return;
                    }
                    while ((*l)->left->left != nil)//находим того, кто нам нужен
                        l = &(*l)->left;
                    BBTnode *n =(*l)->left;//фиксируем найденный элемент
                    (*l)->left = n->right;//заполняем место, откуда мы взяли найденный элемент
                    //переназнаим потомков зафиксированного элемента
                    n->left = (*p)->left;
                    n->right = (*p)->right;
                    //удалим более ненужный элемент
                    (*p)->right = nil;
                    (*p)->left = nil;
                    (*p)->val = nil;
                    delete (*p);
                    //подменим удалённый элемент зафиксированным
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
    struct TBnode
    {
        BBinTree *descendants;
        TBnode *prev;
        int len;
        char sym;
        int key;
        TBnode()
        {
            descendants = new BBinTree();
            prev = nil;
            len = -1;
            key = -1;
        }
        ~TBnode()
        {
            delete descendants;
        }
    };
    struct TrBor
    {
        BBinTree *head;
        TBnode **mas;
        int mascnt;
        int count;
        TrBor()
        {
            head = new BBinTree();
            count = 0;
            mascnt = 10;
            mas = new TBnode* [mascnt];
        }
        ~TrBor()
        {
            delete head;
            delete[] mas;
        }
    };
    using namespace TrStringOperations;
    int borgetnumber(TrBor *B, TrString *str)
    {
        if (str->len == 0)
            return -1;
        BBTnode *q = BTadd(B->head, str->str[0]);
        if (q->val == nil)
            q->val = new TBnode();
        TBnode *p = q->val;
        p->sym = str->str[0];
        for (int i = 1; i < str->len; ++i)
        {
            q = BTadd(p->descendants, str->str[i]);
            if (q->val == nil)
            {
                q->val = new TBnode();
                q->val->prev = p;
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
            TBnode ** data = new TBnode* [B->mascnt];
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
        return p->key;
    }
    TrString *borgetstring(TrBor *B, int n)
    {
        if (n >= B->count)
            return nil;
        TBnode *p = B->mas[n];
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
    void borremove(TrBor *B, int n)
    {
        if (n > B->count)
            return;
        TBnode *p = B->mas[n];
        for (int i = n + 1; i < B->count; ++i)
        {
            --(B->mas[i]->key);
            B->mas[i - 1] = B->mas[i];
        }
        --(B->count);
        int len = p->len;
        TBnode *tmp;
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
    }
    TrBor* borclone(TrBor *src)
    {
        TrBor *dst = new TrBor();
        TrString *str;
        for (int i = 0; i < src->count; ++i)
        {
            str = borgetstring(src, i);
            borgetnumber(dst, str);
            delete str;
        }
        return dst;
    }
}
#endif
