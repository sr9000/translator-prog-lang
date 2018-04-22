
typedef int (*functions)(TrMachine *M, char *code);

functions active_functions[] = {nop,//0
    arr, arradd,arrrem,//3
    bor, boradd,borrem,//6
    borgts,borgtn,obj,//9
    run,//10
    chg,jmp,//12
    end,psh,cln,//15
    var,pop,mth,//18
    set,get,tsts,//21
    tsts,tsts,tsts,//24
    tsts,tsts,tsts,//27
    tsts,tsts,tsts,//30
    tsts,tsts,tsts,//33
    tsts,tsts,tsts,//36
    str,mts,fts,//39
    stm,vct,vctpshbck,//42
    vctpshfrt,vctpopbck,vctpopfrt,//45
    vctget,vctset,vctgsz,//48
    vctrsz,txtlen,txtctn,//51
    txtfnd,txtspl,txtdel,//54
    prt,opnrdf,opnwrf,//57
    cls,nxt,prv,//60
    wrt,scndef,scnfltfil,//63
    scnfltstr};//64
