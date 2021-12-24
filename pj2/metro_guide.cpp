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

string stationNames[500];//�����ʱ����Ҫ��index�ҵ���Ӧ��name��Ϊ�˿����ķ��㣬�˷�һ��ռ䡣
stationIndexSet V;
stationKV stationMap;

system_clock::time_point time_parser(string in){
    tm tm1 = {};
    string temp;
    if (in.substr(0,2)=="��")
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

// ��ô�㰡���������У����Ч��û����orz������̲衣

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
// ���������飬dv��v�����֡������Ͳ���Ҫmap��
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

// ��һ���У�nվ��n-1��ʱ�䣬����line-1����n-2
// �ڶ��ν�����Ϊ������ʼ���غϣ����ﲻ����ʱ�䣬���ӻ��ˡ�

void path_output(int source,int destination,int& ptime,int& pchange,int& lastline){
    int output_path[80]={0};//���е�ǰվ����n-1,����source
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
    for (int j=i-1;j>= 0 ;j--)//��source-1��ʼ��
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
        //�����֧
        string sline,sname,stimepoint;
        istringstream readstr(line);
        getline(readstr,sline,',');
        getline(readstr,sname,',');
        getline(readstr,stimepoint,',');
        if (sname=="��Ϫ·"||sname=="�ζ��³�")//should have a more general way
        {
            branchtimepoint.first=stimepoint;
            getline(readstr,branchtimepoint.second,',');
            branch=sname;
        }
        else if (stimepoint=="--")
        {
            getline(readstr,stimepoint,',');
            if (sname=="�Ϻ�����԰"||"�Ϻ�������")
            {
                prevtime=branchtimepoint.second;
                preindex=stationMap.at(branch);
            }
        }
        else if (sname=="�����´�")
        {
            prevtime=branchtimepoint.first;
                preindex=stationMap.at(branch);
        }
        
        auto i=stationMap.insert(pair<string,int>(sname,++cnt));//��1��ʼ��վ����

        if(!i.second){//û���ȥҪ������cntû�����ӡ�
            cnt--;
        }
        else{
            stationNames[cnt]=sname;
        }
        auto curi=stationMap.at(sname);//Ҳ��������ɲ�����find
        if (curi!=0&&sline==prevline)//����һ����·����վ
        {
            //������ǰһվ�ľ���
            //���ڽ���վ���������ʱ�ľ��룬���еڶ���ʱѰ�ҵ����adjacent��
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
    auto roundfirst=stationMap.at("��ɽ·");
        auto roundlast=stationMap.at("����·");

        w[roundfirst][roundlast]=2;
        w[roundlast][roundfirst]=2;

        lines[roundfirst][roundlast]=-1;
        lines[roundlast][roundfirst]=-1;

}

// ʵ�֣�ÿһ�ζ���
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
        cout<<"����ʱ��"<<timesum<<endl;
        cout<<"������"<<changesum<<endl;
    }
//�����򻻳ˣ��������һվΪ����ʱ��������ôֻ��������Ļ�����Ϊ��ĩվ�϶�����������ˣ�ͨ������¾����������һ��վ������·��ͬʱ����һ�Ρ�
}

void path_output_all(int source,int destination,int& ptime){
    int output_path[80];//���е�ǰվ����n-1,����source
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
    for (int j=i-1;j>= 0 ;j--)//��source-1��ʼ��
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
    // cout<<stationMap.at("����·");
    // tb();
    // cin>>startstion>>endstation;
    // dijkstra(stationMap.at(startstion));
    // path_output(stationMap.at(startstion),stationMap.at(endstation));

    return 0;
}