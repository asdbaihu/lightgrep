#pragma once

#include <set>
#include <vector>
#include <utility>

#include "sparseset.h"

#include "vm_interface.h"
#include "staticvector.h"
#include "skiptable.h"

class Vm: public VmInterface {
public:

  typedef std::vector<Thread> ThreadList;

  Vm();
  Vm(ProgramPtr prog);

  // numCheckedStates should be equal to the number + 1 for the reserved bit
  void init(ProgramPtr prog);

  virtual void startsWith(const byte* beg, const byte* end, uint64 startOffset, HitCallback hitFn, void* userData);
  virtual bool search(const byte* beg, const byte* end, uint64 startOffset, HitCallback hitFn, void* userData);
  virtual void closeOut(HitCallback hitFn, void* userData);
  virtual void reset();

  #ifdef LBT_TRACE_ENABLED
  void setDebugRange(uint64 beg, uint64 end) {
    BeginDebug = beg;
    EndDebug = end;
  }
  #endif

  bool execute(Thread* t, const byte* cur);
  bool execute(ThreadList::iterator t, const byte* cur);

  bool executeEpsilon(Thread* t, uint64 offset);
  bool executeEpsilon(ThreadList::iterator t, uint64 offset);

  void executeFrame(const byte* cur, uint64 offset, HitCallback hitFn);
  void cleanup();

  const ThreadList& first() const { return First; }
  const ThreadList& active() const { return Active; }
  const ThreadList& next() const { return Next; }

  Thread& add(const Thread& t) {
    Active.push_back(t);
    return Active.back();
  }

  unsigned int numActive() const { return Active.size(); }
  unsigned int numNext() const { return Next.size(); }

private:
  void _markSeen(const uint32 label);

  bool _execute(const Instruction* base, ThreadList::iterator t, const byte* cur);
  bool _executeEpsilon(const Instruction* base, ThreadList::iterator t, const uint64 offset);
  bool _executeEpSequence(const Instruction* base, ThreadList::iterator t, const uint64 offset);
  void _executeThread(const Instruction* base, ThreadList::iterator t, const byte* cur, const uint64 offset);
  void _executeFrame(const ByteSet& first, ThreadList::iterator t, const Instruction* base, const byte* cur, const uint64 offset);
  void _cleanup();

  #ifdef LBT_TRACE_ENABLED
  void open_init_epsilon_json(std::ostream& out);
  void close_init_epsilon_json(std::ostream& out) const;
  void open_frame_json(std::ostream& out, uint64 offset, const byte* cur);
  void close_frame_json(std::ostream& out, uint64 offset) const;
  void pre_run_thread_json(std::ostream& out, uint64 offset, const Thread& t,
                           const Instruction* base);
  void post_run_thread_json(std::ostream& out, uint64 offset, const Thread& t,
                            const Instruction* base);
  void thread_json(std::ostream& out, const Thread& t,
                   const Instruction* base, byte state);

  bool first_thread_json;
  std::set<uint64> new_thread_json;

  uint64 BeginDebug, EndDebug;
  uint64 NextId;
  #endif

  uint64 MaxMatches;

  ProgramPtr Prog;
  Instruction* ProgEnd;

  #ifdef LBT_HISTOGRAM_ENABLED
  std::vector<uint32> ProgHistogram;
  #endif

  ThreadList First,
             Active,
             Next;

  bool SeenNone;
  SparseSet Seen;

  std::vector<uint64> MatchEnds;

  std::set< std::pair<uint32,uint64> > CheckStates;

  SparseSet CheckLabels;
  std::vector< std::set<uint64> > CheckOffsets;

  HitCallback* CurHitFn;
  void* UserData;
};
