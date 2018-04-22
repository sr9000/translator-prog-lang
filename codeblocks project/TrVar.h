#ifndef TranslatorVar
#define TranslatorVar

namespace TrVarOperations
{
    #define nil 0
    struct TrShellVar
    {
        char type;
        int counter;
        void * value;
        TrShellVar()
        {
            type = 0;
            counter = 0;
            value = nil;
        }
        ~TrShellVar();
    };
    struct TrVar
    {
        TrShellVar * shell;
        TrVar()
        {
            shell = nil;
        }
        ~TrVar(){};
    };
    void varclear(TrVar *v)
    {
        if (v->shell != nil)
        {
            if (--(v->shell->counter) == 0)
            {
                delete v->shell;
                v->shell = nil;
            }
        }
    }
    void varassign(TrVar *dst, TrVar *src)
    {
        *dst = *src;
        ++(dst->shell->counter);
    }
    //включаем от сюда, так как TrVar уже определён, но нам не хватает данных о других типах, чтобы допилить функции.
    #include "TrString.h"
    using namespace TrStringOperations;
    #include "TrMath.h"
    using namespace TrMathOperations;
    #include "TrBor.h"
    using namespace TrBorOperations;
    #include "TrFile.h"
    using namespace TrFileOperations;
    #include "TrObject.h"
    using namespace TrObjectOperations;
    #include "TrFrame.h"
    using namespace TrFrameOperations;
    #include "TrArr.h"
    using namespace TrArrOperations;
    #include "TrVector.h"
    using namespace TrVectorOperations;
    //b бор
    //a множество(тот же бор)
    //o объект
    //f фрейм
    //i число
    //s строка
    //c файл, code(код исходный и транслированный)
    //v вектор
    TrShellVar::~TrShellVar()
    {
        switch(type)
        {
            case 'b': delete (TrBor*)       value; break;
            case 'a': delete (TrArr*)       value; break;
            case 'o': delete (TrObject*)    value; break;
            case 'f': delete (TrFrame*)     value; break;
            case 'i': delete (TrMath*)      value; break;
            case 's': delete (TrString*)    value; break;
            case 'c': delete (TrFile*)      value; break;
            case 'v': delete (TrVector*)    value; break;
            default: break;
        }
    }
    void typclone(TrVar *dst, TrVar *src)
    {
        dst->shell = new TrShellVar();
        dst->shell->type = src->shell->type;
        dst->shell->counter = 1;
        switch(src->shell->type)
        {
            case 'b': dst->shell->value = (void*) borclone((TrBor*)     src->shell->value); break;
            case 'a': dst->shell->value = (void*) arrclone((TrArr*)     src->shell->value); break;
            case 'o': dst->shell->value = (void*) objclone((TrObject*)  src->shell->value); break;
            case 'f': dst->shell->value = (void*) frmclone((TrFrame*)   src->shell->value); break;
            case 'i': dst->shell->value = (void*) mthclone((TrMath*)    src->shell->value); break;
            case 's': dst->shell->value = (void*) strclone((TrString*)  src->shell->value); break;
            case 'c': dst->shell->value = (void*) filclone((TrFile*)    src->shell->value); break;
            case 'v': dst->shell->value = (void*) vctclone((TrVector*)  src->shell->value); break;
            default: dst->shell->value = nil; break;
        }
    }
    void varcreate(TrVar *var, char type)
    {
        var->shell = new TrShellVar();
        var->shell->type = type;
        var->shell->counter = 1;
        switch(type)
        {
            case 'a': var->shell->value = (void*) new TrArr()     ; break;
            case 'b': var->shell->value = (void*) new TrBor()     ; break;
            case 'o': var->shell->value = (void*) new TrObject()  ; break;
            case 'f': var->shell->value = (void*) new TrFrame()   ; break;
            case 'i': var->shell->value = (void*) new TrMath()    ; break;
            case 's': var->shell->value = (void*) new TrString()  ; break;
            case 'c': var->shell->value = (void*) new TrFile()    ; break;
            case 'v': var->shell->value = (void*) new TrVector()  ; break;
            default: var->shell->value = nil; break;
        }
    }
    void varinit(TrVar *var, char type, void *data)
    {
        var->shell = new TrShellVar();
        var->shell->type = type;
        var->shell->counter = 1;
        var->shell->value = data;
    }
}
#endif
