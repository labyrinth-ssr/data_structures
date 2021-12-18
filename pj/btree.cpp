#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

const int T =
    3; //(most 2T-1 keys ,least T-1 key ,most 2T children,least Tchildren)
using namespace std;
typedef pair<char, int> Record;
typedef pair<string, Record> keyValue;

class PageNode {
private:
public:
  int n; // size of stored keys
  keyValue keys[2 * T - 1];
  PageNode *c[2 * T]; // childrens
  bool is_leaf;
  PageNode(/* args */);
  ~PageNode();
};

PageNode::PageNode():n(0),is_leaf(true) {
  keys[0].first="";
  
}

PageNode::~PageNode() {}

typedef struct BTree {
  PageNode *root = new PageNode;
  PageNode *NIL = new PageNode;
} BTree;

BTree tree;

void disk_read(PageNode *) {}

void disk_write(PageNode *) {}

keyValue b_tree_search(PageNode *x, string k) {
  int i = 0;
  while (i < x->n && k > (x->keys[i]).first) {
    i++;
  }
  if (i < x->n && k == x->keys[i].first) {
    return x->keys[i];
  } else if (x->is_leaf) {
    cout<<"key "<<k<<" missing"<<endl;
    return tree.NIL->keys[0];
  } else {
    disk_read(x->c[i]);
    return b_tree_search(x->c[i], k);
  }
}


// void b_tree_create() {
//   auto x = new PageNode;
//   x->is_leaf = true;
//   x->n = 0;
//   disk_write(x);
//   tree.root = x;
// }
void b_delete(PageNode*x ,string k);

string b_tree_split_child(PageNode *x, int i) { // y is now full,with 2T-1 keys,
  auto z = new PageNode;   // former:1 to t-1(t-1),mid:t,latter:t+1 to 2T-1(t-1)
  auto y = x->c[i];        // in fact num: 0 to t-2 , t-1, t to 2t-2
  z->is_leaf = y->is_leaf; // children:0 to t-1 ,t to 2t-1
  z->n = T - 1;
  for (int j = 0; j < T - 1; j++) {
    z->keys[j] = y->keys[j + T];
  }
  if (!y->is_leaf) {
    for (int j = 0; j < T; j++) {
      z->c[j] = y->c[j + T];
    }
  }
  y->n = T - 1;
  for (int j = x->n; j > i; j--) {
    x->c[j + 1] = x->c[j];
  }
  x->c[i + 1] = z;
  for (int j = x->n - 1; j >= i; j--) {
    x->keys[j + 1] = x->keys[j];
  }
  x->keys[i] = y->keys[T - 1];
  x->n++;
  disk_write(y);
  disk_write(z);
  disk_write(x);
  return x->keys[i].first;
}

void b_tree_insert_nonfull(PageNode *x, keyValue k) {
  int i = x->n - 1;
  if (x->is_leaf) {
    while (i >= 0 && k.first < x->keys[i].first) {
      x->keys[i + 1] = x->keys[i];
      i--;
    }
    if (k.first == x->keys[i].first) {
      cout << "key " + k.first + " conflict" << endl;
      return;
    }

    x->keys[i + 1] = k;
    x->n++;
    disk_write(x);
  } else {
    while (i >= 0 && k.first < x->keys[i].first) {
      i--;
    }
    if (k.first == x->keys[i].first) {
      cout << "key " << k.first << " conflict" << endl;
      return;
    }
    i++;
    disk_read(x->c[i]);
    if (x->c[i]->n == 2 * T - 1) {
      b_tree_split_child(x, i);
      if (k > x->keys[i]) // keyi<original keyi
      {
        i++;
      }
      if (k.first == x->keys[i].first) {
      cout << "key " << k.first << " conflict" << endl;
      return;
    }
    }
    b_tree_insert_nonfull(x->c[i], k);
  }
}

void b_tree_insert(keyValue k) {
  auto r = tree.root;
  if (r->n == 2 * T - 1) {
    auto s = new PageNode;
    tree.root = s;
    s->is_leaf = false;
    s->n = 0;
    s->c[0] = r;
    b_tree_insert_nonfull(s, k);
  } else {
    b_tree_insert_nonfull(r, k);
  }
}

keyValue freq_list_line_input(string line) {
  keyValue ret;
  string ele;
  istringstream readstr(line);
  getline(readstr, ele, ' ');
  ret.first = ele;
  getline(readstr, ele, ' ');
  ret.second.first = ele[0];
  getline(readstr, ele, ' ');
  ret.second.second = stoi(ele);
  return ret;
}

void b_dump(PageNode *x) {
  if (x->is_leaf) {
    for (int i = 0; i < x->n; i++) {
    cout << x->keys[i].first << " -- (" << x->keys[i].second.first << "," << x->keys[i].second.second << ")\n";
    }
    return;
  }
  for (int i = 0; i < x->n; i++) {
    b_dump(x->c[i]);
    cout << x->keys[i].first << " -- (" << x->keys[i].second.first << "," << x->keys[i].second.second << ")\n";

  }
  b_dump(x->c[x->n]);
  return;
}

void b_insert_by_file(string fname) {
  // b_tree_create();
  ifstream fp(fname);
  string line;
  getline(fp, line);
  int i = 0;

  while (getline(fp, line)) {
    auto content = freq_list_line_input(line);
    b_tree_insert(content);
    // b_dump(tree.root);
    // cout<<endl;
    i++;
  }
}

void delete_leaf_key(PageNode *x, int i) {
  // if (x->n<=T-1)
  // {
  //   return;
  // }
  
  for (int j = i; j < x->n - 1; j++) {
    x->keys[j] = x->keys[j + 1];
  }
  x->n--;
  
}

void delete_internal_key(PageNode *x, int i) {
  if (i==x->n-1)
  {
    x->n--;
    return;
  }
  for (int j = i; j < x->n - 1; j++) {
    x->keys[j] = x->keys[j + 1];
  }
  for (int j = i; j < x->n; j++) {
    x->c[j] = x->c[j + 1];
  }
  x->n--;
  
}

PageNode* b_merge(PageNode *x, int i,string key) {//x is parent
  // t-1 + t-1 + 1
  auto resNode = x->c[i];
  auto mergeNode = x->c[i + 1];
  resNode->keys[resNode->n] = x->keys[i];
  for (int j = T; j < 2 * T - 1; j++) {//resN
    resNode->keys[j] = mergeNode->keys[j - T];
  }
  for (int j = T; j < 2 * T; j++) {
    resNode->c[j] = mergeNode->c[j - T];
  }
  resNode->n += 2;


  for (int j = i; j < x->n - 1; j++) {
    x->keys[j] = x->keys[j + 1];
  }
  for (int j = i+1; j < x->n; j++) {
    x->c[j] = x->c[j + 1];
  }
  x->n--;
  if (x->n==0)
  {
    tree.root=resNode;
  }
  b_delete(resNode, key);
  delete mergeNode;


  return resNode;
}

PageNode *b_min_node(PageNode *x) {
  while (!x->is_leaf) {
    x = x->c[x->n];
  }
  return x;
}

PageNode *b_max_node(PageNode *x) {
  while (!x->is_leaf) {
    x = x->c[0];
  }
  return x;
}

PageNode* sibling_borrow(PageNode* x,int i,bool right_sibiling){//all the i(s) are true index
//i inplies the middle parent

  if(right_sibiling){
  auto mainNode=x->c[i];
    auto sib=x->c[i+1];
  auto bor_child=sib->c[0];
  mainNode->keys[T-1]=x->keys[i];
  mainNode->c[T]=bor_child;
  x->keys[i]=sib->keys[0];
  delete_internal_key(sib,0);
  mainNode->n++;
  return mainNode;

  }
  else{
    auto mainNode=x->c[i+1];
    auto sib=x->c[i];
  auto bor_child=sib->c[sib->n];//共n个key，n+1个child
  for (int j=mainNode->n;j>0;j--)//+1
  {
    mainNode->keys[j]=mainNode->keys[j-1];
  }
  mainNode->keys[0]=x->keys[i];
  for (int j=mainNode->n+1;j>0;j--)
  {
    mainNode->c[j]=mainNode->c[j-1];
  }
  mainNode->c[0]=bor_child;

  x->keys[i]=sib->keys[sib->n-1];
  delete_internal_key(sib,sib->n-1);
  mainNode->n++;
  return mainNode;

  }

}

void b_delete(PageNode *x, string key) {
  // auto key = k.first;
  int i = 0;

  // while (1) {
    // i=0;
    bool in_node = false;
    while (i<x->n&& key > x->keys[i].first) {
      i++;
    }
    if (i<x->n&&key == x->keys[i].first) {
      in_node = true;
    }
    if (in_node && x->is_leaf) {
      // case 1
      delete_leaf_key(x, i);
    } else if (in_node) {
      auto minNode = b_min_node(x->c[i]);
      auto maxNode = b_max_node(x->c[i + 1]);
      //case 2a
      if (x->c[i]->n > T - 1)
      {
        x->keys[i] = minNode->keys[minNode->n - 1];
        b_delete(x->c[i],minNode->keys[minNode->n - 1].first);
      }
      else if(x->c[i+1]->n > T - 1){
        x->keys[i] = maxNode->keys[0];
        b_delete(x->c[i+1],maxNode->keys[0].first);
      }
      else {
        x=b_merge(x, i,key);
      }

    }
    else if (x->is_leaf)
    {
      cout<<"key "<<key<<" missing"<<endl;
      return;
    }
     else if (x->c[i]->n>T-1)
    {
      b_delete(x->c[i],key);
    }
     
    else /* (key > x->keys[i].first) */ {
      
      if(i>0&&x->c[i - 1]->n > T-1)
        {
          auto dest=sibling_borrow(x,i-1,false);
        b_delete(dest,key);

        }
        else if (i<x->n&&x->c[i+1]->n > T-1){
          auto dest=sibling_borrow(x,i,true);
        b_delete(dest,key);

        }
      //i=0
        else if (i<x->n&& x->c[i+1]->n == T-1)
        {
          b_merge(x,i,key);
        }else {
          b_merge(x,i-1,key);
        }
    }
  }
  // b_dump(tree.root);
// }

void b_delete_by_file(string fname) {
  ifstream fp(fname);
  string line;
  getline(fp, line);
  int i = 0;

  while (getline(fp, line)) {
    auto content = freq_list_line_input(line);
    if(b_tree_search(tree.root,content.first).first==""){
      continue;
    }
    b_delete(tree.root,content.first);
    // b_dump(tree.root);
    i++;
  }
}

void insert_by_cmd(){
    string word;
    char part;
    int freq;
  cout<<"input word to be inserted:";
  cin>>word;
  cout<<"input part of speech:";
  cin>>part;
  cout<<"input frequency:";
  cin>>freq;
  b_tree_insert(keyValue(word,Record(part,freq)));
}

void delete_by_cmd() {
  string k;
  cout<<"input word to be deleted:";
  cin>>k;
  b_delete(tree.root,k);
}

void update_by_cmd(PageNode* x){
  string k;
  char part;
  int freq;
  cout<<"input word to be updated:";
  cin>>k;
  cout<<"input part of speech:";
  cin>>part;
  cout<<"input frequency:";
  cin>>freq;
  keyValue newRecord(k,Record(part,freq));
  int i = 0;
  while (1)
  {
    while (i < x->n && k > (x->keys[i]).first) {
    i++;
  }
  if (i < x->n && k == x->keys[i].first) {
    cout <<"update "<< newRecord.first << " -- (" << newRecord.second.first << "," << newRecord.second.second << ")\n";
    x->keys[i]=newRecord;
    break;
  } else if (x->is_leaf) {
    cout <<"insert "<< newRecord.first << " -- (" << newRecord.second.first << "," << newRecord.second.second << ")\n";
    b_tree_insert(newRecord);
    break;
  } else {
    disk_read(x->c[i]);
    x=x->c[i];
    i=0;
    // cout<<"go to ci";
  }
  }
}

void search_by_cmd(){
  string k="last";
  cout<<"input word to search:";
  cin>>k;
  auto res=b_tree_search(tree.root,k);
  cout<<k<<"-("<<res.second.first<<","<<res.second.second<<")\n";
}

void init() { b_insert_by_file("init.txt"); }

int main() {
  // b_insert_by_file("insert.txt");
  // update_by_cmd(tree.root);
  
  while (1) {
    int i;
    cout << "input function:\n"
         << "1-initialization 2-delete by command "
         << "3-delete by file 4-insert by command "
         << "5-insert by file 6-update by command "
         << "7-search by command 8-dump\n";
    cin >> i;
    switch (i) {
    case 1:
      init();
      break;
    case 2:
      delete_by_cmd();
      break;
    case 3: {
      string filename;
      cout << "input file name to delete:";
      cin >> filename;
      b_delete_by_file(filename);
      break;
    }
    case 4:
      insert_by_cmd();
      break;
    case 5: {
      string filename;
      cout << "input file name to insert:";
      cin >> filename;
      b_insert_by_file(filename);
      break;
    }
    case 6:
      update_by_cmd(tree.root);
      break;
    case 7:
      search_by_cmd();
      break;
    case 8:
      b_dump(tree.root);
      break;
    default:
      break;
    }
  }
  return 0;
}