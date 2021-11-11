#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

using namespace std;

const int N=5;
typedef vector<vector<int>> Array2d;
vector<vector<int> > C(N, vector<int>(N, 0));

void ordi_mult(Array2d A,Array2d B){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(int k=0;k<N;k++){
            C[i][j]+=A[i][k]*B[k][j];
            }
        }
    }
}

int main(){
    ofstream os;
  os.open("outputb.csv", ios::app );
    Array2d A;
    for(int i=0;i<N;i++){
        vector<int> row;
        for(int j=0;j<N;j++){
            row.push_back(i*N+j+1);
        }
        A.push_back(row);
    }
    Array2d B;
    for(int i=0;i<N;i++){
        vector<int> row;
        for(int j=0;j<N;j++){
            row.push_back(i*N+j+1);
        }
        B.push_back(row);
    }

    //   for(int i=0;i<4;i++){
      auto start = std::chrono::steady_clock::now();
        ordi_mult(A,B);

   auto end = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsed_seconds = end - start;
      std::cout  << elapsed_seconds.count() << "s\n";
      os << elapsed_seconds.count()<<N << endl;

//   }

    for(auto i : C){
        for(auto j : i){
            cout<<j<<" ";
        }
        cout<<endl;
    }
    return 0;
}