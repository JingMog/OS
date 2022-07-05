#include "os.h"


os::os()
{
    //构造队列(链表)
    readyhead = new Procress;
    readyhead->next = nullptr;
    backhead = new Procress;
    backhead->next = nullptr;
    endhead = new Procress;
    endhead->next = nullptr;
    runhead = new Procress;
    runhead->next = nullptr;
    suspendhead = new Procress;
    suspendhead->next = nullptr;
    blockhead = new Procress;
    blockhead->next = nullptr;
    cpucycle = 0;
    mainMemory = 3000;
    cpunum = 2;
    undistributtable a = { 300, mainMemory, "unmalloced" };
    undistributtable b = {0, 300, "操作系统"};
    undisttable.push_back(b);
    undisttable.push_back(a);
}

void os::showtable()
{
    qDebug() << "\n---------未分分区表----------" << "\n";
    for(int i = 0; i < undisttable.size(); i++)
    {
        qDebug() << "第" << i + 1 << "片分区\n";
        qDebug() << "起始地址:" << undisttable[i].startadd << "\n";
        qDebug() << "长度:" << undisttable[i].length << "\n";
        qDebug() << "状态:" << undisttable[i].state << "\n\n";
    }
}

bool os::checkpreprogress(Procress* p)
{
    //检查进程p的所有前驱进程是否都已经完成
    bool ok = true;
    vector<QString> pre = p->get_preprogress();
    Procress* pt = endhead->next;
    for (int i = 0; i < pre.size(); i++)
    {
        pt = endhead->next;
        //一次检查每一个前驱
        while (pt != nullptr)
        {
            if (pt->get_pid() == pre[i])
            {
                break;
            }
            pt = pt->next;
        }
        //如果从while循环中退出则说明有前驱未在end队列中
        if (pt == nullptr)
            ok = false;
    }
    return ok;
}

void os::erasevector(int loc)
{
    //删除指定loc位置的vector元素
    int i = 0;
    for (vector<undistributtable>::iterator iter = undisttable.begin(); iter != undisttable.end(); iter++, i++)
    {
        if (i == loc)
        {
            undisttable.erase(iter);
            break;
        }
    }
}

//首次适应算法分配主存空间
bool os::mallocmem(Procress* p)
{
    //cout << "正在为" << p->get_pid() << "进程分配主存空间" << endl;
    //cout << "所需空间" << p->get_space() << endl;
    //针对分区状态表
    bool isok = false;
    int space = p->get_space();
    for (int i = 0; i < undisttable.size(); i++)
    {
        if (undisttable[i].length > space && undisttable[i].state == "unmalloced")
        {
            //如果第i块未分分区大小大于进程所需空间并且该空间未分配
            //则修改其起始地址，修改空间大小
            //并新建一条记录插入表中
            undistributtable prs = { undisttable[i].startadd, space, p->get_pid() };
            undisttable.insert(undisttable.begin() + i, prs);//指定位置插入元素
            undisttable[i+1].length = undisttable[i+1].length - space;
            undisttable[i+1].startadd = undisttable[i+1].startadd + space;
            return true;
        }
        if (undisttable[i].length == space && undisttable[i].state == "unmalloced")
        {
            //如果第i块未分分区等于进程所需空间
            //则直接修改第i块空间的状态
            undisttable[i].state = p->get_pid();
            return true;
        }
    }
    //如果遍历完所有的未分分区表之后还不能
    return false;
}

//回收主存空间
void os::reclaimmem(Procress* p)
{
    /*
    进程完成之后回收主存空间，并与相邻空闲分区合并
    3中情况：
        1.上下分区均不空闲，则该进程所占空间加入到未分分区表中（按起始地址加入
        2.上方分区空闲，则修改上方未分分区记录
        3.下方分区空闲，则修改下方未分分区记录
        4.上下分区均空闲，则修改上方分区记录，删除下方分区记录
    */
    int size = undisttable.size();
    QString pid = p->get_pid();
    //该进程是分区表中第一个记录
    if (size == 1)
    {
        undisttable[0].state = "unmalloced";
        return;
    }
    if (undisttable[0].state == pid)
    {
        if (undisttable[1].state == "unmalloced")
        {
            //如果该进程在分区表中第一条记录中并且下一个记录未分配，则合并并且删除第一个
            undisttable[0].length += undisttable.at(1).length;//长度相加
            undisttable[0].state = "unmalloced";
            erasevector(1);//删除第二个分区
            return;
        }
        else
        {
            //该进程在分区表中第一条并且下一条已分配
            undisttable[0].state = "unmalloced";
            return;
        }
    }
    //该进程是分区表中最后一个进程
    if (undisttable.at(size-1).state == pid)
    {
        if (undisttable[size - 2].state == "unmalloced")
        {
            //最后一个进程且前一个分区未分配，则合并
            undisttable[size - 2].length += undisttable[size - 1].length;
            undisttable[size - 2].state = "unmalloced";
            erasevector(size - 1);
            return;
        }
        else
        {
            //最后一条且上一条已分配
            undisttable[size - 1].state = "unmalloced";
            return;
        }
    }
    for (int i = 1; i < size-1; i++)
    {
        //遍历分区状态表查找分区记录
        if (undisttable[i].state == pid)
        {
            if (undisttable.at(i - 1).state == "unmalloced" && undisttable.at(i + 1).state == "unmalloced")
            {
                //上下分区均空闲
                undisttable[i - 1].length += undisttable[i].length + undisttable[i + 1].length;
                erasevector(i);
                erasevector(i);
                return;
            }
            else if (undisttable.at(i - 1).state == "unmalloced" && undisttable.at(i + 1).state != "unmalloced")
            {
                //只有上方分区空闲
                undisttable[i - 1].length += undisttable[i].length;
                erasevector(i);
                return;
            }
            else if (undisttable.at(i - 1).state != "unmalloced" && undisttable.at(i + 1).state == "unmalloced")
            {
                //只有下方分区空闲
                undisttable[i].length += undisttable[i+1].length;
                undisttable[i].state = "unmalloced";
                erasevector(i+1);
                return;
            }
            else
            {
                //均不空闲
                undisttable[i].state = "unmalloced";
                return;
            }
        }
    }
}

void os::Sort(Procress* head)
{
    Procress* pre, * p, * tail, * temp;
    tail = NULL;
    if (head->next == nullptr)
        return;
    while ((head->next) != tail)//(head->next)!=tail同样适用 ，多执行最后一个步比较
    {
        p = head->next;
        pre = head;
        while (p->next != tail)
        {
            if ((p->get_priority()) < (p->next->get_priority()))
            {
                pre->next = p->next; //交换节点方法二
                temp = p->next->next;
                p->next->next = p;
                p->next = temp;
                p = pre->next;  //p回退一个节点
            }
            p = p->next;  //p再前进一个节点
            pre = pre->next;
        }
        tail = p;
    }
}

//进程调度
//从就绪队列中挑选优先级最高的进程进入运行队列
void os::scheduling()
{
    //此时ready链表第一个节点即为最高优先级的节点
    Procress* pt = readyhead->next;
    Procress* prept = readyhead;
    //如果pt不为空
    //获取运行队列的长度
    pt = runhead->next;
    for (int i = 0; i < cpunum; i++)
    {
        pt = readyhead->next;
        prept = readyhead;
        while (pt != nullptr)
        {
            //如果该进程的前驱已执行，则可对其进行调度，否则调度ready队列中的其他进程
            if(checkpreprogress(pt) == false)
            {
                //前驱未完成
                pt->set_state("block");
                prept->next = pt->next;
                addProcress(pt);
                break;
            }
//            else
//                qDebug()<<pt->get_pid()<<"完成";
            pt->set_state("run");
            prept->next = pt->next;
            addProcress(pt);
            break;

            prept = prept->next;
            pt = pt->next;
        }
    }

}

void os::backscheduling()
{
    //从后备队列中调度优先级最高的作业
    //后备队列首进程分配主存
    Procress* pt = backhead->next;
    Procress* pre = backhead;
    if (pt == nullptr)
        return;
    while(pt != nullptr)
    {
        if (mallocmem(pt) == true)
        {

            //主存空间分配成功,将其从back队列中移除并加入到ready队列中
            pre->next = pt->next;
            pt->set_state("ready");
            addProcress(pt);
            return;

        }
        pre = pre->next;
        pt = pt->next;
    }

}

void os::run()
{
    cpucycle++;//cpu执行周期加一
    //将run队列中的进程运行时间减1，优先权减1
    Procress* pt = runhead->next;
    Procress* prept = runhead;
    int _runtime, _priority;
    while (pt != nullptr)
    {
        _runtime = pt->get_runtime();
        _runtime--;
        pt->set_runtime(_runtime);
        _priority = pt->get_priority();
        if(_priority>0)
            _priority--;
        pt->set_priority(_priority);
        //如果要求运行时间为0，则将其移入结束链表,并回收主存空间
        if (_runtime == 0)
        {
            reclaimmem(pt);//回收主存空间
            pt->set_state("end");
            //将其移出run队列
            prept->next = pt->next;
            addProcress(pt);
        }
        else
        {
            //一个时间片结束后重新排序，进行下轮调度
            pt->set_state("ready");
            prept->next = pt->next;
            addProcress(pt);
        }
        pt = prept->next;
    }
    return_to_ready();
}

void os::return_to_ready()
{
    //判断阻塞队列中的进程其前驱是否完成
    Procress* pt = blockhead->next;
    Procress* prept = blockhead;
    while(pt != nullptr)
    {
        //如果阻塞队列中的某个进程前驱全部完成，则将其加到就绪队列中
        if(checkpreprogress(pt))
        {
            return__(pt);
            prept = prept->next;
            pt = pt->next;
            return;

        }
        else
        {
            qDebug()<<pt->get_pid();
            prept = pt->next;
            pt = pt->next;
            qDebug()<<"继续遍历";
        }

    }
}

void os::return__(Procress* p)
{
    Procress* pt = blockhead;
    while (pt->next != nullptr)
    {
        if (pt->next->get_pid() == p->get_pid())
        {
            //从挂起队列中删除
            pt->next = p->next;
            break;
        }
        else
            pt = pt->next;
    }
    p->set_state("ready");
    addProcress(p);

}

//判断当前进程状态并将其加入对应链表中
void os::addProcress(Procress* p)
{
    //QString _pid = p->get_pid();
    Procress* pt;
    QString _state = p->get_state();
    if (_state == "ready")
    {
        pt = readyhead->next;
        readyhead->next = p;
        p->next = pt;
    }
    else if (_state == "run")
    {
        pt = runhead->next;
        runhead->next = p;
        p->next = pt;
    }
    else if (_state == "back")
    {
        pt = backhead->next;
        backhead->next = p;
        p->next = pt;
    }
    else if (_state == "suspend")
    {
        pt = suspendhead->next;
        suspendhead->next = p;
        p->next = pt;
    }
    else if (_state == "block")
    {
        pt = blockhead->next;
        blockhead->next = p;
        p->next = pt;
    }
    else
    {
        //end
        pt = endhead->next;
        endhead->next = p;
        p->next = pt;
    }
    Sort(readyhead);
    Sort(backhead);
    backscheduling();
}

Procress* os::creatProcress(int _space, QString _pid, int _runtime,
    int _priority, int _attribute, vector<QString> pre)
{
    //检查前驱节点合法性
    for (int i = 0; i < pre.size(); i++)
    {
        if (count(pidvector.begin(), pidvector.end(), pre[i]))
            continue;
        else
        {
            qDebug() << "进程" << pre[i] << "不存在" << "\n";
            return nullptr;
        }
    }
    //检查pid名称合法性
    if (count(pidvector.begin(), pidvector.end(), _pid))
    {
        qDebug() << "进程名：" << _pid << "已经存在，请检查" << "\n";
        return nullptr;
    }
    pidvector.push_back(_pid);
    QString _state = "back";
    Procress* p = new Procress(_space, _pid, _runtime, _priority, _state, _attribute, pre);
    p->set_arrivaltime(cpucycle);
    addProcress(p);
    return p;
}

//挂起进程
void os::suspend(Procress* p)
{
    //对就绪队列中的进程可以挂起
    //先遍历ready队列找到该进程的前驱
    Procress* prept = readyhead;
    while (prept->next != nullptr)
    {
        if (prept->next->get_pid() == p->get_pid())
        {
            //找到p的前驱节点prept
            qDebug() <<"找到需要挂起的节点";
            break;
        }

        else
            prept = prept->next;
    }
    if(prept != nullptr)
        prept->next = p->next;
    p->set_state("suspend");
    qDebug()<<"从ready中删除了改节点";
    reclaimmem(p);
    addProcress(p);
}

//解挂
void os::unsuspend(Procress* p)
{
    //从挂起队列中解挂某一个进程
    //如果分配内存成功，则将其加到ready队列中,否则将其加到back中
    //先遍历挂起队列找到节点的前驱
    Procress* pt = suspendhead;
    while (pt->next != nullptr)
    {
        if (pt->next->get_pid() == p->get_pid())
        {
            //从挂起队列中删除
            pt->next = p->next;
            break;
        }
        else
            pt = pt->next;
    }
    if (mallocmem(p))
    {
        p->set_state("ready");
    }
    else
        p->set_state("back");
    addProcress(p);
}

void os::set_cpunum(int num)
{
    cpunum = num;
}

void os::set_mainMemery(int num)
{
    mainMemory = num;
}

Procress* os::get_readyhead()
{
    return readyhead;
}

Procress* os::get_backhead()
{
    return backhead;
}

Procress* os::get_endhead()
{
    return endhead;
}

Procress* os::get_runhead()
{
    return runhead;
}

Procress* os::get_suspendhead()
{
    return suspendhead;
}

Procress* os::get_blockhead()
{
    return blockhead;
}

vector<undistributtable> os::get_undisttable()
{
    return undisttable;
}

vector<QString> os::get_pidvector()
{
    return pidvector;
}

int os::get_mainmem()
{
    return mainMemory;
}

void os::IO(Procress* p)
{
    //对就绪队列中的进程可以挂起
    //先遍历ready队列找到该进程的前驱
    Procress* prept = runhead;
    while (prept->next != nullptr)
    {
        if (prept->next->get_pid() == p->get_pid())
        {
            break;
        }

        else
            prept = prept->next;
    }
    if(prept != nullptr)
        prept->next = p->next;
    p->set_state("block");
    addProcress(p);
}

void os::endIO(Procress* p)
{
    //对就绪队列中的进程可以挂起
    //先遍历ready队列找到该进程的前驱
    Procress* prept = blockhead;
    while (prept->next != nullptr)
    {
        if (prept->next->get_pid() == p->get_pid())
        {
            break;
        }

        else
            prept = prept->next;
    }
    if(prept != nullptr)
        prept->next = p->next;
    p->set_state("ready");
    addProcress(p);
}
