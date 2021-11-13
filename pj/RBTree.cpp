#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
const int SIZE = 3000;

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
  Record val;
  Node();
  ~Node();
};

Node::Node() : nodeColor(BLACK), index(-1) {}

Node::~Node() {}

typedef struct RBTree {
  Node *root = new Node;
} RBTree;

RBTree tree;
Node nodes[SIZE];
int dataSize = 0;

void dump(Node *x) {
  if (x->key.empty()) {
    // cout<<"return"<<endl;
  }
  if (!x->key.empty()) {
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
  if (w->p->key.empty()) { // root is w itself,then changed to y.
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
  if (x->p->key.empty()) { // root is x itself,then changed to y.
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
  auto y = new Node; // null initialize? y :to store the parent
  auto yCopy = y;
  auto x = tree.root; // from root
  while (!x->key.empty()) {
    y = x;
    delete yCopy;
    if (z->key == x->key) {
      throw("The word already exits");
      return;
    }
    if (z->key < x->key) {
      x = x->left;
      // cout << "x left ";
    } else {
      x = x->right;
      // cout << "x right "; // in fact,i can know the final destination
    }
  }                   // to the empty node , with left or right unchosen
  z->p = y;           // y is not empty
  if (y->key.empty()) // means that no former nodes
  {
    tree.root = z;
    delete yCopy;
  } else if (z->key < y->key) {
    auto yLeftCopy = y->left;
    y->left = z;
    delete yLeftCopy;
    // cout << "y left ";
  } else {
    auto yRightCopy = y->right;
    y->right = z;
    delete yRightCopy;
    // cout << "y right ";
  }
  z->left = new Node;
  z->right = new Node;
  z->nodeColor = RED;
  rb_insert_fixup(z);
}

void rb_transplant(Node *u, Node *v) { // only responsible for parent relation
  if (u->p->key.empty()) {
    tree.root = v;
  } else if (u == u->p->left) {
    u->p->left = v;
  } else {
    u->p->right = v;
  }
  v->p = u->p;
}

Node *tree_minimum(Node *x) {
  while (!x->left->key.empty()) {
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
  auto zCopy=z;
  auto y = z;
  auto yOriginalColor = y->nodeColor;
  Node *x = NULL;
  if (z->left->key.empty()) {
    x = z->right;
    rb_transplant(z, z->right);
  } else if (z->right->key.empty()) {
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
  if (zCopy>=&nodes[0]&&zCopy<=&nodes[SIZE-1])
  {
    return;
  }
  delete zCopy;
  cout<<"dynamic delete"<<endl;
}

Node *tree_search(Node *x, string k) {
  if (x->key.empty()) {
    throw "key "+x->key+" missing";
    return x;
  } else if (k == x->key) {
    return x;
  } else if (k < x->key) {
    return tree_search(x->left, k);
  } else {
    return tree_search(x->right, k);
  }
}

void init(string fname) {
  ifstream fp(fname);
  string line;
  getline(fp, line);
  int i = 0;
  while (getline(fp, line)) {
    string ele;
    istringstream readstr(line);
    getline(readstr, ele, ' ');
    nodes[i].key = ele;
    getline(readstr, ele, ' ');
    nodes[i].val.first = ele[0];
    getline(readstr, ele, ' ');
    nodes[i].val.second = stoi(ele);
    nodes[i].index = i;
    i++;
  }
  dataSize = i;
  // cout << dataSize << endl;
  for (int j = 0; j < dataSize; j++) {
    try {
      rb_insert(&nodes[j]);
    } catch (const char *e) {
      std::cerr << e << '\n';
    }
  }
}

void delete_by_cmd() {
  string k;
  cout<<"input word to be deleted:";
  cin>>k;
  Node* res=NULL;
  try
  {
    res=tree_search(tree.root, k);
  }
  catch(const char* e)
  {
    std::cerr << e << '\n';
    return;
  }
  rb_delete(res);
}

void delete_by_file(string fname){
  ifstream fp(fname);
  string line;
  getline(fp, line);
  while (getline(fp, line)) {
    string ele;
    istringstream readstr(line);
    getline(readstr, ele, ' ');
    Node* res=NULL;
    try
    {
      res=tree_search(tree.root,ele);
    }
    catch(const char* e)
  {
    std::cerr << e << '\n';
    continue;
  }
  rb_delete(res);
  }
}

void rb_update(keyValue in) {
  Node *res = NULL;
  try {
    res = tree_search(tree.root, in.first);
  } catch (const char *e) {
    auto newNode = new Node;
    newNode->key = in.first;
    newNode->val = in.second;
    rb_insert(newNode);
    return;
  }
  res->val = in.second;
}

void insert_by_cmd(){
    string word;
    char part;
    int freq;
  cout<<"input word to be inserted:";
  cin>>word;
  cout<<"input part of speech";
  cin>>part;
  cout<<"input frequency";
  cin>>freq;
  Node* res=NULL;
  auto newNode = new Node;
  newNode->key = word;
  newNode->val = Record(part,freq);
  try {
    rb_insert(newNode);
    } catch (const char *e) {
      std::cerr << e << '\n';
      return;
    }
}

int main() {
  init("delete.txt");
  dump(tree.root);
  delete_by_file("delete.txt");
  cout<<endl;
  dump(tree.root);
  cout<<"done"<<endl;
  return 0;
}