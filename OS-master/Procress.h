#ifndef PROCRESS_H
#define PROCRESS_H

#include<iostream>
#include<vector>
#include<QDebug>
#include <QStandardItemModel>
using namespace std;

class Procress
{
    private:
        QString pid; //进程名
        int runtime; //要求运行时间
        int arrivaltime; //进程到达时间，为了实现动态添加进程
        int priority;//进程优先级
        QString state;//进程状态：活动就绪、静止就绪、挂起、运行、后备
        int attribute;//进程属性:独立进程（0）、同步进程（1）
        vector<QString> preprogress;//存放同步进程的前驱进程
        int space;//进程所需内存空间大小
    public:
        Procress* next;//链指针
        Procress();//构造函数
        Procress(int _space, QString _pid, int _runtime,
            int _priority, QString _state, int _attribute, vector<QString> pre);
        void set_Procress(int _space, QString _pid, int _runtime, int _arrivaltime,
            int _priority, QString _state, int _attribute);//设置进程属性
        void show_Procress();//显示进程属性
        //void run();//进程运行
        void show(QStandardItemModel* model,int row);//显示该进程的信息

        //对外接口
        int get_priority();//获取进程优先级
        QString get_state();//获取进程状态
        int get_runtime();//获取运行时间
        int get_arrivaltime();//获取到达时间
        QString get_pid();//获取进程Pid
        int get_attribute();//获取进程属性
        int get_space();//获取进程所需内存空间大小
        vector<QString> get_preprogress();//获取进程的前驱
        void set_state(QString _state);//设置进程状态
        void set_runtime(int _runtime);//设置运行时间
        void set_priority(int _priority);//设置优先级
        void set_arrivaltime(int _arrivaltime);//设置到达时间
        void set_pid(QString _pid);
        void set_attribute(int _attribute);
        void set_space(int _space);
};




#endif // PROCRESS_H
