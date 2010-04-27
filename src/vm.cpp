#include "vm.h"

std::ostream& operator<<(std::ostream& out, const Thread& t) {
  out << "{ \"pc\":" << std::hex << t.PC << ", \"Label\":" << t.Label << ", \"Start\":" << t.Start << ", \"End\":"
    << t.End << " }";
  return out;
}

bool Vm::execute(const Instruction* base, const Instruction* pc, Thread& t, ThreadList& next, const byte* cur) {
  switch (pc->OpCode) {
    case LIT_OP:
      if (*cur == pc->Op.Literal) {
        next.push_back(Thread(pc+1, 0, 0, 0));
        return true;
      }
      else {
        return false;
      }
    case JUMP_OP:
      t.PC = base + pc->Op.Offset;
      return true;
  }
  return false;
}
