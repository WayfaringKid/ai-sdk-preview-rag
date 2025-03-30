/* TreePrint.hpp */

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath> // pow
#include <set>
#include <stack> // used in get_max_elt_width()

static const char* const c_leaf_branch_special = "/\\";

/*
 * A class to represent the non-whitespace squares of output using a
 * grid-based tree printing scheme.
 * Holds x and y coordinates and a value, which could be either the
 * value held by a particular tree node or one of / or \ to improve
 * readability of the printed tree.
 */
template <typename U, typename C>
class BinarySearchTree<U, C>::Tree_grid_square {
public:
  template<typename T>
  Tree_grid_square(int x_, int y_, T value_) : x(x_), y(y_) {
      std::ostringstream oss;
      oss << value_;
      value = oss.str();
  }

  // Probe ctor
  Tree_grid_square(int x_, int y_) : x(x_), y(y_) { }

  /*
   * A square is considered less than another if it is closer to the
   * root of the tree. If two squares are at the same y coordinate,
   * whichever one is farthest left is considered less.
   * This ordering is solely for the purpose of storing in a sorted
   * container.
   */
  bool operator<(const Tree_grid_square& rhs) const {
      if (y < rhs.y) {
          return true;
      }
      if (y == rhs.y) {
          return x < rhs.x;
      }
      return false;
  }

  /*
   * Sends the value held at this coordinate to the given string
   * stream.
   */
  std::string get_value() const {
      return value;
  }

  int get_x() const {
      return x;
  }

  int get_y() const {
      return y;
  }

private:
  int x;
  int y;
  std::string value;
};

/*
 * Container to build and hold a set of Tree_grid_squares.
 */
template <typename U, typename C>
class BinarySearchTree<U, C>::Tree_grid {
public:

  Tree_grid(const BinarySearchTree& tree) :
          num_levels(static_cast<int>(tree.height())), leftmost_x(0),
          rightmost_x(0) {
      build(tree.root);
  }

  /*
   * Convenience wrapper of set::find(). Returns a pointer to the
   * Tree_grid_square in the set or 0 if not found.
   */
  const Tree_grid_square* find(const Tree_grid_square& coordinate) const {
      typename std::set<Tree_grid_square>::iterator it =
              coordinates.find(coordinate);
      if (it == coordinates.end()) {
          return 0;
      }
      return &*it;
  } // find wrapper

  /*
   * Returns the number of levels in the traversed tree.
   */
  int get_num_levels() const {
      return num_levels;
  } // get_num_levels

  /*
   * Returns the leftmost x coordinate in the traversed tree.
   */
  int get_leftmost_x() const {
      return leftmost_x;
  } // get_lowest_x

  /*
   * Returns the rightmost x coordinate in the traversed tree.
   */
  int get_rightmost_x() const {
      return rightmost_x;
  }

private:
  std::set<Tree_grid_square> coordinates;
  int num_levels;
  int leftmost_x;
  int rightmost_x;

  /*
   * Given the height of a tree and the index of the current level (0
   * being the root level), returns the horizontal distance (number of
   * grid squares) between the current node and one of its two
   * children.
   *
   * Note that in order for the tree to be printed elegantly, the
   * horizontal distance between nodes must increase as the tree
   * becomes taller.
   */
  static int calculate_x_offset(int tree_height, int current_level) {
      return int(std::pow(2, tree_height) / std::pow(2, current_level + 2));
  }

  /*
   * Recursively fills the set of Node_coordinates
   */
  void build(const Node* root_node, int cur_x = 0, int cur_y = 0) {
      if (!root_node) {
          return;
      }
      coordinates.insert(Tree_grid_square(cur_x, cur_y,
                                          root_node->datum));
      if (cur_x < leftmost_x) {
          leftmost_x = cur_x;
      }
      if (cur_x > rightmost_x) {
          rightmost_x = cur_x;
      }

      int x_offset = calculate_x_offset(num_levels, cur_y / 2);

      // Slashes indicating parent-child relationships.
      int branch_x_offset = (x_offset) <= 1 ? 1 : x_offset / 2;

      int left_branch_x = cur_x - branch_x_offset;
      int left_branch_y = cur_y + 1;
      Tree_grid_square left_branch(left_branch_x, left_branch_y, "/");

      int right_branch_x = cur_x + branch_x_offset;
      int right_branch_y = cur_y + 1;
      Tree_grid_square right_branch(right_branch_x, right_branch_y, "\\");


      // Special case where leaf branches collide.
      typename std::set<Tree_grid_square>::iterator collision_iter =
              coordinates.find(left_branch);
      if (collision_iter != coordinates.end()) {
          coordinates.erase(collision_iter);
          coordinates.insert( Tree_grid_square(left_branch.get_x(),
                                               left_branch.get_y(),
                                               c_leaf_branch_special));
      } else {
          coordinates.insert(left_branch);
      }

      coordinates.insert(right_branch);

      build(root_node->left, cur_x - x_offset, cur_y + 2);
      build(root_node->right, cur_x + x_offset, cur_y + 2);
  } // build
};

//--------------------------------------------------------------------

/*
 * Returns an (actually) human-readable string representation of the
 * tree
 */
template <typename U, typename C>
std::string BinarySearchTree<U, C>::to_string() const {
    if (!root) {
        return "( )";
    }
    int node_width = get_max_elt_width();
    Tree_grid coordinates(*this);

    std::ostringstream oss;
    std::string padding(size_t(node_width), ' ');
    int farthest_left = coordinates.get_leftmost_x() - node_width;
    int farthest_right = coordinates.get_rightmost_x() + node_width;
    // Two printed lines per tree level: one for the values, one for the
    // slash characters (branches).
    for (int y = 0; y <= coordinates.get_num_levels() * 2; ++y) {
        oss << "\n";
        for (int x = farthest_left; x <= farthest_right; ++x) {
            const Tree_grid_square* tgs_ptr =
                    coordinates.find( Tree_grid_square(x, y));
            if (tgs_ptr) {
                // TODO width field in square?
                if (tgs_ptr->get_value() == "/") {
                    oss << std::right;
                    oss << std::setw(node_width);
                    oss << tgs_ptr->get_value();
                } else if (tgs_ptr->get_value() == "\\") {
                    oss << std::left;
                    oss << std::setw(node_width);
                    oss << tgs_ptr->get_value();
                } else if (tgs_ptr->get_value() == c_leaf_branch_special) {
                    oss << '\\' << std::string(size_t(node_width - 2), ' ') << '/';
                } else {
                    oss << std::setw(node_width);
                    oss << tgs_ptr->get_value();
                }
            } else {
                oss << padding;
            }
        } // for x
        // oss << "\n";
    } // for y
    return oss.str();
} // to_string

static const int c_min_elt_width = 2;

/*
 * Returns the width of the widest elt in this tree.
 */
template <typename U, typename C>
int BinarySearchTree<U, C>::get_max_elt_width() const {
    int current_max = c_min_elt_width;
    std::stack<Node*> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
        Node* current = nodes.top();
        nodes.pop();
        if (!current) {
            continue;
        }
        std::ostringstream oss;
        oss << current->datum;
        int width = int(oss.str().length());
        if (width > current_max) {
            current_max = width;
        }
        nodes.push(current->left);
        nodes.push(current->right);
    } // while
    return current_max;
} // get_max_elt_width
