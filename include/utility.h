/*
  liblightgrep: not the worst forensics regexp engine
  Copyright (C) 2012 Lightbox Technologies, Inc

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "basic.h"

#include <vector>

#include "automata.h"
#include "vm_interface.h"
#include "patterninfo.h"

struct SearchInfo {};

uint32 estimateGraphSize(const std::vector<Pattern>& keywords);

NFAPtr createGraph(const std::vector<Pattern>& keywords,
                   bool determinize = true,
                   bool ignoreBadParse = false);

class Visitor {
public:
  virtual ~Visitor() {}

  virtual void discoverVertex(NFA::VertexDescriptor,
                              const NFA&) const {}

  virtual void treeEdge(NFA::VertexDescriptor,
                        NFA::VertexDescriptor,
                        const NFA&) const {}
};

void bfs(const NFA& graph, NFA::VertexDescriptor start, Visitor& visitor);

ByteSet firstBytes(const NFA& graph);
void nextBytes(ByteSet& set, NFA::VertexDescriptor v, const NFA& graph);

std::shared_ptr<VmInterface> initVM(const std::vector<Pattern>& keywords, SearchInfo& info);

std::vector<std::vector<NFA::VertexDescriptor>> pivotStates(NFA::VertexDescriptor source, const NFA& graph);

uint32 maxOutbound(const std::vector<std::vector<NFA::VertexDescriptor>>& tranTable);

void writeGraphviz(std::ostream& out, const NFA& graph);
