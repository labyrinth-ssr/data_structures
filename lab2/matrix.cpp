#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>

using namespace std;
template <class T>

void make2dArray(T **&x, int size) {
  x = new T *[size];
  for (int i = 0; i < size; i++) {
    x[i] = new T[size];
    memset(x[i], 0, size * sizeof(T));
  }
}

typedef pair<int, int> Pos;
typedef int **Int2d;

class Matrix {
private:
public:
  Pos startPos;
  int size;
  Int2d content;
  Matrix();
  Matrix(int);
  Matrix(int isize, Pos istartPos, Int2d icontent);
  Matrix **divide();
  int get_ele(Pos);
  void set_ele(Pos, int);
  void print_matrix();
  Matrix operator+(const Matrix &b) {
    int size = b.size;
    Int2d nContent;
    make2dArray(nContent,size);
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        nContent[i][j]=this->content[startPos.first + i][startPos.second + j]+b.content[b.startPos.first + i][b.startPos.second + j];
      }
    }
    Matrix res(size, Pos(0, 0), nContent);
    return res;
  }

  Matrix operator-(const Matrix &b) {
    int size = b.size;
    Int2d nContent;
    make2dArray(nContent,size);
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        nContent[i][j]=this->content[startPos.first + i][startPos.second + j]-b.content[b.startPos.first + i][b.startPos.second + j];
      }
    }
    Matrix res(size, Pos(0, 0), nContent);
    return res;
  }
};
Matrix::Matrix():size(0),startPos(Pos(0,0)),content(NULL) {
}

Matrix::Matrix(int isize, Pos istartPos, Int2d icontent)
    : content(icontent), size(isize), startPos(istartPos) {}

Matrix **Matrix::divide() {
  int nsize = size / 2;
  Matrix **res;
  make2dArray(res, 4);
  res[0] = new Matrix(nsize, startPos, content);
  res[1] =
      new Matrix(nsize, Pos(startPos.first, startPos.second + nsize), content);
  res[2] =
      new Matrix(nsize, Pos(startPos.first + nsize, startPos.second), content);
  res[3] = new Matrix(
      nsize, Pos(startPos.first + nsize, startPos.second + nsize), content);
  return res;
}

void Matrix::print_matrix() {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      cout << content[startPos.first + i][startPos.second + j] << " ";
    }
    cout << endl;
  }
}

inline int Matrix::get_ele(Pos p) {
  return content[startPos.first + p.first][startPos.second + p.second];
}

inline void Matrix::set_ele(Pos p, int i) {
  content[startPos.first + p.first][startPos.second + p.second] = i;
}

Matrix::Matrix(int isize) : size(isize), startPos(Pos(0, 0)) {
  make2dArray(content,isize);
  // this->print_matrix();
}

void multiply(Matrix A, Matrix B, Matrix *C) {
  int n = A.size;
  int nsize = n / 2;
  if (n == 1) {
    C->set_ele(Pos(0, 0), A.get_ele(Pos(0, 0)) * B.get_ele(Pos(0, 0)));
    // cout<<"C:";
    // C->print_matrix();
    return;
  } else {
    Matrix** subA = A.divide();
    Matrix** subB = B.divide();
    Matrix** subC = C->divide();

    Matrix *P[8];
    for (int i = 1; i <= 7; i++) {
      P[i] = new Matrix(nsize);
    }

    multiply(*subA[0], *subB[1] - *subB[3], P[1]);
    multiply(*subA[0] + *subA[1], *subB[3], P[2]);
    multiply(*subA[2] + *subA[3], *subB[0], P[3]);
    multiply(*subA[3], *subB[2] - *subB[0], P[4]);
    multiply(*subA[0] + *subA[3], *subB[0] + *subB[3], P[5]);
    multiply(*subA[1] - *subA[3], *subB[2] + *subB[3], P[6]);
    multiply(*subA[0] - *subA[2], *subB[0] + *subB[1], P[7]);

    for (int i = 0; i < nsize; i++) {
      for (int j = 0; j < nsize; j++) {
        auto con = P[5]->get_ele(Pos(i, j)) + P[4]->get_ele(Pos(i, j)) -
                   P[2]->get_ele(Pos(i, j)) + P[6]->get_ele(Pos(i, j));
        subC[0]->set_ele(Pos(i, j), con);
      }
    }
    for (int i = 0; i < nsize; i++) {
      for (int j = 0; j < nsize; j++) {
        auto con = P[1]->get_ele(Pos(i, j)) + P[2]->get_ele(Pos(i, j));

        subC[1]->set_ele(Pos(i, j), con);
      }
    }
    for (int i = 0; i < nsize; i++) {
      for (int j = 0; j < nsize; j++) {
        auto con = P[3]->get_ele(Pos(i, j)) + P[4]->get_ele(Pos(i, j));

        (*subC[2]).set_ele(Pos(i, j), con);
      }
    }
    for (int i = 0; i < nsize; i++) {
      for (int j = 0; j < nsize; j++) {
        auto con = P[5]->get_ele(Pos(i, j)) + P[1]->get_ele(Pos(i, j)) -
                   P[3]->get_ele(Pos(i, j)) - P[7]->get_ele(Pos(i, j));

        (*subC[3]).set_ele(Pos(i, j), con);
      }
    }
    for (int i = 1; i < 8; i++) {
      delete P[i];
    }
    // cout<<"C:";
    // C->print_matrix();
  }
}

int main() {
  ofstream os;
  os.open("outputa.csv", ios::out | ios::trunc);
  os << "index"
     << ","
     << "time"<<"," <<"N"<< endl;
  for(int i=0;i<4;i++){
    int N=pow(2,i+4);
  
  // vector input
  Int2d a,b,c,d;
  make2dArray(a,N);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      a[i][j]=i*N+j+1;
    }
  }
  make2dArray(c,N);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      c[i][j]=i*N+j+1;
    }
  }

  //并非2^n长度时padding
  
  int cnt=0;
  int length=N;
  bool flag=true;
  while(length>0){
    if(length % 2 != 0){
      flag=false;
    }
    length/=2;
    cnt++;
  }
  cout<<"cnt:"<<cnt<<endl;
  
  int paddingN=pow(2,cnt);

  make2dArray(b,paddingN);
  make2dArray(d,paddingN);
  for(int i=0;i<paddingN;i++){
    for(int j=0;j<paddingN;j++){
      if(i<N&&j<N){
        b[i][j]=a[i][j];
        d[i][j]=c[i][j];
      }
      else{
        b[i][j]=0;
        d[i][j]=0;
      }
    }
  }
  cout<<"padding:"<<paddingN;
  int paddingN=N;

  Matrix A(paddingN, Pos(0, 0), b);
  Matrix B(paddingN, Pos(0, 0), d);
  Matrix *C = new Matrix(paddingN);

  for (int i = 0; i < 4; i++) {
    auto start = std::chrono::steady_clock::now();
    multiply(A, B, C);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << elapsed_seconds.count() << "s\n";
    os <<i<<","<< elapsed_seconds.count() <<","<<N<< endl;
  }
  // Int2d output = C->content;

  //   for(int i=0;i<paddingN;i++){
  //     for(int j=0;j<paddingN;j++){
  //       cout<<output[i][j]<<" ";
  //     }
  //     cout<<endl;
  //   }

  }
  
  return 0;
}