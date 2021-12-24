#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <cstring>

using namespace std;
using namespace std::chrono;
typedef unordered_set<int> stationIndexSet;

int MAX=32767;
typedef unordered_map<string,int> stationKV;
int w[500][500]={0};
int lines[500][500];
int d[500];
int pi[500];

string stationNames[500];//输出的时候需要靠index找到对应的name，为了开发的方便，浪费一点空间。
stationIndexSet V;
stationKV stationMap;

system_clock::time_point time_parser(string in){
    tm tm1 = {};
    string temp;
    if (in.substr(0,2)=="次")
    {
        const char* day="2011 Oct 08 ";
        temp=day+in.substr(2,10);
    }
    else{
        const char* day="2011 Oct 07 ";
        temp=day+in;
    }
    stringstream ss(temp);
    ss >> get_time(&tm1,"%Y %b %d %H:%M:%S");
    auto tp = system_clock::from_time_t(mktime(&tm1));
    return tp;
}

int min_gap(string a,string b){
            duration<double, milli> dur = time_parser(a) - time_parser(b);
            auto timedur=(int)dur.count();	
            return timedur/(60*1000);
}

// 怎么搞啊，，，不行，这个效率没救了orz。想喝奶茶。

int extract_min(stationIndexSet& q){//q: the remaining stations
    int equalv[10];
    int mind;
    int minv;
    int i=0;
    for(auto v:q){
        if (d[v]!=MAX)
        {
            minv=v;
            mind=d[v];
            break;
        }
    }
    
    for(auto v:q){
        if (d[v]==mind)
        {
            equalv[i++]=v;
        }
        if (d[v]<mind)
        {
            minv=v;
            mind=d[v];
            memset(equalv, 0, sizeof(equalv));
            i=0;
        }
    }
    q.erase(minv);
    // cout<<stationNames[minv]<<" ";
    return minv;
}
// 假设是数组，dv，v是数字。这样就不需要map，
void dijkstra(int s){//s:source
    for (auto v: V)//
    {
        d[v]=MAX;
        pi[v]=MAX;
    }
    d[s]=0;
    stationIndexSet S;
    auto q=V;
    while (!q.empty())
    {
        auto u=extract_min(q);
        S.insert(u);
        for (auto v :q)
        {
            if (w[u][v]!=0)
            {
                if (d[v]>d[u]+w[u][v])
                {
                    d[v]=d[u]+w[u][v];
                    pi[v]=u;
                }
            }
        }
    }
}

// 在一段中，n站有n-1段时间，换乘line-1，即n-2
// 在段衔接中因为必有起始点重合，这里不增加时间，增加换乘。

void path_output(int source,int destination,int& ptime,int& pchange,int& lastline){
    int output_path[80]={0};//所有的前站，共n-1,包括source
    auto p=destination;
    int i=0;
    output_path[i++]=destination;
    while (p!=source)
    {        
        output_path[i++] =pi[p];
        p=pi[p];
    }
    // output_path[i]=source;
    i--;
    int prevLine=0;
    // cout<<stationNames[source];     
    int cnt=0;
    for (int j=i-1;j>= 0 ;j--)//从source-1开始，
    { 
        ptime+=w[output_path[j]][output_path[j+1]];
        auto curLine=lines[output_path[j]][output_path[j+1]];
        if(j==i-1&&curLine==lastline){
            cnt--;
        }
        if (curLine==-1)
        {
            if (prevLine==3||prevLine==4)
            {
                curLine=prevLine;
            }
            else {
                curLine=3;
            }
        }
        if (curLine==prevLine)
        {
            ;
        }
        else{
            if (prevLine!=0)
            {
                cout<<"-"<<stationNames[output_path[j+1]];
            }
            cout<<"-line "<<curLine;
            cnt++;
            prevLine=curLine;
        }
    }
    lastline=lines[output_path[1]][output_path[0]];
    pchange+=cnt;
    cout<<"-"<<stationNames[destination];
}

void init_graph(){
        ifstream fp("combined_file1.csv");
    string line,prevtime,prevline,branch;
        pair<string,string> branchtimepoint;
    int preindex;
    getline(fp,line);
    int cnt=0;
    while (getline(fp,line))
    {
        //处理分支
        string sline,sname,stimepoint;
        istringstream readstr(line);
        getline(readstr,sline,',');
        getline(readstr,sname,',');
        getline(readstr,stimepoint,',');
        if (sname=="龙溪路"||sname=="嘉定新城")//should have a more general way
        {
            branchtimepoint.first=stimepoint;
            getline(readstr,branchtimepoint.second,',');
            branch=sname;
        }
        else if (stimepoint=="--")
        {
            getline(readstr,stimepoint,',');
            if (sname=="上海动物园"||"上海赛车场")
            {
                prevtime=branchtimepoint.second;
                preindex=stationMap.at(branch);
            }
        }
        else if (sname=="龙柏新村")
        {
            prevtime=branchtimepoint.first;
                preindex=stationMap.at(branch);
        }
        
        auto i=stationMap.insert(pair<string,int>(sname,++cnt));//从1开始给站点编号

        if(!i.second){//没插进去要减掉，cnt没有增加。
            cnt--;
        }
        else{
            stationNames[cnt]=sname;
        }
        auto curi=stationMap.at(sname);//也可以是完成插入后的find
        if (curi!=0&&sline==prevline)//除了一条线路的首站
        {
            //连接与前一站的距离
            //对于交换站，除与插入时的距离，还有第二次时寻找到后的adjacent。
            w[curi][preindex]= min_gap(stimepoint,prevtime);
            // cout<<w[curi][preindex]<<" ";
            // cout<<curi<<" "<<preindex<<" ";
            w[preindex][curi]= min_gap(stimepoint,prevtime);
            if (lines[curi][preindex]==3)//use -1 to represent 3 or 4
            {
                lines[curi][preindex]=-1;
                lines[preindex][curi]=-1;
            }
            else{
                lines[curi][preindex]=stoi(sline);
            lines[preindex][curi]=stoi(sline);
            }
        }
        preindex=curi;
        prevline=sline;
        prevtime=stimepoint;
    }
    for (auto station:stationMap)
    {
        V.insert(station.second);
    }
    auto roundfirst=stationMap.at("宜山路");
        auto roundlast=stationMap.at("虹桥路");

        w[roundfirst][roundlast]=2;
        w[roundlast][roundfirst]=2;

        lines[roundfirst][roundlast]=-1;
        lines[roundlast][roundfirst]=-1;

}

// 实现：每一段都是
void tb(){
    ifstream fp("performance-benchmark.txt");
    string line;
    int lastline=0;
    int cnt=0;
    while (getline(fp,line))
    {
        int timesum=0,changesum=0;
        string curstation;
        istringstream readstr(line);
        getline(readstr,curstation,'\t');
        string prestation=curstation;
        cout<<curstation;
        while (getline(readstr,curstation,'\t'))
        {
            dijkstra(stationMap.at(prestation));
            path_output(stationMap.at(prestation),stationMap.at(curstation),timesum,changesum,lastline);
            prestation=curstation;
        }
        cout<<"换乘时间"<<timesum<<endl;
        cout<<"换乘数"<<changesum<<endl;
    }
//反方向换乘，仅当最后一站为输入时产生，那么只考虑输出的话，作为首末站肯定会输出，换乘，通常情况下就是在输出的一个站左右线路不同时换乘一次。
}

void path_output_all(int source,int destination,int& ptime){
    int output_path[80];//所有的前站，共n-1,包括source
    auto p=destination;
    int i=0;
    while (p!=source)
    {
        output_path[i] =pi[p];
        p=pi[p];
        i++;
    }
    i--;
    // cout<<output_path[i-1]<<endl;
    cout<<stationNames[source];
    int cnt=0;
    for (int j=i-1;j>= 0 ;j--)//从source-1开始，
    { 
        ptime+=w[output_path[j]][output_path[j+1]];
        auto curLine=lines[output_path[j]][output_path[j+1]];
        cout<<w[output_path[j]][output_path[j+1]];
        cout<<"-"<<stationNames[output_path[j]];
    }
    ptime+=w[output_path[0]][destination];
    cout<<w[output_path[0]][destination];
    cout<<"-"<<stationNames[destination];
}

void path_time_tb(){
    int n;
    cin>>n;
    string curname,prevname;
    cin>>curname;
    prevname=curname;
    n--;
    int timesum=0;
    while (n--)
    {
        cin>>curname;
        dijkstra(stationMap.at(prevname));
        path_output_all(stationMap.at(prevname),stationMap.at(curname),timesum);
        prevname=curname;
    }
    cout<<timesum;
}

int main(){
    
    init_graph();
    tb();

    // string startstion,endstation;
    // cout<<stationMap.at("民雷路");
    // tb();
    // cin>>startstion>>endstation;
    // dijkstra(stationMap.at(startstion));
    // path_output(stationMap.at(startstion),stationMap.at(endstation));

    return 0;
}