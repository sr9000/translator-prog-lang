
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
using namespace std;

#include "TrString.h"
using namespace TrStringOperations;
#include "TrMath.h"
using namespace TrMathOperations;
#include "TrBor.h"
using namespace TrBorOperations;
#include "TrVar.h"
using namespace TrVarOperations;
#include "TrVector.h"
using namespace TrVectorOperations;
#include "TrFile.h"
using namespace TrFileOperations;
#include "TrObject.h"
using namespace TrObjectOperations;
#include "TrFrame.h"
using namespace TrFrameOperations;
#include "TrArr.h"
using namespace TrArrOperations;




#include "TrCompiler.h"
#include "TrMachine.h"

int main()
{
    char name[500];
    FILE *file = fopen("settings.txt", "r");
    if(ferror(file))
        return -100;
    fgets(name, 490, file);
    fclose(file);
    //translate to names
    int len = strlen(name);
    char trobjname[500];
    memcpy(trobjname, name, len);
    char trobj[] = ".trobj";
    memcpy(&trobjname[len], trobj, strlen(trobj) + 1);

    //TRcompiler
    char trname[500];
    memcpy(trname, name, len);
    char tr[] = ".tr";
    memcpy(&trname[len], tr, strlen(tr) + 1);

    FILE *f = fopen(trname, "r");
    FILE *o = fopen(trobjname, "wb");
    FILE *l = fopen(".\\tmp\\parse.txt", "w");
    FILE *error = fopen(".\\tmp\\error.txt", "w");
    int r = Compile(f, o, l, error);
    fclose(f);
    fclose(o);
    fclose(l);
    return r;
    //if (r){return r;}*/
    //TRmachine
    /*FILE *f = fopen(trobjname, "rb");
    TrMachine m;
    initmachine(&m, f);
    int res = LoopMachineCycle(&m);
    ClearResources(&m);
    fclose(f);
    return res;
    //*/
}
