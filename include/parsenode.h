#pragma once

#include "basic.h"

#include <iostream>
#include <limits>

static const uint32 UNBOUNDED = std::numeric_limits<uint32>::max();

struct ParseNode {
  enum NodeType {
    REGEXP,
    ALTERNATION,
    CONCATENATION,
    GROUP,
    REPETITION,
    REPETITION_NG,
    DOT,
    CHAR_CLASS,
    LITERAL,
    TEMPORARY
  };

  NodeType Type;

  ParseNode *Left;

//  union  {
    ParseNode* Right;
    int Val;
    struct {
      uint32 Min, Max;
    } Rep;
    ByteSet Bits;
  //};

  ParseNode(): Type(LITERAL), Left(0), Val(0) {}

  ParseNode(NodeType t, uint32 v): Type(t), Left(0) {
    if (Type == CHAR_CLASS) {
      // Use placement new to initialize Bits; note that this does not
      // allocate memory, so we don't have to delete anything.
      new(&(this->Bits)) ByteSet();
      Bits.set(v);
    }
    else {
      Val = v;
    }
  }

  ParseNode(NodeType t, ParseNode* l):
    Type(t), Left(l), Right(0) {}

  ParseNode(NodeType t, ParseNode* l, ParseNode* r):
    Type(t), Left(l), Right(r) {}

  ParseNode(NodeType t, ParseNode* l, uint32 min, uint32 max):
    Type(t), Left(l)
  {
    Rep.Min = min;
    Rep.Max = max;
  }

  ParseNode(NodeType t, uint32 first, uint32 last):
    Type(t), Left(0), Bits()
  {
    Bits.reset();
    range(first, last);
  }

  explicit ParseNode(NodeType t, const ByteSet& b):
    Type(t), Left(0), Bits(b) {}

  void range(uint32 first, uint32 last) {
    for (uint32 i = first; i <= last; ++i) {
      Bits.set(i);
    }
  }
};

std::ostream& operator<<(std::ostream& out, const ParseNode& n);

void printTree(std::ostream& out, const ParseNode& n);
void printTreeDetails(std::ostream& out, const ParseNode& n);
void repetition(std::ostream& out, uint32 min, uint32 max);
