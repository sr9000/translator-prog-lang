#ifndef TranslatorFile
#define TranslatorFile

namespace TrFileOperations
{
    struct TrFile
    {
        vector < char > data;
        bool write_on_close;
        char *filename;
        int ptr;
        TrFile()
        {
            filename = 0;
        }
        ~TrFile()
        {
            if(filename != 0)
                delete[] filename;
        }
    };

    void filinit(TrFile *f)
    {
        f->ptr = 0;
        f->data.clear();
        f->write_on_close = false;
        if(f->filename != 0)
            delete[] f->filename;
        f->filename = 0;
    }

    void filopenwrite(TrFile *f, char* name)
    {
        filinit(f);
        f->write_on_close = true;
        int l = strlen(name);
        f->filename = new char[l + 1];
        memcpy(f->filename, name, l + 1);
    }

    void filopenread(TrFile *f, char* name)
    {
        FILE *fil = fopen(name, "r");
        filinit(f);
        char x = fgetc(fil);
        while(!feof(fil))
        {
            f->data.push_back(x);
            x = fgetc(fil);
        }
        fclose(fil);
    }

    void filshift(TrFile *f, int shift)
    {
        f->ptr = f->ptr + shift;
        if (f->ptr < 0)
            f->ptr = 0;
        if (f->ptr > f->data.size())
            f->ptr = f->data.size();
    }

    void filwritecstr(TrFile *f, char *str)
    {
        int l = strlen(str);
        if (l <= f->data.size() - f->ptr)
        {
            for(int i = 0; i < l; ++i)
                f->data[i] = str[i];
            f->ptr += l;
        } else
        {
            int n = f->data.size() - f->ptr;
            for (int i = 0; i < n; ++i)
                f->data[i] = str[i];
            for (int i = n; i < l; ++i)
                f->data.push_back(str[i]);
            f->ptr = f->data.size();
        }
    }

    void filclose(TrFile *f)
    {
        if (f->write_on_close)
        {
            FILE *fil = fopen(f->filename, "w");
            for (int i = 0; i < f->data.size(); ++i)
                fputc(f->data[i], fil);
            fclose(fil);
        }
        filinit(f);
    }

    char filreadchar(TrFile *file)
    {
        char x = file->data[file->ptr];
        ++(file->ptr);
        if (file->ptr > file->data.size())
            file->ptr = file->data.size();
        return x;
    }

    bool fileof(TrFile *file)
    {
        return file->ptr >= file->data.size();
    }

    TrFile* filclone(TrFile *src)
    {
        TrFile *dst = new TrFile();
        dst->write_on_close = src->write_on_close;
        dst->data.resize(src->data.size());
        for (int i = 0; i < dst->data.size(); ++i)
            dst->data[i] = src->data[i];
        return dst;
    }
}
#endif
