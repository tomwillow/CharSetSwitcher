//Thunk.h
//ts=sts=sw=4
//女孩不哭 2013-09-11 22:00
//保留所有权利

#pragma once

class TThunk
{
public:
    TThunk();
    ~TThunk();

public:
    template<typename T>
    void *Stdcall(void *pThis, T mfn)
    {
        return fnStdcall(pThis, getmfn(mfn));
    }

    template<typename T>
    void *Cdeclcall(void *pThis, T mfn)
    {
        return fnCdeclcall(pThis, getmfn(mfn));
    }

private:
    typedef unsigned char   byte1;
    typedef unsigned short  byte2;
    typedef unsigned int    byte4;

    void *fnStdcall(void *pThis, void *mfn);
    void *fnCdeclcall(void *pThis, void *mfn);

    template<typename T>
    void *getmfn(T t)
    {
        union {
            T t;
            void *p;
        }u;
        u.t = t;
        return u.p;
    }

private:
#pragma pack(push,1)
    struct MCODE_STDCALL {
        byte1 push[3];
        byte4 mov;
        byte4 pthis;
        byte1 jmp;
        byte4 addr;
    };

    struct MCODE_CDECL {
        byte1 pop_ret[7];
        byte1 push_this[5];
        byte1 push_my_ret[5];
        byte1 jmp_mfn[5];
        byte1 add_esp[3];
        byte1 jmp_ret[7];
        byte4 ret_addr;
    };
#pragma pack(pop)

private:
    MCODE_CDECL     m_cdecl;
    MCODE_STDCALL   m_stdcall;
    TThunk *m_pthis;
};