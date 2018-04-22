#ifndef TranslatorObject
#define TranslatorObject

namespace TrObjectOperations
{
    #include "TrVar.h"
    using namespace TrVarOperations;
    #define nil 0
    struct TrFragment
    {
        int maxcount;
        TrVar type;
    };
    struct TrObject
    {
        int parts;
        int *sizes;
        TrFragment **types;
        TrObject()
        {
            parts = 0;
            sizes = nil;
            types = nil;
        }
        void clear()
        {
            if (parts)
            {
            for (int i = 0; i < parts; i++)
            {
                for (int j = 0; j < sizes[i]; ++j)
                    varclear(&(types[i][j].type));
                delete[] types[i];
            }
            delete[] types;
            delete[] sizes;
            parts = 0;
            }
        }
        ~TrObject()
        {
            clear();
        }
    };
    void objclear(TrObject *O)
    {
        O->clear();
    }
    void objinitialize(TrObject *O, int _prt, int *sizes, int *cnts, TrVar *types)
    {
        O->clear();
        O->parts = _prt;
        O->sizes = sizes;
        int g = 0;
        O->types = new TrFragment*[_prt];
        for (int i = 0; i < _prt; ++i)
        {
            O->types[i] = new TrFragment[sizes[i]];
            for (int j = 0; j < sizes[i]; ++j, ++g)
            {
                O->types[i][j].maxcount = cnts[g];
                varassign(&(O->types[i][j].type), &(types[g]));
            }
        }
    }
    TrObject* objclone(TrObject *src)
    {
        TrObject *dst = new TrObject();
        dst->parts = src->parts;
        dst->sizes = new int[dst->parts];
        dst->types = new TrFragment* [dst->parts];
        for (int i = 0; i < dst->parts; ++i)
        {
            dst->sizes[i] = src->sizes[i];
            dst->types[i] = new TrFragment[dst->sizes[i]];
            for (int j = 0; j < dst->sizes[i]; ++j)
            {
                dst->types[i][j].maxcount = src->types[i][j].maxcount;
                varassign(&(dst->types[i][j].type), &(src->types[i][j].type));
            }
        }
        return dst;
    }
}
#endif
