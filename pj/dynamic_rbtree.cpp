#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
// const int SIZE = 3000;

typedef enum { RED, BLACK } Color;
typedef pair<char, int> Record;
typedef pair<string, Record> keyValue;
const int NTEST = 15;

class Node {
public:
  Color nodeColor;
  int index;
  string key;
  // Type type;
  Node *left;
  Node *right;
  Node *p;
  Record val; //考虑到record 比较小
  Node();
  Node(keyValue, int);
  ~Node();
  inline bool is_NIL();
};

bool Node::is_NIL() { return this->key.empty(); }

Node::Node() : nodeColor(BLACK), index(-1) {}

Node::Node(keyValue in, int i)
    : nodeColor(BLACK), key(in.first), val(in.second), index(i) {}

Node::~Node() {}

typedef struct RBTree {
  Node *root = new Node;
  Node *NIL = new Node;
} RBTree;

RBTree tree;

void dump(Node *x) {
  if (x->is_NIL()) {
    // cout<<"return"<<endl;
  }
  if (!x->is_NIL()) {
    // cout<<"enter"<<endl;
    dump(x->left);
    cout << x->key << " -- (" << x->val.first << "," << x->val.second << ")"
         << endl;
    // cout<<x->index<<" ";
    dump(x->right);
  }
}

void left_rotate(Node *w) {
  // w is root ,make it be its right child's left child.
  // first,assign y,assign y's parent.assign w's right child.assign y's left
  // child. assign w's parent.
  auto y = w->right; // reset w's right child's left child's parent.
  w->right = y->left;
  y->left->p = w;
  y->p = w->p;
  if (w->p->is_NIL()) { // root is w itself,then changed to y.
    tree.root = y;
  } else if (w == w->p->left) { // since w's parent relation is unknown.
    w->p->left = y;
  } else {
    w->p->right = y;
  }
  y->left = w;
  w->p = y;
}

void right_rotate(Node *x) {
  // x is root ,make it be its left child's right child.
  // first,assign y,assign y's parent.assign x's left child.assign y's right
  // child. assign x's parent.
  auto y = x->left; // reset x's left child's right child's parent.
  x->left = y->right;
  y->right->p = x;
  y->p = x->p;
  if (x->p->is_NIL()) { // root is x itself,then changed to y.
    tree.root = y;
  } else if (x == x->p->right) { // since x's parent relation is unknown.
    x->p->right = y;
  } else {
    x->p->left = y;
  }
  y->right = x;
  x->p = y;
}

void rb_insert_fixup(Node *z) {
  while (z->p->nodeColor == RED) {
    // cout << "parent red" << endl;
    if (z->p == z->p->p->left) {
      auto y = z->p->p->right; // uncle
      if (y->nodeColor == RED) {
        z->p->nodeColor = BLACK;
        y->nodeColor = BLACK;
        z->p->p->nodeColor = RED;
        // cout << "p black,uncle black,grand red" << endl;
        z = z->p->p;
      } else {
        if (z == z->p->right) {
          z = z->p;
          // cout << "left rotate" << endl;
          left_rotate(z);
        }
        z->p->nodeColor = BLACK;
        z->p->p->nodeColor = RED;
        // cout << "right rotate ,p black, grand red" << endl;
        right_rotate(z->p->p);
      }
    } else {
      auto y = z->p->p->left; // uncle
      if (y->nodeColor == RED) {
        z->p->nodeColor = BLACK;
        y->nodeColor = BLACK;
        z->p->p->nodeColor = RED;
        z = z->p->p;
        // cout << "p black,uncle black,grand red" << endl;
      } else {
        if (z == z->p->left) {
          z = z->p;

          // cout << "right rotate ";
          right_rotate(z);
        }
        z->p->nodeColor = BLACK;
        z->p->p->nodeColor = RED;
        // cout << "left rotate ,p black, grand red" << endl;
        left_rotate(z->p->p);
      }
    }
  }
  tree.root->nodeColor = BLACK;
}

void rb_insert(Node *z) {
  auto y = tree.NIL;  // null initialize? y :to store the parent
  auto x = tree.root; // from root
  while (!x->is_NIL()) {
    y = x;
    if (z->key == x->key) {
        
      throw("key "+x->key+" conflict");
    }
    if (z->key == x->key)
    {
              return;

    }
    
    if (z->key < x->key) {
      x = x->left;
      // cout << "x left ";
    } else {
      x = x->right;
      // cout << "x right "; // in fact,i can know the final destination
    }
  }                // to the empty node , with left or right unchosen
  z->p = y;        // y is not empty
  if (y->is_NIL()) // means that no former nodes
  {
    tree.root = z;
  } else if (z->key < y->key) {
    delete y->left;
    y->left = z;
    // cout << "y left ";
  } else {
    delete y->right;
    y->right = z;
    // cout << "y right ";
  }
  z->left = new Node;
  z->right = new Node;

  z->nodeColor = RED;
  rb_insert_fixup(z);
}

void rb_transplant(Node *u, Node *v) { // only responsible for parent relation
  if (u->p->is_NIL()) {
    tree.root = v;
  } else if (u == u->p->left) {
    u->p->left = v;
  } else {
    u->p->right = v;
  }
  v->p = u->p;
}

Node *tree_minimum(Node *x) {
  while (!x->left->is_NIL()) {
    x = x->left;
  }
  return x;
}

void rb_delete_fixup(Node *x) {
  while (x != tree.root && x->nodeColor == BLACK) {
    Node *w = NULL;
    if (x == x->p->left) {
      w = x->p->right;
      if (w->nodeColor == RED) {
        w->nodeColor = BLACK;
        x->p->nodeColor = RED;
        left_rotate(x->p);
        w = x->p->right;
      }
      if (w->left->nodeColor == BLACK && w->right->nodeColor == BLACK) {
        w->nodeColor = RED;
        x = x->p;
      } else {
        if (w->right->nodeColor == BLACK) {
          w->left->nodeColor = BLACK;
          w->nodeColor = RED;
          right_rotate(w);
          w = x->p->right;
        }
        w->nodeColor = x->p->nodeColor;
        x->p->nodeColor = BLACK;
        w->right->nodeColor = BLACK;
        left_rotate(x->p);
        x = tree.root;
      }
    } else {
      w = x->p->left;
      if (w->nodeColor == RED) {
        w->nodeColor = BLACK;
        x->p->nodeColor = RED;
        right_rotate(x->p);
        w = x->p->left;
      }
      if (w->right->nodeColor == BLACK && w->left->nodeColor == BLACK) {
        w->nodeColor = RED;
        x = x->p;
      } else {
        if (w->left->nodeColor == BLACK) {
          w->right->nodeColor = BLACK;
          w->nodeColor = RED;
          left_rotate(w);
          w = x->p->left;
        }
        w->nodeColor = x->p->nodeColor;
        x->p->nodeColor = BLACK;
        w->left->nodeColor = BLACK;
        right_rotate(x->p);
        x = tree.root;
      }
    }
  }
  x->nodeColor = BLACK;
}

void rb_delete(Node *z) {
  auto zCopy = z;
  auto y = z;
  auto yOriginalColor = y->nodeColor;
  Node *x = NULL;
  if (z->left->is_NIL()) {
    x = z->right;
    rb_transplant(z, z->right);
  } else if (z->right->is_NIL()) {
    x = z->left;
    rb_transplant(z, z->left);
  } else {
    y = tree_minimum(z->right);
    yOriginalColor = y->nodeColor;
    x = y->right;
    if (y->p == z) {
      x->p = y;
    } else {
      rb_transplant(y, y->right);
      y->right = z->right;
      y->right->p = y;
    }
    rb_transplant(z, y);
    y->left = z->left;
    y->left->p = y;
    y->nodeColor = z->nodeColor;
  }
  if (yOriginalColor == BLACK) {
    rb_delete_fixup(x);
  }
  delete zCopy;
}

Node *tree_search(Node *x, string k) {
  if (x->is_NIL()) {
    throw "key " + k + " missing";
  } 
  if (x->is_NIL())
  {
      return x;
  }
  else if (k == x->key) {
    return x;
  } else if (k < x->key) {
    return tree_search(x->left, k);
  } else {
    return tree_search(x->right, k);
  }
}

keyValue freq_list_line_input(string line) {
  keyValue ret;
  string ele;
  istringstream readstr(line);
  getline(readstr, ele, ' ');
  ret.first = ele;
  // nodes[i].key = ele;
  getline(readstr, ele, ' ');
  ret.second.first = ele[0];
  // nodes[i].val.first = ele[0];
  getline(readstr, ele, ' ');
  ret.second.second = stoi(ele);
  // nodes[i].val.second = stoi(ele);
  // nodes[i].index = i;
  return ret;
}

void insert_by_file(string fname) {
  ifstream fp(fname);
  string line;
  getline(fp, line);
  int i = 0;
  while (getline(fp, line)) {
    auto content = freq_list_line_input(line);
    try {
      rb_insert(new Node(content, i));
    } catch ( string e) {
      std::cerr << e << '\n';
    }
    i++;
    // cout << i << " ";
  }
}

void init() { insert_by_file("init.txt"); }

keyValue get_record_input() {
  Record value;
  keyValue in;
  string word;
  char part;
  int freq;
  cout << "input word:";
  cin >> word;
  in.first = word;
  cout << "input part of speech:";
  cin >> part;
  value.first = part;
  cout << "input frequency:";
  cin >> freq;
  value.second = freq;
  in.second = value;
  return in;
}

void delete_by_cmd() {
  string k;
  cout << "input word to be deleted:";
  cin >> k;
  Node *res = NULL;
  try {
    res = tree_search(tree.root, k);
  } catch (string e) {
    std::cerr << e << '\n';
  }
  rb_delete(res);
}

void delete_by_file(string fname) {
  ifstream fp(fname);
  string line;
  getline(fp, line);
  while (getline(fp, line)) {
    string ele;
    istringstream readstr(line);
    getline(readstr, ele, ' ');
    Node *res = NULL;
    try {
      res = tree_search(tree.root, ele);
    } catch (string e) {
      std::cerr << e << '\n';
      continue;
    }
    rb_delete(res);
  }
}

void update_by_cmd() {
    cout<<"update by cmd"<<endl;
  auto in = get_record_input();
  Node *res = NULL;
  try {
    res = tree_search(tree.root, in.first);
  } catch (string e) {
    rb_insert(new Node(in, 3001));
    return;
  }
  res->val = in.second;
}

void insert_by_cmd() {
    cout<<"insert by cmd:"<<endl;
  auto in = get_record_input();
  try {
    rb_insert(new Node(in, 2001));
  } catch (string e) {
    std::cerr << e << '\n';
    return;
  }
}

void search_by_cmd() {
  string k;
  cout << "input word to be searched:";
  cin >> k;
  try {
    auto res = tree_search(tree.root, k);
    cout << "(" << res->val.first << "," << res->val.second << ")" << endl;
  } catch (string e) {
    std::cerr << e << '\n';
  }
}

int main() {
  insert_by_file("insert.txt");
//   search_by_cmd();
dump(tree.root);
  cout<<endl;
  cout<<"done"<<endl;
  return 0;
}