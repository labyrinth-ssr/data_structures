#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace std::chrono;

int MAX=32767;
array<string,500> stations;
int w[500][500];
int line[500][500];
int d[500];

system_clock::time_point time_parser(string in){
    tm tm1 = {};
    stringstream ss("2014 "+in);
    ss >> get_time(&tm1,"%Y %H:%M:%S");
    auto tp = system_clock::from_time_t(mktime(&tm1));
    return tp;
}

void dijkstra(){
    for ()
    {
        d[v]=
    }
    
}

int main(){

// tm tm2={};
// stringstream ss2("2014 12:40:00");
// ss2 >> get_time(&tm2, "%Y %H:%M:%S");
// auto tp2 = system_clock::from_time_t(mktime(&tm2));
// duration<double, milli> dur = tp2 - tp;	
// cout << "time: " << dur.count()/1000/60 << "m" << std::endl;
    for (int i = 0; i < 14; i++)
    {
        ifstream fp("line"+i); //定义声明一个ifstream对象，指定文件路径
        string line;
        getline(fp,line); //跳过列名，第一行不做处理
        int cnt=1;
        system_clock::time_point prevTime;
        while (getline(fp,line)){ //循环读取每行数据
        string name,strTime;
        int time=0;
        istringstream readstr(line); //string数据流化
        //将一行数据按'，'分割
            getline(readstr,name,','); 
            getline(readstr,name,','); 
            if (cnt==1)
            {
                getline(readstr,strTime,','); 
                auto time1=time_parser(strTime);
                prevTime=time1;
            }
            else{
            getline(readstr,strTime,','); 
            auto time1=time_parser(strTime);
            duration<double, milli> dur = time1 - prevTime;
            time=(int)dur.count();	
            prevTime=time1;
            }
        for (int j = 0; j < cnt; j++)
        {
            if (stations[j]==)
            {
                /* code */
            }
            
        }
    }
    }

    return 0;
}