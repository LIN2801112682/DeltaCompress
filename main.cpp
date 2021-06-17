#include <iostream>
#include<bits/stdc++.h>
using namespace std;
typedef struct Node
{
    int m_low;
    int m_high;
    string m_content;
    int m_type;
} Node;
stack<Node> delta;
int backtrackingPath(string basic,string s)
{
    int m = basic.size();
    int n = s.size();
    int dp[m+1][n+1];

    for(int i = 0; i < m+1; i++)dp[i][0] = i;
    for(int i = 0; i < n+1; i++)dp[0][i] = i;

    for(int i = 1; i < m+1; i++)
    {
        for(int j = 1; j < n+1; j++)
        {
            if(basic[i-1] == s[j-1])
            {
                dp[i][j] = dp[i-1][j-1];
            }
            else
            {
                dp[i][j] = min(min(dp[i-1][j]+1,dp[i][j-1]+1),dp[i-1][j-1]+1);
            }
        }
    }
    cout<<"distance "<<dp[m][n]<<endl;
    while(n>=0||m>=0)
    {
        if(n!=0&&dp[m][n-1]+1==dp[m][n])
        {
            Node a;
            a.m_content=s[n-1];
            //cout<<a.m_content<<"hhhhhhh"<<endl;
            a.m_low=m-1;
            a.m_high=m;
            a.m_type=0;
            if(delta.size()==0)
            {
                delta.push(a);
            }
            else
            {
                Node b=delta.top();
                if(b.m_type==a.m_type&&b.m_low==a.m_low)
                {
                    a.m_content=a.m_content+b.m_content;
                    delta.pop();
                }
                delta.push(a);
            }
            cout<<"insert "<<s[n-1]<<" at "<<m-1<<endl;
            n=n-1;
            continue;
        }
        else if(m!=0&&dp[m-1][n]+1 == dp[m][n])
        {
            cout<<"delete "<<basic[m-1]<<" at "<<m-1<<endl;
            Node a;
            a.m_content="-";
            a.m_low=m-1;
            a.m_high=m-1;
            a.m_type=1;
            if(delta.size()==0)
            {
                delta.push(a);
            }
            else
            {
                Node b=delta.top();
                if(b.m_type==a.m_type&&b.m_low==a.m_low+1)
                {
                    a.m_high=b.m_high;
                    delta.pop();
                }
                delta.push(a);
            }
            m=m-1;
            continue;
        }
        else if(dp[m-1][n-1]+1 == dp[m][n])
        {
            cout<<"replace "<<basic[m-1]<<" to "<<s[n-1]<<" at "<<m-1<<endl;
            Node a;
            a.m_content=s[n-1];
            a.m_low=m-1;
            a.m_high=m-1;
            a.m_type=2;
            if(delta.size()==0)
            {
                delta.push(a);
            }
            else
            {
                Node b=delta.top();
                if(b.m_type==a.m_type&&b.m_low==a.m_low+1)
                {
                    a.m_high=b.m_high;
                    a.m_content=a.m_content+b.m_content;
                    delta.pop();
                }
                delta.push(a);
            }
            n=n-1;
            m=m-1;
            continue;
        }
        n=n-1;
        m=m-1;
    }
}

int main()
{
    //string basic = "{\"level\":\"info\",\"time\":\"2021-03-25T17:01:01.940Z\",\"caller\":\"/root/gopath/src/github.com/influxdata/influxdb/query/executor.go:530\",\"msg\":\"Executing query\",\"service\":\"query\",\"query\":\"SELECT last(SL_1_Tag108) AS SL_1_Tag108 FROM SZSSLKJYXGS_1OU7.autogen.\"68885b4c32ff4708a8a649c9b4baae59\" WHERE gatewayId = '2c938083783650af01786535768406ac' AND time >= '2021-03-24T17:01:01Z' AND time <= '2021-03-25T17:01:01Z' fill(none)\",\"batch\":1}";
    //string s = "{\"level\":\"info\",\"time\":\"2021-03-25T16:10:00.062Z\",\"caller\":\"/root/gopath/src/github.com/influxdata/influxdb/query/executor.go:530\",\"msg\":\"Executing query\",\"service\":\"query\",\"query\":\"SELECT count(C1_B1_G1_Tag3) FROM liantong.autogen.\"0ebfb027fcb54e3e93db371c85646230\" WHERE C1_B1_G1_Tag3 = 22.000000000 AND time >= '2021-03-25T16:09:00Z' AND time < '2021-03-25T16:10:00Z'\",\"batch\":1}";
    string basic;
    ifstream basic_in("chr1.raw", ios::in);
    getline(basic_in,basic);
    basic_in.close();
    cout<<basic<<endl;

    ifstream logfile;
    logfile.open("path.txt",ios::in);
    string s;
    int n=0;
    while(getline(logfile,s))
    {
        stringstream ss;
        ss << n;
        string nstr = ss.str();
        string filename="delta\\chr_"+nstr+".delta";
        backtrackingPath(basic,s);
        ofstream chrfile(filename);
        while(delta.size()!=0)
        {
            Node d=delta.top();
            if(delta.size()!=1)
            {
                chrfile<<d.m_low<<"#"<<d.m_high<<"#"<<d.m_content<<"#"<<d.m_type<<endl;
            }
            else
            {
                chrfile<<d.m_low<<"#"<<d.m_high<<"#"<<d.m_content<<"#"<<d.m_type;
            }
            delta.pop();
        }
        chrfile.close();
        n++;
    }
    logfile.close();
    return 0;
}
