#ifndef TranslatorFrame
#define TranslatorFrame

namespace TrFrameOperations
{
    #include "TrString.h"
    using namespace TrStringOperations;
    struct TrFrame
    {
        int parts;
        TrString *segments;
        TrFrame()
        {
            parts = 0;
        }
        void clear()
        {
            if (parts)
            {
                delete[] segments;
                parts = 0;
            }
        }
        ~TrFrame()
        {
            clear();
        }
    };
    void frmclear(TrFrame *F)
    {
        F->clear();
    }
    TrFrame* frmclone(TrFrame *src)
    {
        TrFrame *dst = new TrFrame();
        dst->parts = src->parts;
        dst->segments = new TrString [dst->parts];
        for (int i = 0; i < dst->parts; ++i)
            strassign(&(dst->segments[i]), &(src->segments[i]));
        return dst;
    }
}
#endif
