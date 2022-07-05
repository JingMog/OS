#include "Procress.h"
#include<iostream>

using namespace std;

//构造函数
Procress::Procress()
{
    this->space = 0;
    this->pid = "unamed";
    this->state = "back";
    this->priority = 0;
    this->attribute = 0;//默认为同步进程
}

//构造函数重载
Procress::Procress(int _space, QString _pid, int _runtime,
    int _priority, QString _state, int _attribute, vector<QString> pre)
{
    this->space = _space;
    this->pid = _pid;
    this->runtime = _runtime;
    this->arrivaltime = 0;
    this->priority = _priority;
    this->state = _state;
    this->attribute = _attribute;
    this->preprogress = pre;
}

void Procress::set_Procress(int _space, QString _pid, int _runtime,int _arrivaltime,
    int _priority, QString _state, int _attribute)
{
    this->space = _space;
    this->pid = _pid;
    this->runtime = _runtime;
    this->arrivaltime = _arrivaltime;
    this->priority = _priority;
    this->state = _state;
    this->attribute = _attribute;
}

void Procress::show_Procress()
{
    qDebug() << "进程名称" << pid << "  " << "运行时间:" << runtime << "  "
             << "到达时间:" << arrivaltime << "  "
             << "优先级:" << priority << "  "
             << "状态:" << state << "  "
             << "属性:" << attribute << "  "
             << "所需空间:" << space << "  "
             << "进程前驱节点:";
    if (preprogress.size() == 0)
        qDebug() << "无";
    for (int i = 0; i < preprogress.size(); i++)
        qDebug() << preprogress[i] << " ";
}

int Procress::get_priority()
{
    return this->priority;
}

QString Procress::get_state()
{
    return this->state;
}


void Procress::set_state(QString _state)
{
    state = _state;
}

int Procress::get_runtime()
{
    return runtime;
}

void Procress::set_runtime(int _runtime)
{
    runtime = _runtime;
}

void Procress::set_priority(int _priority)
{
    priority = _priority;
}

void Procress::set_arrivaltime(int _arrivaltime)
{
    arrivaltime = _arrivaltime;
}

int Procress::get_arrivaltime()
{
    return arrivaltime;
}

QString Procress::get_pid()
{
    return pid;
}

int Procress::get_attribute()
{
    return attribute;
}

int Procress::get_space()
{
    return space;
}

void Procress::set_pid(QString _pid)
{
    pid = _pid;
}

void Procress::set_attribute(int _attribute)
{
    attribute = _attribute;
}

void Procress::set_space(int _space)
{
    space = _space;
}

vector<QString> Procress::get_preprogress()
{
    return preprogress;
}

void Procress::show(QStandardItemModel* model,int row)
{
    model->setItem(row, 0, new QStandardItem(pid));
    model->setItem(row, 1, new QStandardItem(QString::number(runtime)));
    model->setItem(row, 2, new QStandardItem(QString::number(priority)));
    model->setItem(row, 3, new QStandardItem(QString::number(attribute)));
    model->setItem(row, 4, new QStandardItem(QString::number(space)));
    if(state == "back" || state == "ready")
    {
        //就绪队列和后备队列显示前驱节点
        if(preprogress.size() == 0)
            model->setItem(row, 5, new QStandardItem("无"));
        else
        {
            QString pre = "";
            for(int i = 0; i < preprogress.size(); i++)
                pre = pre + preprogress[i] + ", ";
            model->setItem(row, 5, new QStandardItem(pre));
        }
        model->item(row, 5)->setTextAlignment(Qt::AlignCenter);
        if(row % 2 == 0)
            model->item(row,5)->setBackground(QColor(222,234,246));
    }
    if(row % 2 == 0)
    {
        model->item(row,0)->setBackground(QColor(222,234,246));
        model->item(row,1)->setBackground(QColor(222,234,246));
        model->item(row,2)->setBackground(QColor(222,234,246));
        model->item(row,3)->setBackground(QColor(222,234,246));
        model->item(row,4)->setBackground(QColor(222,234,246));
    }
    model->item(row, 0)->setTextAlignment(Qt::AlignCenter);
    model->item(row, 1)->setTextAlignment(Qt::AlignCenter);
    model->item(row, 2)->setTextAlignment(Qt::AlignCenter);
    model->item(row, 3)->setTextAlignment(Qt::AlignCenter);
    model->item(row, 4)->setTextAlignment(Qt::AlignCenter);

}
