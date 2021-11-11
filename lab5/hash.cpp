#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum Gender { MALE, FEMALE };
enum Status { EMPTY, VALUED, DELETED };

int cnt_search=0;
int cnt=0;

typedef unsigned long long int ULLI;
typedef struct Student {
  Status status = EMPTY;
  ULLI key;
  int num;
  Gender gender;
  int age = 0;
  long long int phone_num;
} Student;
const int w = 48;
const int r = 8;
const int m = pow(2, r);
const double A = (sqrt(5) - 1) / 2;
ULLI s = A * pow(2.0, w);

Student hashTable[256];

int hash1(ULLI k) {
  auto res = ((s * k) % (ULLI)pow(2, w)) >> (w - r); 
  return res;
}
int hash2(ULLI k) {
  return hash1(k) / 2 * 2 + 1; 
}
int hash_(ULLI k, int i) {
  return (hash1(k) + i * hash2(k)) % m;
}
int hash_insert(ULLI k) {
  int i = 0;
  int res;
  while (i < m) {
    res = hash_(k, i);
    if (hashTable[res].status == EMPTY || hashTable[res].key == DELETED) {
      hashTable[res].status = VALUED;
      return res;
    }
    i++;
  }
  throw "hash table overflow"; 
}
int hash_search(ULLI k) {
  int i = 0;
  int res;
  while (i < m) {
    res = hash_(k, i);
    if (hashTable[res].status == VALUED && hashTable[res].key == k){
        cnt++;
        cnt_search+=i+1;
        cout<<i<<endl;
      return res;
    }
    else if (hashTable[res].status == EMPTY) {
      cout << "no such content" << endl;
      return -1;
    }
    i++;
  }
  cout << "no such content" << endl;
  return -1;
}
void hash_delete(ULLI k) {
  auto res = hash_search(k);
  if (res == -1) {
    cout << "content doesn't exist" << endl;
    return;
  }
  hashTable[res].status = DELETED;
}

ULLI gbk3_to_ulli(string in) {
  bitset<48> res;
  for (int i = 0; i < in.length(); i += 2) {
    res <<= 16;
    auto temp = (bitset<48>(in[i]) & bitset<48>(0x0000ff)) << 8;
    res |= temp | (bitset<48>(in[i + 1]) & bitset<48>(0x0000ff));
  }
  return res.to_ullong();
}

void test_search_all(){
    ifstream fp("data(gbk).csv");
    string line;
    getline(fp,line);
    while(getline(fp,line)){
        string name;
        istringstream readline(line);
        getline(readline,name,',');
        auto key=gbk3_to_ulli(name);
        auto loc=hash_search(key);
    }
}

int main() {
  ifstream fp(
      "data(gbk).csv"); 
  string line;
  getline(fp, line); 
  while (getline(fp, line)) { 
    string ele;
    istringstream readstr(line); 
    getline(readstr, ele, ',');  
    auto key = gbk3_to_ulli(ele);
    int loc;
    try {
      loc = hash_insert(key);
    } catch (const char *msg) {
      std::cerr << msg << '\n';
    }
    hashTable[loc].key = key;
    getline(readstr, ele, ','); 
    hashTable[loc].num = stoi(ele);
    getline(readstr, ele, ','); 
    if (ele == "ÄĞ")
      hashTable[loc].gender = MALE;
    else if (ele == "Å®")
      hashTable[loc].gender = FEMALE;
    getline(readstr, ele, ','); 
    hashTable[loc].age = stoi(ele);
    getline(readstr, ele); 
    hashTable[loc].phone_num = stoll(ele);
    // cout << loc << " ";
  }
  test_search_all();
  cout<<cnt_search<<" "<<cnt<<" "<<(double)cnt_search/(double)cnt<<endl;
  return 0;
}