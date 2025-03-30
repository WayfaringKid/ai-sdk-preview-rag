#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP
/* BinarySearchTree.hpp
 *
 * Abstract data type representing a binary search tree
 *
 * By Andrew DeOrio <awdeorio@umich.edu>
 *    Amir Kamil    <akamil@umich.edu>
 *    James Juett   <jjuett@umich.edu>
 * Updated
 *   2017-05-01
 *
 * DO NOT CHANGE any of the implementations in this file that are
 * provided with the starter code. Only fill in implementations
 * for the private static member functions as directed.
 */

#include <cassert>  //assert
#include <iostream> //ostream
#include <functional> //less

// You may add aditional libraries here if needed. You may use any
// part of the STL except for containers.

template <typename T,
          typename Compare=std::less<T> // default if argument isn't provided
         >
class BinarySearchTree {

  // OVERVIEW: This class represents a binary search tree, storing
  // elements of type T. The Compare functor determines the ordering
  // between elements. The default is std::less<T>, which orders
  // according to the < operator on T. (For simplicity, we assume only
  // comparators that can be default constructed will be used.)

  // INVARIANTS: All these invariants must hold for valid implementations
  // of BinarySearchTree. The invariants may also be considered as an implicit
  // part of the REQUIRES clause for all member functions - your implementations
  // of those functions may assume the invariants hold and depend on them. Your
  // implementations of member functions must also ensure the invariants hold
  // when they have finished executing.
  //
  // INVARIANT: NO DUPLICATES
  // The BST is not allowed to contain duplicate elements.
  //
  // INVARIANT: SORTING
  // Consider a pointer to a node in the tree structure. It must obey
  // the following sorting invariant:
  // Either:
  //   1) it is null (i.e. representing an empty part of the tree)
  // OR
  //   2) the node's left subtree obeys the sorting invariant, and every
  //      element in the left subtree is strictly less than the datum
  //      in the node
  //      -- AND --
  //      the node's right subtree obeys the sorting invariant, and every
  //      element in the right subtree is strictly greater than the datum
  //      in the node
  // Again, "less than" and "greater than" are as defined by the
  // Compare functor. Note that "greater than or equal to" and
  // "greater than" end up meaning the same thing when duplicates are
  // not allowed.

  // NOTE: Any operation you define must use RECURSION rather than iteration.
  //       You may NOT use any looping constructs.

private:

  // A Node stores an element and pointers to its left and right children
  // DO NOT change this struct definition.
  struct Node {

    // Default constructor - does nothing
    Node() {}

    // Custom constructor provided for convenience
    Node(const T &datum_in, Node *left_in, Node *right_in)
            : datum(datum_in), left(left_in), right(right_in) { }

    T datum;
    Node *left;
    Node *right;
  };

public:

  // Default constructor
  // (Note this will default construct the less comparator)
  BinarySearchTree()
    : root(nullptr) { }

  // Copy constructor
  BinarySearchTree(const BinarySearchTree &other)
    : root(copy_nodes_impl(other.root)) { }

  // Assignment operator
  BinarySearchTree &operator=(const BinarySearchTree &rhs) {
    if (this == &rhs) {
      return *this;
    }
    destroy_nodes_impl(root);
    root = copy_nodes_impl(rhs.root);
    return *this;
  }

  // Destructor
  ~BinarySearchTree() {
    destroy_nodes_impl(root);
  }

  // EFFECTS: Returns whether this BinarySearchTree is empty.
  bool empty() const {
    return empty_impl(root);
  }

  // EFFECTS: Returns the height of the tree.
  size_t height() const {
    return static_cast<size_t>(height_impl(root));
  }

  // EFFECTS: Returns the number of elements in this BinarySearchTree.
  size_t size() const {
    return static_cast<size_t>(size_impl(root));
  }

  // EFFECTS: Traverses the tree using an in-order traversal,
  //          printing each element to os in turn. Each element is followed
  //          by a space (there will be an "extra" space at the end).
  //          If the tree is empty, nothing is printed.
  // MODIFIES: os
  void traverse_inorder(std::ostream &os) const {
    traverse_inorder_impl(root, os);
  }

  // EFFECTS: Traverses the tree using a pre-order traversal,
  //          printing each element to os in turn. Each element is followed
  //          by a space (there will be an "extra" space at the end).
  //          If the tree is empty, nothing is printed.
  // MODIFIES: os
  void traverse_preorder(std::ostream &os) const {
    traverse_preorder_impl(root, os);
  }

  // EFFECTS: Returns whether or not the sorting invariant holds on
  //          the root of this BinarySearchTree.
  //
  // NOTE: This function must be recursive.
  bool check_sorting_invariant() const {
    return check_sorting_invariant_impl(root, less);
  }

  class Iterator {
    // OVERVIEW: Iterator interface for BinarySearchTree.
    //           Iterates over the elements in ascending order as defined
    //           by the sorted ordering of the BinarySearchTree.

    // Big Three for Iterator not needed

  public:
    Iterator()
      : root(nullptr), current_node(nullptr) {}

    // EFFECTS:  Returns the current element by reference.
    // WARNING:  Dereferencing an iterator returns an element from the tree
    //           by reference, which could be modified. It is the
    //           responsibility of the user to ensure that any
    //           modifications result in a new value that compares equal
    //           to the existing value. Otherwise, the sorting invariant
    //           will no longer hold.
    T &operator*() const {
      return current_node->datum;
    }

    // EFFECTS:  Returns the current element by pointer.
    // WARNING:  Dereferencing an iterator returns an element from the tree
    //           by reference, which could be modified. It is the
    //           responsibility of the user to ensure that any
    //           modifications result in a new value that compares equal
    //           to the existing value. Otherwise, the sorting invariant
    //           will no longer hold.
    // NOTE:     This allows the -> operator to be applied to an iterator
    //           to access a member of the pointed-to element:
    //             BinarySearchTree<std::pair<int, double>> tree;
    //             auto it = tree.insert({ 3, 4.1 });
    //             cout << it->first << endl; // prints 3
    //             cout << it->second << endl; // prints 4.1
    T *operator->() const {
      return &current_node->datum;
    }

    // Prefix ++
    Iterator &operator++() {
      if (current_node->right) {
        // If has right child, next element is minimum of right subtree
        current_node = min_element_impl(current_node->right);
      }
      else {
        // Otherwise, look in the whole tree for the next biggest element
        current_node = min_greater_than_impl(root, current_node->datum, less);
      }
      return *this;
    }

    // Postfix ++ (implemented in terms of prefix ++)
    Iterator operator++(int) {
      Iterator result(*this);
      ++(*this);
      return result;
    }

    bool operator==(const Iterator &rhs) const {
      return current_node == rhs.current_node;
    }

    bool operator!=(const Iterator &rhs) const {
      return current_node != rhs.current_node;
    }

  private:
    friend class BinarySearchTree;

    Node *root;
    Node *current_node;
    Compare less;

    Iterator(Node *root_in, Node* current_node_in, Compare less_in)
      : root(root_in), current_node(current_node_in), less(less_in) { }

  }; // BinarySearchTree::Iterator
  ////////////////////////////////////////


  // EFFECTS : Returns an iterator to the first element in this
  //           BinarySearchTree or an end Iterator if the tree is empty.
  Iterator begin() const {
    if (root == nullptr) {
      return Iterator();
    }
    return Iterator(root, min_element_impl(root), less);
  }

  // EFFECTS: Returns an iterator to past-the-end.
  Iterator end() const {
    return Iterator();
  }


  // EFFECTS: Returns an Iterator to the minimum element in this
  //          BinarySearchTree or an end Iterator if the tree is empty.
  Iterator min_element() const {
    return Iterator(root, min_element_impl(root), less);
  }

  // EFFECTS: Returns an Iterator to the maximum element in this
  //          BinarySearchTree or an end Iterator if the tree is empty.
  Iterator max_element() const {
    return Iterator(root, max_element_impl(root), less);
  }

  // EFFECTS: Returns an Iterator to the minimum element in this
  //          BinarySearchTree greater than the given value.
  //          If the tree is empty or if no element is greater than
  //          the given value, returns an end Iterator.
  Iterator min_greater_than(const T &value) const {
    return Iterator(root, min_greater_than_impl(root, value, less), less);
  }


  // EFFECTS: Searches this tree for an element equivalent to query.
  //          Returns an iterator to the existing element if found,
  //          and an end iterator otherwise.
  // WARNING: This function returns an Iterator that allows an element
  //          contained in this tree to be modified. It is the
  //          responsibility of the user to ensure that any
  //          modifications result in a new value that compares equal
  //          to the existing value. Otherwise, the sorting invariant
  //          will no longer hold.
  Iterator find(const T &query) const {
    return Iterator(root, find_impl(root, query, less), less);
  }

  // REQUIRES: The given item is not already contained in this BinarySearchTree
  // MODIFIES: this BinarySearchTree
  // EFFECTS : Inserts the element k into this BinarySearchTree, maintaining the
  //           sorting invariant. Returns an iterator to the newly inserted element.
  Iterator insert(const T &item) {
    assert(find(item) == end());
    root = insert_impl(root, item, less);
    return find(item);
  }

  // EFFECTS: Returns a human-readable string representation of this
  //          BinarySearchTree. Works best for small trees.
  //
  // NOTE: This member function is implemented for you in TreePrint.hpp.
  //       You may use it, but you don't need to worry about how it works.
  std::string to_string() const;


private:

  // DATA REPRESENTATION
  // The root node of this BinarySearchTree.
  Node *root;

  // An instance of the Compare type. Use this to compare elements.
  Compare less;

    
  // NOTE: These member types are implemented for you in TreePrint.hpp.
  //       They support the to_string function. You do not have to do
  //       anything with them. DO NOT CHANGE.
  class Tree_grid_square;
  class Tree_grid;

  // NOTE: This member function is implemented for you in TreePrint.hpp.
  //       It supports the to_string function. You do not have to do
  //       anything with it. DO NOT CHANGE.
  int get_max_elt_width() const;



// ---------- DO NOT CHANGE ANYTHING IN THIS FILE ABOVE THIS LINE ----------


  // TREE IMPLEMENTATION FUNCTIONS
  // You must write an implementation for each of these static member
  // functions, which are called from the regular member functions that
  // are included in the starter code for the BinarySearchTree class.


  // EFFECTS: Returns whether the tree rooted at 'node' is empty.
  // NOTE:    This function must run in constant time.
  //          No iteration or recursion is allowed.
  static bool empty_impl(const Node *node) {
    assert(false);
  }

  // EFFECTS: Returns the size of the tree rooted at 'node', which is the
  //          total number of nodes in that tree. The size of an empty
  //          tree is 0.
  // NOTE:    This function must be tree recursive.
  static int size_impl(const Node *node) {
    assert(false);
  }

  // EFFECTS: Returns the height of the tree rooted at 'node', which is the
  //          number of nodes in the longest path from the 'node' to a leaf.
  //          The height of an empty tree is 0.
  // NOTE:    This function must be tree recursive.
  static int height_impl(const Node *node) {
    assert(false);
  }

  // EFFECTS: Creates and returns a pointer to the root of a new node structure
  //          with the same elements and EXACTLY the same structure as the
  //          tree rooted at 'node'.
  // NOTE:    This function must be tree recursive.
  static Node *copy_nodes_impl(Node *node) {
    assert(false);
  }

  // EFFECTS: Frees the memory for all nodes used in the tree rooted at 'node'.
  // NOTE:    This function must be tree recursive.
  static void destroy_nodes_impl(Node *node) {
    assert(false);
  }

  // EFFECTS : Searches the tree rooted at 'node' for an element equivalent
  //           to 'query'. If one is found, returns a pointer to the node
  //           containing it. If the tree is empty or the element is not
  //           found, returns a null pointer.
  //
  // NOTE: This function must be tail recursive.
  // HINT: Equivalence is defined according to the Compare functor
  //       associated with this instantiation of the BinarySearchTree
  //       template, NOT according to the == operator. Use the "less"
  //       parameter to compare elements.
  //       Two elements A and B are equivalent if and only if A is
  //       not less than B and B is not less than A.
  static Node * find_impl(Node *node, const T &query, Compare less) {
    assert(false);
  }

  // REQUIRES: item is not already contained in the tree rooted at 'node'
  // MODIFIES: the tree rooted at 'node'
  // EFFECTS : If 'node' represents an empty tree, allocates a new
  //           Node to represent a single-element tree with 'item' as
  //           its only element and returns a pointer to the new Node.
  //           If the tree rooted at 'node' is not empty, inserts
  //           'item' into the proper location as a leaf in the
  //           existing tree structure according to the sorting
  //           invariant and returns the original parameter 'node'.
  // NOTE: This function must be linear recursive, but does not
  //       need to be tail recursive.
  // HINT: Element ordering is defined according to the Compare functor
  //       associated with this instantiation of the BinarySearchTree
  //       template, NOT according to the < operator. Use the "less"
  //       parameter to compare elements.
  static Node * insert_impl(Node *node, const T &item, Compare less) {
    assert(false);
  }

  // EFFECTS : Returns a pointer to the Node containing the minimum element
  //           in the tree rooted at 'node' or a null pointer if the tree is empty.
  // NOTE: This function must be tail recursive.
  // NOTE: This function is used in the implementation of the ++ operator for
  //       the iterator code that is provided for you.
  // HINT: You don't need to compare any elements! Think about the
  //       structure, and where the smallest element lives.
  static Node * min_element_impl(Node *node) {
    assert(false);
  }

  // EFFECTS : Returns a pointer to the Node containing the maximum element
  //           in the tree rooted at 'node' or a null pointer if the tree is empty.
  // NOTE: This function must be tail recursive.
  // HINT: You don't need to compare any elements! Think about the
  //       structure, and where the largest element lives.
  static Node * max_element_impl(Node *node) {
    assert(false);
  }


  // EFFECTS: Returns whether the sorting invariant holds on the tree
  //          rooted at 'node'.
  // NOTE:    This function must be tree recursive.
  static bool check_sorting_invariant_impl(const Node *node, Compare less) {
    assert(false);
  }

  // EFFECTS : Traverses the tree rooted at 'node' using an in-order traversal,
  //           printing each element to os in turn. Each element is followed
  //           by a space (there will be an "extra" space at the end).
  //           If the tree is empty, nothing is printed.
  // MODIFIES: os
  // NOTE: This function must be tree recursive.
  //       See https://en.wikipedia.org/wiki/Tree_traversal#In-order
  //       for the definition of a in-order traversal.
  static void traverse_inorder_impl(const Node *node, std::ostream &os) {
    assert(false);
  }

  // EFFECTS : Traverses the tree rooted at 'node' using a pre-order traversal,
  //           printing each element to os in turn. Each element is followed
  //           by a space (there will be an "extra" space at the end).
  //           If the tree is empty, nothing is printed.
  // MODIFIES: os
  // NOTE: This function must be tree recursive.
  //       See https://en.wikipedia.org/wiki/Tree_traversal#Pre-order
  //       for the definition of a pre-order traversal.
  static void traverse_preorder_impl(const Node *node, std::ostream &os) {
    assert(false);
  }

  // EFFECTS : Returns a pointer to the Node containing the smallest element
  //           in the tree rooted at 'node' that is greater than 'val'.
  //           Returns a null pointer if the tree is empty or if it does not
  //           contain any elements that are greater than 'val'.
  //
  // NOTE: This function must be linear recursive.
  // NOTE: This function is used in the implementation of the ++ operator for
  //       the iterator code that is provided for you.
  // HINT: At each step, compare 'val' the the current node (using the
  //       'less' parameter). Based on the result, you gain some information
  //       about where the element you're looking for could be.
  static Node * min_greater_than_impl(Node *node, const T &val, Compare less) {
    assert(false);
  }


}; // END of BinarySearchTree class

#include "TreePrint.hpp" // DO NOT REMOVE!!!

// MODIFIES: os
// EFFECTS : Prints the elements in the tree to the given ostream,
//           separated by a space. The elements are printed using an
//           in-order traversal, and an initial "[" and trailing "]"
//           are printed before the first and after the last element.
//           Does not print a newline. Returns os.
// EXAMPLES: [ ]
//           [ 5 ]
//           [ 3 5 7 ]
// NOTE:     The correct operation of this function depends on the
//           BinarySearchTree Iterator, which in turn depends on some
//           of the functions you must write.

template <typename T, typename Compare>
std::ostream &operator<<(std::ostream &os,
                         const BinarySearchTree<T, Compare> &tree) {
// DO NOT CHANGE THE IMPLEMENTATION OF THIS FUNCTION
  os << "[ ";
  for (T& elt : tree) {
    os << elt << " ";
  }
  return os << "]";
}

#endif // DO NOT REMOVE!!
