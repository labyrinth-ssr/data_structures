#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;
using namespace std::chrono;
typedef unordered_set<int> stationIndexSet;

int MAX = 32767;
typedef unordered_map<string, int> stationKV;
int w[500][500] = {0};
int lines[500][500];
int d[500];
// int pi[500];
vector<int> pi[500],piu[500], temp, tempLineStation, entireMinchange;
vector<vector<int>> entireLineStation;
int segMinChange = MAX;
int temppTime = 0;

string stationNames
    [500]; //�����ʱ����Ҫ��index�ҵ���Ӧ��name��Ϊ�˿����ķ��㣬�˷�һ��ռ䡣
stationIndexSet V;
stationKV stationMap;

system_clock::time_point time_parser(string in) {
  tm tm1 = {};
  string temp;
  if (in.substr(0, 2) == "��") {
    const char *day = "2011 Oct 08 ";
    temp = day + in.substr(2, 10);
  } else {
    const char *day = "2011 Oct 07 ";
    temp = day + in;
  }
  stringstream ss(temp);
  ss >> get_time(&tm1, "%Y %b %d %H:%M:%S");
  auto tp = system_clock::from_time_t(mktime(&tm1));
  return tp;
}

int min_gap(string a, string b) {
  duration<double, milli> dur = time_parser(a) - time_parser(b);
  auto timedur = (int)dur.count();
  return timedur / (60 * 1000);
}

int extract_min(stationIndexSet &q) { // q: the remaining stations
  int equalv[10];
  int mind;
  int minv;
  int i = 0;
  for (auto v : q) {
    if (d[v] != MAX) {
      minv = v;
      mind = d[v];
      break;
    }
  }

  for (auto v : q) {
    if (d[v] == mind) {
      equalv[i++] = v;
    }
    if (d[v] < mind) {
      minv = v;
      mind = d[v];
      memset(equalv, 0, sizeof(equalv));
      i = 0;
    }
  }
  q.erase(minv);
  // cout<<stationNames[minv]<<" ";
  return minv;
}
// ���������飬dv��v�����֡������Ͳ���Ҫmap��
void dijkstra(int s) { // s:source
  for (auto v : V)     //
  {
    d[v] = MAX;
    pi[v].push_back(MAX);
  }
  d[s] = 0;
  stationIndexSet S;
  auto q = V;
  while (!q.empty()) {
    auto u = extract_min(q);
    S.insert(u);
    for (auto v : q) {
      if (w[u][v] != 0) {
        if (d[v] > d[u] + w[u][v]) {
          d[v] = d[u] + w[u][v];
          pi[v].clear();
          pi[v].push_back(u);
        } else if (d[v] == d[u] + w[u][v]) {
          pi[v].push_back(u);
        }
      }
    }
  }
}

// ��һ���У�nվ��n-1��ʱ�䣬����line-1����n-2
// �ڶ��ν�����Ϊ������ʼ���غϣ����ﲻ����ʱ�䣬���ӻ��ˡ�

//�޿ɱ���ر��ֿ��ˣ��ڵ�һ������γ�վ�������浽templingstation,�������ĺ����������
void merge_line() {
  tempLineStation.clear();
  int i = temp.size();
  int pline = 0;
  auto start = temp.back();
  tempLineStation.push_back(start);
  for (int j = i - 2; j >= 0; j--) //��source-1��ʼ��
  {
    if (pline != lines[temp[j]][temp[j + 1]]) {
      if (pline != 0) {
        tempLineStation.push_back(temp[j + 1]);
      }
      pline = lines[temp[j]][temp[j + 1]];
      tempLineStation.push_back(pline); //
    }
  }
  tempLineStation.push_back(temp[0]);
  for (int i = 0; i < tempLineStation.size(); i++) {
    if (tempLineStation[i] == -1) //ֻ����·����-1
    {
      if (i - 2 >= 0 &&
          (tempLineStation[i - 2] == 3 || tempLineStation[i - 2] == 4)) {
        tempLineStation[i] = tempLineStation[i - 2];
        tempLineStation.erase(tempLineStation.begin() + i - 2,
                              tempLineStation.begin() + i);
        i--;
      } else if (i + 2 < tempLineStation.size() &&
                 (tempLineStation[i + 2] == 3 || tempLineStation[i + 2] == 4)) {
        // tempLineStation[i]=tempLineStation[i+2];
        tempLineStation.erase(tempLineStation.begin() + i,
                              tempLineStation.begin() + i + 2);
        i--;
      } else
        tempLineStation[i] = 3;
    }
  }
}

void branch_line_output(const int &cnt) { //����seg���������line��ͬ�����⣬�����ӡ������seg��С������һ�ֿ��ܵĶ�·��
  int i = temp.size();
  temppTime = 0;
  for (int j = i - 2; j >= 0; j--) //��source-1��ʼ��
  {
    temppTime += w[temp[j]][temp[j + 1]];
  }
  merge_line();
  if (tempLineStation.size() == 2) {
    auto last = entireLineStation.back();
    auto ins = last[last.size() - 2];
    tempLineStation.insert(tempLineStation.begin() + 1, ins);
  }
  for (int j = 0; j + 1 < tempLineStation.size(); j += 2) {
    cout << stationNames[tempLineStation[j]] << "-";
    cout << "line " << tempLineStation[j + 1] << "-";
  }

  cout << stationNames[tempLineStation.back()];
  if (tempLineStation.back() == tempLineStation[0]) {
    cout << " ��������" << 0;

  } else {
    cout << " ��������" << tempLineStation.size() / 2;
  }

  if (tempLineStation.size() / 2 < segMinChange) {
    segMinChange = tempLineStation.size() / 2;
    if (entireLineStation.size() <= cnt) {
      entireLineStation.push_back(tempLineStation);
    } else {
      entireLineStation[cnt] = tempLineStation;
    }
  }
}

void path_output_dfs(int s, int t,  
                     const int &cnt) { //�������п��ܵĶ�·��
  if (s == t) {
    temp.push_back(t);
    branch_line_output(cnt);
    // segMinChange=MAX;
    cout << endl;
    temp.pop_back();
    return;
  }
  temp.push_back(t);
  for (int i = 0; i < pi[t].size(); i++) {
    path_output_dfs(s, pi[t][i],
                    cnt); //��Ϊ��������������ֱ�ӵݹ�
  }
  temp.pop_back();
}

void entire_line_station_print_change() { //
  int changesum = 0;
  int preline = 0;
  for (auto i : entireLineStation) {
    if (preline == i[1]) {
      changesum--;
    }
    for (int j = 0; j + 1 < i.size(); j += 2) {
      cout << stationNames[i[j]] << "-";
      cout << "line " << i[j + 1] << "-";
    }
    preline = i[i.size() - 2];
    changesum += i.size() / 2;
  }
  cout << stationNames[entireLineStation.back().back()];
  cout << "��С��������" << --changesum << endl;
}

bool bellman_ford(int s) {
  int size = V.size();
  for (auto v : V) //
  {
    d[v] = MAX;
    pi[v].push_back(MAX);
  }
  d[s] = 0;
  for (int i = 1; i <= size - 1; i++) {
    for (int u = 1; u <= size; u++) {
      for (int v = 1; v <= size; v++) {
        if (w[u][v] != 0) {
          if (d[v] > d[u] + w[u][v]) {
            d[v] = d[u] + w[u][v];
            pi[v].clear();
            pi[v].push_back(u);
            // pi[v]=u;
          } 
          else if (d[v] == d[u] + w[u][v]) {
            if (find(pi[v].begin(),pi[v].end(),u)==pi[v].end())
            {
                          pi[v].push_back(u);

            }
            
          }
        }
      }
    }
  }

  for (int u = 1; u <= size; u++) {    
    for (int v = 1; v <= size; v++) {
      if (w[u][v] != 0) {
        if (d[v] > d[u] + w[u][v]) {
          return false;
        }
      }
    }
  }
  return true;
}

void init_graph() {
  ifstream fp("combined_file1.csv");
  string line, prevtime, prevline, branch;
  pair<string, string> branchtimepoint;
  int preindex;
  getline(fp, line);
  int cnt = 0;
  while (getline(fp, line)) {
    //�����֧
    string sline, sname, stimepoint;
    istringstream readstr(line);
    getline(readstr, sline, ',');
    getline(readstr, sname, ',');
    getline(readstr, stimepoint, ',');
    if (sname == "��Ϫ·" ||
        sname == "�ζ��³�") // should have a more general way
    {
      branchtimepoint.first = stimepoint;
      getline(readstr, branchtimepoint.second, ',');
      branch = sname;
    } else if (stimepoint == "--") {
      getline(readstr, stimepoint, ',');
      if (sname == "�Ϻ�����԰" || "�Ϻ�������") {
        prevtime = branchtimepoint.second;
        preindex = stationMap.at(branch);
      }
    } else if (sname == "�����´�") {
      prevtime = branchtimepoint.first;
      preindex = stationMap.at(branch);
    }

    auto i =
        stationMap.insert(pair<string, int>(sname, ++cnt)); //��1��ʼ��վ����

    if (!i.second) { //û���ȥҪ������cntû�����ӡ�
      cnt--;
    } else {
      stationNames[cnt] = sname;
    }
    auto curi = stationMap.at(sname);   //Ҳ��������ɲ�����find
    if (curi != 0 && sline == prevline) //����һ����·����վ
    {
      //������ǰһվ�ľ���
      //���ڽ���վ���������ʱ�ľ��룬���еڶ���ʱѰ�ҵ����adjacent��
      w[curi][preindex] = min_gap(stimepoint, prevtime);
      w[preindex][curi] = min_gap(stimepoint, prevtime);
      if (lines[curi][preindex] == 3) // use -1 to represent 3 or 4
      {
        lines[curi][preindex] = -1;
        lines[preindex][curi] = -1;
      } else {
        lines[curi][preindex] = stoi(sline);
        lines[preindex][curi] = stoi(sline);
      }
    }
    preindex = curi;
    prevline = sline;
    prevtime = stimepoint;
  }
  for (auto station : stationMap) {
    V.insert(station.second);
  }
  auto roundfirst = stationMap.at("��ɽ·");
  auto roundlast = stationMap.at("����·");
  w[roundfirst][roundlast] = 2;
  w[roundlast][roundfirst] = 2;
  lines[roundfirst][roundlast] = -1;
  lines[roundlast][roundfirst] = -1;
}
// ʵ�֣�ÿһ�ζ���
void tb() {
  ifstream fp("performance-benchmark.txt");
  string line;
  int lastline = 0;
  int cnt = 6;
  while (getline(fp, line) && cnt--) {
    int timesum = 0, changesum = 0;
    string curstation;
    istringstream readstr(line);
    getline(readstr, curstation, '\t');
    string prestation = curstation;
    int cnt1 = 0;
    while (getline(readstr, curstation, '\t')) {
      segMinChange = MAX;
      bellman_ford(stationMap.at(prestation));
      path_output_dfs(stationMap.at(prestation), stationMap.at(curstation),
                       cnt1++);
      timesum += temppTime;
      prestation = curstation;
    }
    entire_line_station_print_change();

    cout << "����ʱ��" << timesum << endl;
    entireLineStation.clear();
  }
  //�����򻻳ˣ��������һվΪ����ʱ��������ôֻ��������Ļ�����Ϊ��ĩվ�϶�����������ˣ�ͨ������¾����������һ��վ������·��ͬʱ����һ�Ρ�
}

int main() {
  int timesum = 0, changesum = 0, lastline = 0;
  init_graph();

  tb();
  // string a,b;
  // cin>>a>>b;
  // dijkstra(stationMap.at(a));
  
  // path_output_dfs(stationMap.at(a),stationMap.at(b),0);
  // // tb();
  // cout<<stationNames[1];

  return 0;
}