#ifndef TranslatorVector
#define TranslatorVector

namespace TrVectorOperations
{
    #include "TrVar.h"
    #define nil 0
    using namespace TrVarOperations;
    struct TrVector
    {
        int f, b, count;
        TrVar *mas;
        TrVector()
        {
            f = 9;
            b = 0;
            count = 10;
            mas = new TrVar[count];
        }
        ~TrVector()
        {
            for (int i = (f + 1) % count; i != b; i = (i + 1) % count)
                varclear(&mas[i]);
            delete[] mas;
        }
    };
    int vctsize(TrVector *V)
    {
        return (V->b - V->f - 1 + V->count) % V->count;
    }
    void vctresize(TrVector *V, int S)
    {
        TrVar *data = new TrVar[S + 2];
        int j = 0;
        for (int i = (V->f + 1) % V->count; i != V->b && j < S; i = (i + 1) % V->count, ++j)
        {
            data[j] = V->mas[i];
        }
        delete[] V->mas;
        V->mas = data;
        V->count = S + 2;
        V->f = S + 1;
        V->b = j;
    }
    void vctpushfront(TrVector *V, TrVar *x)
    {
        if ((V->b + 1) % V->count == V->f)
            vctresize(V, 2 * V->count);
        V->mas[V->f] = *x;
        V->f = (V->f + V->count - 1) % V->count;
    }
    void vctpushback(TrVector *V, TrVar *x)
    {
        if ((V->b + 1) % V->count == V->f)
            vctresize(V, 2 * V->count);
        V->mas[V->b] = *x;
        V->b = (V->b + 1) % V->count;
    }
    TrVar* vctpopfront(TrVector *V)
    {
        if ((V->f + 1) % V->count == V->b)
            return nil;
        TrVar *r = new TrVar();
        V->f = (V->f + 1) % V->count;
        *r = V->mas[V->f];
        return r;
    }
    TrVar* vctpopback(TrVector *V)
    {
        if ((V->f + 1) % V->count == V->b)
            return nil;
        TrVar *r = new TrVar();
        V->b = (V->b + V->count - 1) % V->count;
        *r = V->mas[V->b];
        return r;
    }
    TrVar* vctget(TrVector *V, int ind)
    {
        if (ind >= (V->b - V->f - 1 + V->count) % V->count)
            return nil;
        TrVar *r = new TrVar();
        varassign(r, &(V->mas[(V->f + ind + 1) % V->count]));
        return r;
    }
    void vctset(TrVector *V, int ind, TrVar *x)
    {
        if (ind >= (V->b - V->f - 1 + V->count) % V->count)
            return;
        varclear(&(V->mas[(V->f + ind + 1) % V->count]));
        varassign(&(V->mas[(V->f + ind + 1) % V->count]), x);
    }
    TrVector* vctclone(TrVector *src)
    {
        TrVector *dst = new TrVector();
        *dst = *src;
        dst->mas = new TrVar[dst->count];
        for (int i = (dst->f + 1) % dst->count; i != dst->b; i = (i + 1) % dst->count)
        {
            dst->mas[i] = src->mas[i];
        }
        return dst;
    }
}
#endif
