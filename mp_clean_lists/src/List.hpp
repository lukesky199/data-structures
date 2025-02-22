/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in MP3.1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in MP3.1
  if (tail_ == NULL) 
    return List<T>::ListIterator(NULL);

  return List<T>::ListIterator(tail_->next);
  // return List<T>::ListIterator(tail_, 0);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in MP3.1
  ListNode* next;
  while (head_ != NULL) {
    next = head_->next;
    delete head_;
    head_ = next;
  }
  tail_ = NULL;
  length_ = 0;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in MP3.1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = head_;
  newNode -> prev = NULL;
  
  if (head_ != NULL) {
    head_ -> prev = newNode;
  }
  if (tail_ == NULL) {
    tail_ = newNode;
  }
  head_ = newNode;
  

  length_++;

}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in MP3.1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = NULL;
  newNode -> prev = tail_;
  
  if (tail_ != NULL) {
    tail_ -> next = newNode;
  }
  if (head_ == NULL) {
    head_ = newNode;
  }
  tail_ = newNode;

  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in MP3.1
  ListNode * curr = start;

  for (int i = 0; i < splitPoint && curr != NULL; i++) {
    curr = curr->next;
  }

  if (curr != NULL) {
      if (curr->prev != NULL)
        curr->prev->next = NULL;
      curr->prev = NULL;
  }

  return curr;
}

/**
  * Modifies List using the rules for a TripleRotate.
  *
  * This function will to a wrapped rotation to the left on every three 
  * elements in the list starting for the first three elements. If the 
  * end of the list has a set of 1 or 2 elements, no rotation all be done 
  * on the last 1 or 2 elements.
  * 
  * You may NOT allocate ANY new ListNodes!
  */
template <typename T>
void List<T>::tripleRotate() {
  // @todo Graded in MP3.1
  ListNode* curr = head_;
  for (int i = 0; length_ - i >= 3; i+= 3) { //i = 3, length_ - i = 3
    ListNode* rotated = curr;
    curr = curr->next;
    ListNode* new_right = curr;
    if (i == 0) // head_ edgecase
      head_ = new_right;
    
    curr = curr->next;
    rotated->next = curr->next; // Setting up ->next
    curr->next = rotated;

    new_right->prev = rotated->prev; // Setting up ->prev
    if (rotated->prev != NULL)
      rotated->prev->next = new_right;
    rotated->prev = curr;

    curr = rotated->next; // setting up for the next triple
    if (curr == NULL) { // tail_ edgecase
      tail_ = rotated;
    } else {
      curr->prev = rotated;
    }
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in MP3.2
  if (startPoint == endPoint) { return; } //If nullptr or only 1 node
  ListNode* startPrev = startPoint->prev;
  ListNode* endNext = endPoint->next;
  ListNode* current = startPoint;
  ListNode* tmp = nullptr;

  while (current != endPoint) {
    tmp = current->prev;
    current->prev = current->next;
    current->next = tmp;
    current = current->prev;
  }
  tmp = current->prev;
  current->prev = startPrev;
  current->next = tmp;

  if(tmp != nullptr) {
    endPoint = startPoint;
    startPoint = tmp->prev;
    endPoint->next = endNext;
  }
}

#include <iostream>
/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in MP3.2
  ListNode* current = head_;
  ListNode* start;
  while (current != tail_) {
    start = current;
    for (int i = 0; i < n && current != tail_; i++) {
      current = current->next;
    }
    if (start == head_) {
      if (current == tail_) {
        reverse(head_, tail_);
        break;
      } else {
        reverse(head_, current->prev);
      }
    } else {
      if (current == tail_) {
        reverse(start->prev->next, tail_);
        break;
      } else {
        reverse(start->prev->next, current->prev);
      }
    }
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

#include <stack>
/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in MP3.2
  if (first == nullptr) {
    return second;
  } else if (second == nullptr) {
    return first;
  }

  ListNode* newHead = first;
  ListNode* current = second;
  ListNode* iter = newHead;
  
  while (current != nullptr) {
    ListNode* insert = current;
    current = current->next;

    if (insert->data < newHead->data) {
      insert->next = newHead;
      insert->prev = nullptr;
      newHead->prev = insert;
      newHead = insert;
      continue;
    }

    while (iter->next != nullptr && iter->next->data < insert->data) {
      iter = iter->next;
    }
    if (insert->data < iter->data) {
      iter->prev->next = insert;
      insert->prev = iter->prev;
      iter->prev = insert;
      insert->next = iter;
    } else if (iter->next == nullptr) {
      iter->next = insert;
      insert->prev = iter;
      insert->next = nullptr;
    } else {
      insert->next = iter->next;
      iter->next->prev = insert;
      insert->prev = iter;
      iter->next = insert;
    }
  }

  return newHead;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in MP3.2
  if (chainLength == 1) {
    return start;
  }

  ListNode* current = start;
  for (int i = 0; i < chainLength/2; i++) {
    // std::cout<< current->data << " ";
    current = current->next;
  }

  // std::cout << std::endl;
  current->prev->next = nullptr;
  current->prev = nullptr;

  // std::cout << __LINE__ << std::endl;
  
  ListNode* a = mergesort(start, chainLength/2);
  ListNode* b = mergesort(current, chainLength - chainLength/2);

  // std::cout << __LINE__ << std::endl;
  ListNode* newHead = merge(a, b);
  // std::cout << __LINE__ << std::endl;

  return newHead;
}
