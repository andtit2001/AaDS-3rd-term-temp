#include <algorithm>
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
