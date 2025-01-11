#include <emscripten.h>

typedef unsigned short ushort;
typedef unsigned char uchar;

ushort pc;

uchar* stack;
uchar stack_i;

ushort* callstack;
uchar callstack_i;

uchar* rom;
uchar* ram;

int main(){
    pc = 0;

    stack = new uchar[256];
    stack_i = 0;

    callstack = new ushort[256];
    callstack_i = 0;

    ram = new uchar[0x6000];

    return 0;
}
uchar Load(ushort addr){
    if(addr >= 0xa000){
        return ram[addr - 0xa000];
    }else{
        return rom[addr];
    }
}
void Push(uchar value){
    stack[stack_i++] = value;
}
uchar Pop(){
    return stack[--stack_i];
}
extern "C" void EMSCRIPTEN_KEEPALIVE Emulate(){
    int emucount = 0;
    uchar tmp = 0;
    while(emucount++ < 100000){
        switch(Load(pc)){
            case 0x00:
                break;
            case 0x01:
                Push(Load(++pc));
                break;
            case 0x02:
                Pop();
                break;
            case 0x03:
                stack_i = 0;
                break;
            case 0x04:
                Push(Pop() + Pop());
                break;
            case 0x05:
                tmp = Pop();
                Push(Pop() - tmp);
                break;
            case 0x06:
                Push(Pop() * Pop());
                break;
            case 0x07:
                tmp = Pop();
                Push(Pop() / tmp);
                break;
            case 0x08:
                tmp = Pop();
                Push(Pop() % tmp);
                break;
            case 0x09:
                Push(~(Pop() & Pop()));
                break;
            case 0x0a:
                Push(Pop() == Pop());
                break;
            case 0x0b:
                Push(Pop() < Pop()); //スタックの都合で逆
                break;
        }
        pc++;
    }
   return;
}