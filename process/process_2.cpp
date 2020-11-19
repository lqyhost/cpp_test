// F:\debug\c++debug\OperatingSystem\ProcessManagement.cpp
// author:1045263694@qq.com
#include <iostream>
#include <string>
#include <ctime>
//#include <windows.h>

# define increment 5
using namespace std;
//以下是进程类的定义
class process
{
	friend void SuspendProcess(process *headptr,int counter);	//友元函数
	friend void WakeupProcess(process *headptr,int counter);	//友元函数
protected:
	int pid;				  //进程名
	int priority;		      //进程优先级
	int cputime;			  //进程使用CPU时间
	int status;		          //进程状态
	int Rotationtimeslice;    //轮转时间片
	int large;                //进程大小
public:
	process(){}				  //默认构造函数
	process(process &p)
	{
		p.pid=pid;
		//p.priority=priority;
		//p.cputime=cputime;
		//p.large=large;
		p.status=status;
	}
	virtual ~process(){}
	void display()
	{
		cout<<"进程名："<<pid<<endl//<<"优先级："<<priority<<endl<<"CPU时间："<<cputime<<endl<<"大小："<<large<<endl
		<<"状态："<<status<<endl;
	}
	void setall(int &counter)
	{
		cout<<"0-正常，1-挂起，2-唤醒" <<endl;
		cout<<"请输入新的进程名："; cin>>pid;
		//cout<<"请输入新的进程优先级："; cin>>priority;
		//cout<<"请输入新进程大小："; cin>>large;
		//cout<<"请输入新进程使用CPU时长："; cin>>cputime;
		cout<<"请输入新进程状态："; cin>>status;         //0-正常，1-挂起，2-唤醒
		counter++;
	}
	int Getpid()
	{
		return pid;
	}
	int Getstatus()
	{
		return status;
	}
};//process

void CreateNewprocess(process *headptr,int &counter,int capacity)
{
	if(counter < capacity)
	{
		(headptr+counter)->setall(counter);
	}
	else
	{
		cout<<"内存已满无法创建新进程，请申请新的内存空间或者执行换出操作"<<endl;
	}
	
}

void ViewProcess(process *headptr,int capacity,int counter)
{
	for(int i=0;i < counter;i++)
	{
		(headptr+i)->display();
	}
	cout<<"当前内存中有"<<counter<<"个进程"<<endl;
	cout<<"剩余可用内存空间为"<< capacity-counter<<"个进程"<<endl; 
}

void KillProcess(process *headptr,int &counter)
{
	int pid;
	cout<<"请输入你要杀死的进程名";cin>>pid;
	for(int i=0;i<counter;i++)
	{
		if((headptr+i)->Getpid()==pid)
		{
			for(int j=i;j<counter;j++)
				*(headptr+j)=*(headptr+j+1);
			break;
		}
		else if((i==counter-1)&&((headptr+i)->Getpid()!=pid))
		{
			cout<<"您输入了错误的进程名"<<endl;
			Sleep(1000);
		}			
	}
	--counter;
}

void SwapOutProcess(process *headptr,int &counter)
{
	int pid;
	cout<<"请输入你要换出的进程名";cin>>pid;
	for(int i=0;i<counter;i++)
	{
		if((headptr+i)->Getpid()==pid)
		{
			--counter;
			(headptr+i)->setall(counter);
			break;
		}
		else if((i==counter-1)&&((headptr+i)->Getpid()!=pid))
		{
			cout<<"您输入了错误的进程名"<<endl;
			Sleep(1000);
		}			
	}
}

void SuspendProcess(process *headptr,int counter)
{
	int pid;
	cout<<"请输入你要挂起的进程名";cin>>pid;
	for(int i=0;i<counter;i++)
	{
		if((headptr+i)->Getpid()==pid)
		{
			(headptr+i)->status=1;
			break;
		}
		else if((i==counter-1)&&((headptr+i)->Getpid()!=pid))
		{
			cout<<"您输入了错误的进程名"<<endl;
			Sleep(1000);
		}			
	}
}

void WakeupProcess(process *headptr,int counter)
{
	int pid;
	cout<<"请输入你要唤醒的进程名";cin>>pid;
	for(int i=0;i<counter;i++)
	{
		if((headptr+i)->Getpid()==pid)
		{
			(headptr+i)->status=0;
			cout<<"唤醒成功"<<endl;
			break;
		}
		else if((i==counter-1)&&((headptr+i)->Getpid()!=pid))
		{
			cout<<"您输入了错误的进程名"<<endl;
			Sleep(1000);
		}			
	}	
}

void ApplyForNewMemory(process *&headptr,int &capacity,int counter)
{
	
	if(counter==capacity)
	{
		capacity+=increment;
		process *p=headptr;	
		process *newheadptr = new process[capacity];
		process *q=newheadptr;
		for(int i=0;i<counter;i++)
		{
			*q=*p;
			p++;
			q++;
		}
		p=headptr;
		headptr=newheadptr;
		delete[] p;
	}
	else
		cout<<"您无需申请新的存储空间"<<endl;

}
int counter=0;				//当前进程数量
int capacity=5;				//初始容量
int applystatus=0;
//以下是主函数
int main()
{

	time_t Time;
	time(&Time);
	int control;
	process *headptr = new process[capacity];
	do
	{
		cout <<  "当前系统时间是:" << ctime(&Time) <<endl;
		cout<<"|========================进程操作管理系统===========================|"<<endl
			<<"|                        *1.创建新的进程	                           |"<<endl
			<<"|                        *2.查看运行进程                            |"<<endl
			<<"|                        *3.换出新的进程                            |"<<endl
			<<"|                        *4.杀死运行进程                            |"<<endl
			<<"|                        *5.挂起运行进程                            |"<<endl
			<<"|                        *6.唤醒挂起进程                            |"<<endl
			<<"|                        *7.申请新的内存                            |"<<endl
			<<"|                        *0.退出                                    |"<<endl
			<<endl;
		cout<<"请输入你的指令：";
		cin>>control;
		switch(control)
		{
		case 1: 
				system("cls");
				cout<<"（创建新的进程函数）"<<endl;
				CreateNewprocess(headptr,counter,capacity);
				break;
		case 2: 
				system("cls");
				cout<<"（查看运行进程函数）"<<endl;
				ViewProcess(headptr,capacity,counter);
				break;
		case 3: 
				system("cls");
				cout<<"（换出新的进程函数）"<<endl;
				SwapOutProcess(headptr,counter);
				break;
		case 4: 
				system("cls");
				cout<<"（杀死运行进程函数）"<<endl;
				KillProcess(headptr,counter);
				break;
		case 5: 
				system("cls");
				cout<<"（挂起运行进程函数）"<<endl;
				SuspendProcess(headptr,counter);
				break;
		case 6: 
				system("cls");
				cout<<"（唤醒挂起进程函数）"<<endl;
				WakeupProcess(headptr,counter);
				break;
		case 7: 
				system("cls");
				cout<<"（申请新的内存函数）"<<endl;
				ApplyForNewMemory(headptr,capacity,counter);
				break;
		case 0: 
				cout<<"                         感谢使用！ 3秒后程序结束"<<endl;
				Sleep(3000);
				break;
		default:
				cout<<"                         输入错误！请重新输入：";
				Sleep(1000);
				system("cls");
		}
	}
	while(control!=0);
	delete[] headptr;
	return 0;
}
