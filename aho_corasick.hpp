#include <algorithm>
#include <map>
#include <string>
#include <vector>

template <class _NodeID, class _CharT, class _SizeType, class _IterableWrapper>
class ITrie {
 public:
  using NodeID = _NodeID;
  using CharT = _CharT;
  using SizeType = _SizeType;
  using IterableWrapper = _IterableWrapper;

 protected:
  struct ITrieNode {
    NodeID parent;
    CharT prev_char;
    std::vector<SizeType> entries;
  };
  SizeType dictionary_size = 0, tree_size = 1;

  virtual NodeID AddNode(NodeID, CharT) = 0;
  virtual void AddEntryToNode(NodeID, SizeType) = 0;

 public:
  // Dictionary interface
  template <class InputIter>
  NodeID Insert(InputIter, InputIter);
  template <class InputIter>
  NodeID Find(InputIter, InputIter) const noexcept;
  virtual const std::vector<SizeType>& GetDictionaryEntries(NodeID) const
      noexcept = 0;
  virtual std::basic_string<CharT> GetStringByIndex(SizeType) const = 0;
  SizeType DictionarySize() const noexcept;

  // Tree interface
  virtual NodeID Root() const noexcept = 0;
  virtual NodeID GetNext(NodeID, CharT) const noexcept = 0;
  virtual NodeID GetParent(NodeID) const noexcept = 0;
  virtual CharT GetIncomingChar(NodeID) const noexcept = 0;
  virtual IterableWrapper GetOutcomingEdges(NodeID) const = 0;
  SizeType TreeSize() const noexcept;

  // Path-related interface
  template <class InputIter>
  NodeID Traverse(InputIter, InputIter, NodeID) const noexcept;
  virtual std::basic_string<CharT> GetString(NodeID) const;
};

template <class _CharT = char, class _SizeType = size_t>
class MapVectorTrie : public ITrie<_SizeType, _CharT, _SizeType,
                                   const std::map<_CharT, _SizeType>&> {
 public:
  using NodeID = typename MapVectorTrie::NodeID;
  using CharT = typename MapVectorTrie::CharT;
  using SizeType = typename MapVectorTrie::SizeType;
  using IterableWrapper = typename MapVectorTrie::IterableWrapper;

 protected:
  struct Node : ITrie<SizeType, CharT, SizeType, IterableWrapper>::ITrieNode {
    std::map<CharT, SizeType> transitions;
  };

  std::vector<Node> nodes_;
  std::vector<NodeID> terminal_nodes_;

  virtual NodeID AddNode(NodeID, CharT) override;
  virtual void AddEntryToNode(NodeID, SizeType) override;

 public:
  MapVectorTrie();

  // Dictionary interface
  virtual const std::vector<SizeType>& GetDictionaryEntries(NodeID) const
      noexcept override;
  virtual std::basic_string<CharT> GetStringByIndex(SizeType) const override;

  // Tree interface
  virtual NodeID Root() const noexcept override;
  virtual NodeID GetNext(NodeID, CharT) const noexcept override;
  virtual NodeID GetParent(NodeID) const noexcept override;
  virtual CharT GetIncomingChar(NodeID) const noexcept override;
  virtual IterableWrapper GetOutcomingEdges(NodeID) const override;
};

////////////////////////////////////////////////////////////////////////////////

template <class _NodeID, class _CharT, class _SizeType, class _IterableWrapper>
template <class InputIter>
typename ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::NodeID
ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::Insert(
    InputIter begin, InputIter end) {
  NodeID current = Root();
  for (; begin != end; ++begin) {
    CharT ch = *begin;
    NodeID next = GetNext(current, ch);
    if (next == current) {
      current = AddNode(current, ch);
      ++tree_size;
    } else
      current = next;
  }
  AddEntryToNode(current, dictionary_size++);
  return current;
}

template <class _NodeID, class _CharT, class _SizeType, class _IterableWrapper>
template <class InputIter>
typename ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::NodeID
ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::Find(
    InputIter begin, InputIter end) const noexcept {
  return Traverse(begin, end, Root());
}

template <class _NodeID, class _CharT, class _SizeType, class _IterableWrapper>
typename ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::SizeType
ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::DictionarySize() const
    noexcept {
  return dictionary_size;
}

template <class _NodeID, class _CharT, class _SizeType, class _IterableWrapper>
typename ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::SizeType
ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::TreeSize() const noexcept {
  return tree_size;
}

template <class _NodeID, class _CharT, class _SizeType, class _IterableWrapper>
template <class InputIter>
typename ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::NodeID
ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::Traverse(
    InputIter begin, InputIter end, NodeID start) const noexcept {
  NodeID current = start;
  for (; begin != end; ++begin) {
    CharT ch = *begin;
    NodeID next = GetNext(current, ch);
    if (next == current) return start;
    current = next;
  }
  return current;
}

template <class _NodeID, class _CharT, class _SizeType, class _IterableWrapper>
std::basic_string<
    typename ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::CharT>
ITrie<_NodeID, _CharT, _SizeType, _IterableWrapper>::GetString(
    NodeID node) const {
  std::basic_string<CharT> string;
  for (; node != Root(); node = GetParent(node))
    string.push_back(GetIncomingChar(node));
  std::reverse(string.begin(), string.end());
  return string;
}

////////////////////////////////////////////////////////////////////////////////

template <class _CharT, class _SizeType>
MapVectorTrie<_CharT, _SizeType>::MapVectorTrie() : nodes_(1) {
  nodes_.front().parent = 0;
  nodes_.front().prev_char = '\0';
}

template <class _CharT, class _SizeType>
typename MapVectorTrie<_CharT, _SizeType>::NodeID
MapVectorTrie<_CharT, _SizeType>::AddNode(NodeID node, CharT ch) {
  nodes_[node].transitions[ch] = nodes_.size();
  nodes_.emplace_back();
  nodes_.back().parent = node;
  nodes_.back().prev_char = ch;
  return nodes_.size() - 1;
}

template <class _CharT, class _SizeType>
void MapVectorTrie<_CharT, _SizeType>::AddEntryToNode(NodeID node,
                                                      SizeType index) {
  nodes_[node].entries.emplace_back(index);
  if (index >= terminal_nodes_.size())
    terminal_nodes_.resize(index + 1);
  terminal_nodes_[index] = node;
}

template <class _CharT, class _SizeType>
const std::vector<typename MapVectorTrie<_CharT, _SizeType>::SizeType>&
MapVectorTrie<_CharT, _SizeType>::GetDictionaryEntries(NodeID node) const
    noexcept {
  return nodes_[node].entries;
}

template <class _CharT, class _SizeType>
std::basic_string<typename MapVectorTrie<_CharT, _SizeType>::CharT>
MapVectorTrie<_CharT, _SizeType>::GetStringByIndex(SizeType index) const {
  return this->GetString(terminal_nodes_[index]);
}

template <class _CharT, class _SizeType>
typename MapVectorTrie<_CharT, _SizeType>::NodeID
MapVectorTrie<_CharT, _SizeType>::Root() const noexcept {
  return 0;
}

template <class _CharT, class _SizeType>
typename MapVectorTrie<_CharT, _SizeType>::NodeID
MapVectorTrie<_CharT, _SizeType>::GetNext(NodeID node, CharT ch) const
    noexcept {
  auto it = nodes_[node].transitions.find(ch);
  if (it == nodes_[node].transitions.end()) return node;
  return it->second;
}

template <class _CharT, class _SizeType>
typename MapVectorTrie<_CharT, _SizeType>::NodeID
MapVectorTrie<_CharT, _SizeType>::GetParent(NodeID node) const noexcept {
  return nodes_[node].parent;
}

template <class _CharT, class _SizeType>
typename MapVectorTrie<_CharT, _SizeType>::CharT
MapVectorTrie<_CharT, _SizeType>::GetIncomingChar(NodeID node) const noexcept {
  return nodes_[node].prev_char;
}

template <class _CharT, class _SizeType>
typename MapVectorTrie<_CharT, _SizeType>::IterableWrapper
MapVectorTrie<_CharT, _SizeType>::GetOutcomingEdges(NodeID node) const {
  return nodes_[node].transitions;
}
