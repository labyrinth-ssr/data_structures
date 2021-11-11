#include <iostream>
#include <limits.h>
#include <algorithm>
//underflow 咋写啊...

using namespace std;
const int D = 2;
const int N = 50;
int heapSize= 10;

int Heap[N]={16,14,10,8,7,9,3,2,4,1};

void max_heapify(int i){
    // int max=Heap[i];
    // int maxChild=Heap[i];
    int maxChildIndex=i;
    for(int j=i*D+1;j<=i*D+D&&j<heapSize;j++){
        if(Heap[j]>Heap[maxChildIndex]){
            maxChildIndex=j;
        }
    }
    if(maxChildIndex!=i){
        int temp=Heap[i];
        Heap[i]=Heap[maxChildIndex];
        Heap[maxChildIndex]=temp;
        max_heapify(maxChildIndex);
    }
}

int extract_max(){
    if(heapSize<=0){
        cout<<"heap underflow"<<endl;
    }
    int max=Heap[0];
    Heap[0]=  Heap[heapSize-1];
    heapSize-=1;
    max_heapify(0);
    return max;
}

void print_heap(){
    for(int i=0;i<heapSize;i++){
        cout<<Heap[i]<<" ";
    }
}
// void print_tree(){
//     ;
// }
inline int parent(int i){
    return (i-1)/D;
}
void increase_key(int i,int k){        
    Heap[i]=max(k,Heap[i]);
    while(i>0&&Heap[parent(i)]<Heap[i]){
            int temp=Heap[parent(i)];
            Heap[parent(i)]=Heap[i];
            Heap[i]=temp;
            i=parent(i);
    }
}

void insert(int key){
    heapSize++;
    Heap[heapSize-1]=INT_MIN;
    increase_key(heapSize-1,key);
}

int main(){
    increase_key(8,15);
    print_heap();
    return 0;
}