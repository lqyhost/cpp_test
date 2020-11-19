#include <bits/stdc++.h>
#include <iostream>
#define FINAL 0
#define RUNNING 1
#define READY 2
#define BLOCK 3
using namespace std;

const int maxn=20;
int N;
struct PCB{
    int id;//进程标识数
    int priority;//进程优先级
    int cputime;//进程已占用的CPU时间
    int alltime;//进程还需占用的CPU时间
    //当进程再运行STARTBLOCK个时间片后，进程将进入阻塞状态，startblocktmp为副本
    int startblock,startblocktmp;
    //已阻塞的进程再等待BLOCKTIME个时间片后，将转换成就绪状态，blocktimetmp为副本
    int blocktime,blocktimetmp;
    int state;//进程状态,FINAL完成,RUNNING运行,READY就绪,BLOCK阻塞
}p1,p2;

struct PCB P[maxn];
/*
struct cmp1{//最大值出
    bool operator () (const PCB &p1,const PCB &p2) const{
        return p1.priority<p2.priority;
    }
};

struct cmp2{//最小值出
    bool operator () (const PCB &p1,const PCB &p2) const{
        return p1.blocktime>p2.blocktime;
    }
};

priority_queue<PCB,vector<PCB>,cmp1 > que1;
priority_queue<PCB,vector<PCB>,cmp2 > que2;
set<int>ready;
set<int>block;
set<int>::iterator it;
*/
//按格式打印各个进程的状态
void show(int i){
    cout<<"RUNNING PROG "<<i<<endl;
    printf("READY_QUEUE:");
    for(int i=0; i<N; i++){
        if(P[i].state==READY) cout<<"->id"<<i;
    }
    cout<<endl;
    printf("BLOCK_QUEUE:");
    for(int i=0; i<N; i++){
        if(P[i].state==BLOCK) cout<<"->id"<<i;
    }
    cout<<endl;
    printf("===================================================\n");
    printf("ID PRIORITY CPUTIME ALLTIME ");
    printf("STARTBLOCK BLOCKTIME STATE \n");

    for(int i=0; i<N; i++){
        printf("%d\t%d\t%d\t",P[i].id,P[i].priority,P[i].cputime);
        printf("%d\t%d\t%d\t",P[i].alltime,P[i].startblock,P[i].blocktime);
        if(P[i].state==FINAL) cout<<"FINAL"<<endl;
        else if(P[i].state==RUNNING) cout<<"RUNNING"<<endl;
        else if(P[i].state==READY) cout<<"READY"<<endl;
        else if(P[i].state==BLOCK) cout<<"BLOCK"<<endl;
    }
}
void process_management(){
    int n=0;//完成的进程数
    int j=0;//第j个时间片

    show(j);
    int running=0;//1表示有程序正在运行，0表示没有
    while(n!=N){
        //int flag=0;
        //优先队列que存就绪状态进程的优先级和对应的进程号
        priority_queue<pair<int,int> > que;
        //while(!que.empty()) que.pop();

        //遍历所有进程(一个时间片),及在各个状态下的操作
        for(int i=0; i<N; i++){
            if(P[i].state==READY) {
                P[i].priority+=1;
                que.push(make_pair(P[i].priority,i));
            }
            else if(P[i].state==RUNNING) {
                P[i].priority-=3;
                P[i].cputime+=1;
                P[i].alltime-=1;
                P[i].startblocktmp--;
                if(P[i].alltime==0) {//进程结束
                    P[i].state=FINAL;//结束标志
                    //flag=1;
                    running=0;
                    n++;//完成进程数加一
                }
                else if(P[i].startblocktmp<=0&&P[i].startblock>0) {//转为阻塞态
                    P[i].state=BLOCK;
                    P[i].blocktimetmp=P[i].blocktime;
                    running=0;
                }
            }
            else if(P[i].state==BLOCK){
                P[i].blocktimetmp-=1;
                if(P[i].blocktimetmp<=0) P[i].state=READY;//转为就绪态
            }
        }

        //当cpu空闲且就绪队列不为空时，取最大优先级的进程运行
        if(( (!running))&&(!que.empty()) ){
            pair<int,int> k=que.top();
            int tmp=k.second;
            P[tmp].state=1;
            P[tmp].startblocktmp=P[tmp].startblock;
            running=1;
        }
        show(++j);
    }
}
int main()
{
    printf("请输入进程数:\n");
    cin>>N;
    printf("请依次输入%d的初始值(进程从0~%d):\n",N,N-1);
    printf("(0-结束状态(FIANL),1-运行状态(RUNNING),2-就绪状态(READY),3-阻塞状态(BLOCK))\n");
    printf("ID PRIORITY CPUTIME ALLTIME ");
    printf("STARTBLOCK BLOCKTIME STATE \n");

    for(int i=0; i<N; i++){
        cin>>P[i].id>>P[i].priority>>P[i].cputime>>P[i].alltime;
        cin>>P[i].startblock>>P[i].blocktime>>P[i].state;
    }
    //P[n].state=1;
    printf("进程运行过程中在各个时间片的状态\n");
    process_management();
    return 0;
}
