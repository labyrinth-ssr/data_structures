#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<int> insertion(vector<int> input) {
  for (int i = 1; i < input.size(); i++) {
    int save = input[i];
    int key = i - 1;
    while (key >= 0 && save < input[key]) {
      input[key + 1] = input[key];
      key--;
    }
    input[key + 1] = save;
    for (int i = 0; i < input.size(); i++) {
      // cout << input[i] << " ";
    }
    // cout << endl;
  }
  return input;
}

vector<int> merge(vector<int> n1, vector<int> n2) {
  vector<int> res;
  int i1 = 0, i2 = 0;
  while (i1 < n1.size() || i2 < n2.size()) {
    if (i1 < n1.size() && (i2 >= n2.size() || n1[i1] <= n2[i2])) {
      res.push_back(n1[i1]);
      // cout<<n1[i1]<<" ";
      i1++;
    } else if (i1 >= n1.size() || n1[i1] > n2[i2]) {
      res.push_back(n2[i2]);
      // cout<<n2[i2]<<" ";
      i2++;
    }
  }
  // cout<<endl;
  return res;
}

vector<int> merge_sort(vector<int> input) {
  if (input.size() == 1)
    return input;
  vector<int> front(input.begin(), input.begin() + input.size() / 2);
  front = merge_sort(front);
  vector<int> behind(input.begin() + input.size() / 2, input.end());
  behind = merge_sort(behind);
  return merge(front, behind);
}

vector<int> mixed_sort(int k, vector<int> input) {
  if (input.size() == 1)
    return input;
  else if (input.size() <= k) {
    return insertion(input);
  }
  vector<int> front(input.begin(), input.begin() + input.size() / 2);
  front = mixed_sort(k, front);
  vector<int> behind(input.begin() + input.size() / 2, input.end());
  behind = mixed_sort(k, behind);
  return merge(front, behind);
}

int main() {
  ofstream os;
  os.open("output3.csv", ios::out | ios::trunc);
  vector<int> input;
  os << "group"
     << ","
     << "No."
     << ","
     << "time" << endl;
  for (int i = 0; i < 4; i++) {
    // worst case input
    for (int j = 50000; j >= 0; j--) {
      input.push_back(j);
    }
    for (int k = 1; k < 200; k++) {
      // starting time
      auto start = std::chrono::steady_clock::now();

      vector<int> res = mixed_sort(k, input);

      // ending time
      auto end = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsed_seconds = end - start;
      // std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
      os << i << "," << k << "," << elapsed_seconds.count() << endl;
    }
  }
  cout << "done";
  return 0;
}