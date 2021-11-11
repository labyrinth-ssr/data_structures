#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;

unordered_map<char, int> pri{{'*', 1},  {'/', 1},  {'+', 2},  {'-', 2},
                             {'%', 1}, {'(', 20}, {')', -3},
                             {'{', 20}, {'}', -3},{'^',0},{'!',-1}};

vector<char> infix_to_postfix(vector<char> input) {
  // input.push_back('+');
  stack<char> sigStack;
  vector<char> res;
  bool last_is_num = false;
  int last_num;
  int cnt=0;
  for (auto i : input) {
    if (i >= '0' && i <= '9') {
      res.push_back(i);
        if(cnt==input.size()-1){
            res.push_back(' ');
        }
      last_is_num = true;
    } else {
      if (last_is_num) {
        res.push_back(' ');
      }
      //如何判断overflow
      if (i == ')' || i == '}') {
        while (!sigStack.empty() &&
               (sigStack.top() != '(' && sigStack.top() != '{')) {
          // if(sigStack.top()!='('&&sigStack.top()!='{'&&sigStack.top()!=')'&&sigStack.top()!='}'){
          res.push_back(sigStack.top());
          cout << "res push back in brace" << sigStack.top() << endl;
          res.push_back(' ');
          // }
          sigStack.pop();
        }
        sigStack.pop(); // pop left braces
      }

      else if (i == '(' || i == '{' || sigStack.empty() ||
               pri.at(i) < pri.at(sigStack.top())) {
        sigStack.push(i);
      }

      else {
        while (!sigStack.empty() &&
               (pri.at(sigStack.top()) <= pri.at(i) && sigStack.top() != '(' &&
                sigStack.top() != '{')) {
          // if(sigStack.top()!='('&&sigStack.top()!='{'&&sigStack.top()!=')'&&sigStack.top()!='}'){
          res.push_back(sigStack.top());
          // cout<<"res push back ordinary"<<sigStack.top()<<endl;
          res.push_back(' ');
          // }
          sigStack.pop();
        }
        cout << "push sigStack:" << i << endl;
        sigStack.push(i);
      }
      last_is_num = false;
    }
    cnt++;
  }
  while (!sigStack.empty()) {
    res.push_back(sigStack.top());
    res.push_back(' ');
    sigStack.pop();
  }
  return res;
}
//each element is separated by a blankspace;
int calculate(vector<char> input){
    stack<int> nums;
    for(int i =0;i<input.size();i++){
        if(input[i]==' ') continue;
        else if (input[i] >='0'&&input[i] <='9'){
            int num=input[i]-'0';
            i++;
            while(input[i] >='0'&&input[i] <='9'){
                num =(num*10)+(input[i]-'0');
                i++;
            }
            nums.push(num);
            // cout<<"push:"<<num<<endl;

        }
        else{
            switch (input[i])
            {
            case '*':
                {
                    int front=nums.top();
                    nums.pop();
                    front *= nums.top();
                    nums.pop();
                    nums.push(front);
                }
                break;
            case '+':
                {
                    int back=nums.top();
                    nums.pop();
                    back+=nums.top();
                    nums.pop();
                    nums.push(back);
                }
                break;
                case '-':
                {
                    int back=nums.top();
                    nums.pop();
                    int res=nums.top()-back;
                    nums.pop();
                    nums.push(res);
                }
                break;
                case '/':
                {
                    int back=nums.top();
                    nums.pop();
                    int res=nums.top()/back;
                    nums.pop();
                    nums.push(res);
                }
                break;
                case '%':
                {
                    int back=nums.top();
                    nums.pop();
                    int res=nums.top()% back;
                    nums.pop();
                    nums.push(res);
                }
                break;
                case '^':{
                  int back=nums.top();
                    nums.pop();
                    int res=pow(nums.top(),back);
                    nums.pop();
                    nums.push(res);
                }
                case '!':{
                  int single=nums.top();
                  nums.pop();
                  int res=1;
                  for(int i=1;i<=single;i++){
                    res*=i;
                  }
                  nums.push(res);
                }
            default:
                break;
            }

        }
    }
    return nums.top();
}

int main() {
  char c;
  vector<char> input;
  while (cin >> c && c != '=') {
    input.push_back(c);
  }
  vector<char> res = infix_to_postfix(input);
  for (auto i : res) {
    cout << i;
  }
  cout<<calculate(res);
  return 0;
}