#include <initializer_list>
#include <stdexcept>
template<typename> class list;
template<typename T, bool isConst> class ListIterator;

template<typename T> class Node{
  friend class list<T>;
  friend class ListIterator<T, true>;
  friend class ListIterator<T, false>;
  public:
    Node(): next(nullptr), prev(nullptr) { }
    Node(const T &a): Node() { data = a; }
  private:
    Node *next;
    T data;
    Node *prev;
};

template<typename T> class list{
  public:
    using iterator = ListIterator<T, false>;
    using const_iterator = ListIterator<T, true>;

    list(): sentinel(new Node<T>()) {
      sentinel->next = sentinel->prev = sentinel;
    }
    list(std::initializer_list<T> il);

    void push_front(const T&);
    void push_back(const T&);
    bool empty();
    iterator insert(const Node<T>*, const T&);
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    T& front();
    T& back();
    const T& front() const;
    const T& back() const;

  private:
    void check(const std::string &msg);
    Node<T> *sentinel;
};

template<typename T, bool b1, bool b2>
bool operator==(const ListIterator<T, b1>&, const ListIterator<T, b2>&);

template<typename T, bool b1, bool b2>
bool operator!=(const ListIterator<T, b1>&, const ListIterator<T, b2>&);

template <typename T, bool isConst> class ListIterator {
  template<typename U, bool b1, bool b2> friend
    bool operator==(const ListIterator<U, b1>&, const ListIterator<U, b2>&);

  template<typename U, bool b1, bool b2> friend
    bool operator!=(const ListIterator<U, b1>&, const ListIterator<U, b2>&);

  friend class ListIterator<T, true>;
  friend class ListIterator<T, false>;

  using ref_t = std::conditional_t<isConst, const T&, T&>;
  using ptr_t = std::conditional_t<isConst, const T*, T*>;
  public:
    ListIterator(): ptr(nullptr) { }
    ListIterator(Node<T> *p): ptr(p) { }
    ListIterator(const ListIterator<T, false>& rhs): ptr(rhs.ptr) { }

    ListIterator& operator++();
    ListIterator& operator--();
    ListIterator operator++(int);
    ListIterator operator--(int);

    ref_t operator*() const; 
    ptr_t operator->() const;
  private:
    Node<T> *ptr;
};

template<typename T, bool isConst>
auto ListIterator<T, isConst>::operator*() const -> ref_t{
  return (*ptr).data;
}

template<typename T, bool isConst>
auto ListIterator<T, isConst>::operator->() const -> ptr_t{
  return & *this->operator*();
}

template<typename T, bool isConst> 
ListIterator<T, isConst>& ListIterator<T, isConst>::operator++(){
  ptr = ptr->next;
  return *this;
}

template<typename T, bool isConst>
ListIterator<T, isConst>& ListIterator<T, isConst>::operator--(){
  ptr = ptr->prev;
  return *this;
}

template<typename T, bool isConst>
ListIterator<T, isConst> ListIterator<T, isConst>::operator++(int){
  ListIterator ret = *this;
  ++*this;
  return ret;
}

template<typename T, bool isConst>
ListIterator<T, isConst> ListIterator<T, isConst>::operator--(int){
  ListIterator ret = *this;
  --*this;
  return ret;
}


template<typename T>
typename list<T>::iterator list<T>::insert(const Node<T> *pos, const T& t){
  Node<T> *prev = pos->prev;
  Node<T> *next = prev->next;
  Node<T> *new_node = new Node<T>(t);
  new_node->next = next;
  new_node->prev = prev;
  prev->next = new_node;
  next->prev = new_node;

  return new_node;
}

template<typename T>
void list<T>::push_back(const T &t){
  insert(sentinel, t);
}

template<typename T>
void list<T>::push_front(const T &t){
  insert(sentinel->next, t);
}

template<typename T>
bool list<T>::empty() {
  if (!sentinel->next && !sentinel->prev)
    return true;
  return false;
}

template<typename T>
typename list<T>::iterator list<T>::begin() {
  return sentinel->next;
}

template<typename T>
typename list<T>::iterator list<T>::end() {
  return sentinel->prev;
}

template<typename T>
typename list<T>::const_iterator list<T>::begin() const {
  return sentinel->next;
}


template<typename T>
typename list<T>::const_iterator list<T>::end() const{
  return sentinel->prev;
}
template<typename T>
typename list<T>::const_iterator list<T>::cbegin() const{
  return sentinel->next;
}
template<typename T>
typename list<T>::const_iterator list<T>::cend() const{
  return sentinel->prev;
}


template<typename T>
T& list<T>::front() {
  check("front on empty list");
  return begin()->data;
}


template<typename T>
T& list<T>::back() {
  check("back on empty list");
  return end()->data;
}

template<typename T>
const T& list<T>::front() const {
  check("front on empty list");
  return begin()->data;
}

template<typename T>
const T& list<T>::back() const {
  check("back on empty list");
  return end()->data;
}

template<typename T>
void list<T>::check(const std::string &msg){
  if (empty())
    throw std::out_of_range(msg);
}

template<typename T, bool b1, bool b2>
bool operator==(const ListIterator<T, b1> &lhs, const ListIterator<T, b2> &rhs) {
  return lhs.ptr == rhs.ptr;
}

template<typename T, bool b1, bool b2> 
bool operator!=(const ListIterator<T, b1> &lhs, const ListIterator<T, b2> &rhs) {
  return !(lhs == rhs);
}
