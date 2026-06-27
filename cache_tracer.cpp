#include "pin.H"
#include <fstream>

using namespace std;

ofstream traceFile;


VOID RecordMemAccess(VOID *addr, BOOL isWrite) {
    traceFile << (isWrite ? "W " : "R ")
              << addr << endl;
}

// read
VOID InsLoad(VOID *addr) {
    RecordMemAccess(addr, FALSE);
}

// write
VOID InsStore(VOID *addr) {
    RecordMemAccess(addr, TRUE);
}

// instrumentation
VOID Instruction(INS ins, VOID *v) {
    if (INS_IsMemoryRead(ins)) {
        INS_InsertCall(ins, IPOINT_BEFORE,
            (AFUNPTR)InsLoad,
            IARG_MEMORYREAD_EA,
            IARG_END);
    }

    if (INS_IsMemoryWrite(ins)) {
        INS_InsertCall(ins, IPOINT_BEFORE,
            (AFUNPTR)InsStore,
            IARG_MEMORYWRITE_EA,
            IARG_END);
    }
}


int main(int argc, char *argv[]) {
    PIN_Init(argc, argv);

    traceFile.open("trace.out");

    INS_AddInstrumentFunction(Instruction, 0);

    PIN_StartProgram();

    traceFile.close();

    return 0;
}