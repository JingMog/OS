#ifndef OS_H
#define OS_H
#include <iostream>
#include <vector>

#include <algorithm>
#include <QDebug>
#include "Procress.h"
using namespace std;

//分区状态表
struct undistributtable
{
    int startadd;//起始地址
    int length;//长度
    QString state;//分区状态，若占用，则写该进程的pid；若空闲，则写unmalloced
};


class os
{
    private:
        Procress* readyhead;//就绪队列
        Procress* backhead;//后备队列
        Procress* endhead;//结束队列
        Procress* runhead;//运行队列
        Procress* suspendhead;//挂起队列
        Procress* blockhead;
        int mainMemory;//主存空间大小
        int cpucycle;//当前CPU执行周期，用来记录进程到达时间
        int cpunum;//cpu核心数
        vector<undistributtable> undisttable;//分区状态表
        vector<QString> pidvector;//pid向量组

        void Sort(Procress* pBegin);
        void erasevector(int loc);//删除未分分区表中指定位置的元素
        void reclaimmem(Procress* p);//回收内存
        bool mallocmem(Procress* p);//分配内存

        void addProcress(Procress* p); //增加进程到就绪队列中
        bool checkpreprogress(Procress* p);//检查进程的所有前驱节点是否已经全部执行完成
        void return_to_ready();
        void return__(Procress* p);
    public:
        os();//构造函数
        //void show();
        void showtable();
        void scheduling();//调度进程
        void backscheduling();//从后备队列中调度一作业进入内存
        void suspend(Procress* p);//挂起进程
        void unsuspend(Procress* p);//解挂
        void run();  //运行
        Procress * creatProcress(int _space, QString _pid, int _runtime,
            int _priority, int _attribute, vector<QString> pre = vector<QString>());//创建进程

        //void showtableinfo(Ui::app * ui);//在表格中显示信息

        void set_cpunum(int num);
        void set_mainMemery(int num);
        Procress* get_readyhead();
        Procress* get_backhead();
        Procress* get_endhead();
        Procress* get_runhead();
        Procress* get_suspendhead();
        Procress* get_blockhead();
        vector<undistributtable> get_undisttable();
        vector<QString> get_pidvector();
        int get_mainmem();//获取主存空间

        void IO(Procress* p);
        void endIO(Procress* p);
};



#endif // OS_H
