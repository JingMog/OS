
#include "app.h"
#include "./ui_app.h"


app::app(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::app)
{

    ui->setupUi(this);

    painter = new QPainter(this);

    system = new os();
    QStandardItemModel* queue[6] = {};
    queue[0] = readyqueue;
    queue[1] = backqueue;
    queue[2] = runqueue;
    queue[3] = suspendqueue;
    queue[4] = endqueue;
    queue[5] = blockqueue;
    QTableView* Table[6] = {};
    Table[0] = ui->tableReady;
    Table[1] = ui->tableBack;
    Table[2] = ui->tableRun;
    Table[3] = ui->tablesuspend;
    Table[4] = ui->tableEnd;
    Table[5] = ui->tableblock;
    readyqueue = new QStandardItemModel(this);
    backqueue = new QStandardItemModel(this);
    runqueue = new QStandardItemModel(this);
    suspendqueue = new QStandardItemModel(this);
    endqueue = new QStandardItemModel(this);
    blockqueue = new QStandardItemModel(this);
    memallocationqueue = new QStandardItemModel(this);
    ui->tableReady->setModel(readyqueue);//创建readyqueue的table
    ui->tableBack->setModel(backqueue);//创建backqueue的table
    ui->tableRun->setModel(runqueue);//创建runqueue的table
    ui->tablesuspend->setModel(suspendqueue);//创建suspendqueue的table
    ui->tableEnd->setModel(endqueue);//创建endqueue的table
    ui->tableblock->setModel(blockqueue);
    ui->memTable->setModel(memallocationqueue);//创建内存分配情况Table
    memallocationqueue->setColumnCount(3);
    memallocationqueue->setHeaderData(0, Qt::Horizontal, "起始地址");//设置表头
    memallocationqueue->setHeaderData(1, Qt::Horizontal, "长度");//设置表头
    memallocationqueue->setHeaderData(2, Qt::Horizontal, "状态");//设置表头
    ui->memTable->setColumnWidth(0,70);
    ui->memTable->setColumnWidth(1,70);
    ui->memTable->setColumnWidth(2,70);

    blockqueue->setColumnCount(5);
    blockqueue->setHeaderData(0, Qt::Horizontal, "进程名");//设置表头
    blockqueue->setHeaderData(1, Qt::Horizontal, "运行时间");
    blockqueue->setHeaderData(2, Qt::Horizontal, "优先级");
    blockqueue->setHeaderData(3, Qt::Horizontal, "属性");
    blockqueue->setHeaderData(4, Qt::Horizontal, "所需空间");
    suspendqueue->setColumnCount(5);
    readyqueue->setColumnCount(6);
    readyqueue->setHeaderData(0, Qt::Horizontal, "进程名");//设置表头
    readyqueue->setHeaderData(1, Qt::Horizontal, "运行时间");
    readyqueue->setHeaderData(2, Qt::Horizontal, "优先级");
    readyqueue->setHeaderData(3, Qt::Horizontal, "属性");
    readyqueue->setHeaderData(4, Qt::Horizontal, "所需空间");
    readyqueue->setHeaderData(5, Qt::Horizontal, "前驱节点");
    ui->tableReady->setColumnWidth(5, 70);
    backqueue->setColumnCount(6);
    backqueue->setHeaderData(0, Qt::Horizontal, "进程名");//设置表头
    backqueue->setHeaderData(1, Qt::Horizontal, "运行时间");
    backqueue->setHeaderData(2, Qt::Horizontal, "优先级");
    backqueue->setHeaderData(3, Qt::Horizontal, "属性");
    backqueue->setHeaderData(4, Qt::Horizontal, "所需空间");
    backqueue->setHeaderData(5, Qt::Horizontal, "前驱节点");
    ui->tableBack->setColumnWidth(5, 70);
    runqueue->setColumnCount(5);
    runqueue->setHeaderData(0, Qt::Horizontal, "进程名");//设置表头
    runqueue->setHeaderData(1, Qt::Horizontal, "运行时间");
    runqueue->setHeaderData(2, Qt::Horizontal, "优先级");
    runqueue->setHeaderData(3, Qt::Horizontal, "属性");
    runqueue->setHeaderData(4, Qt::Horizontal, "所需空间");
    suspendqueue->setColumnCount(5);
    suspendqueue->setHeaderData(0, Qt::Horizontal, "进程名");//设置表头
    suspendqueue->setHeaderData(1, Qt::Horizontal, "运行时间");
    suspendqueue->setHeaderData(2, Qt::Horizontal, "优先级");
    suspendqueue->setHeaderData(3, Qt::Horizontal, "属性");
    suspendqueue->setHeaderData(4, Qt::Horizontal, "所需空间");
    endqueue->setColumnCount(5);
    endqueue->setHeaderData(0, Qt::Horizontal, "进程名");//设置表头
    endqueue->setHeaderData(1, Qt::Horizontal, "运行时间");
    endqueue->setHeaderData(2, Qt::Horizontal, "优先级");
    endqueue->setHeaderData(3, Qt::Horizontal, "属性");
    endqueue->setHeaderData(4, Qt::Horizontal, "所需空间");
    for(int i = 0; i < 6; i++)
    {
        Table[i]->verticalHeader()->hide();//隐藏左边一栏
        Table[i]->setColumnWidth(0,50);
        Table[i]->setColumnWidth(1,70);
        Table[i]->setColumnWidth(2,50);

        Table[i]->setColumnWidth(3,50);
        Table[i]->setColumnWidth(4,70);
        Table[i]->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    ui->memTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->memTable->verticalHeader()->hide();

    //右键菜单实现
    readytableMenue = new QMenu(ui->tableReady);//就绪队列菜单
    suspendtableMenue = new QMenu(ui->tablesuspend);//就绪队列菜单
    ui->tableReady->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tablesuspend->setContextMenuPolicy(Qt::CustomContextMenu);

    //菜单栏的实现
    //QMenuBar
    connect(ui->actionSuspend, SIGNAL(triggered()), this, SLOT(Suspend()));
    connect(ui->actionUnsuspend, SIGNAL(triggered()), this, SLOT(Unsuspend()));
    connect(ui->action_setcpunum, SIGNAL(triggered()), this, SLOT(set_cpunum()));
//    connect(ui->actionIO, SIGNAL(triggered()), this, SLOT(IO()));
//    connect(ui->actionIOend, SIGNAL(triggered()), this, SLOT(IOend()));

    //定时器的实现
    timer_scheduling = new QTimer();
    timer_scheduling->setInterval(1000);//设置调用时间间隔为1秒
    timer_run = new QTimer();
    timer_run->setInterval(1000);//设置调用时间间隔为1s
    connect(timer_scheduling, SIGNAL(timeout()), this, SLOT(Scheduling()));//连接定时器
    connect(timer_run, SIGNAL(timeout()), this, SLOT(Run()));//连接定时器
    connect(ui->button_beginScheduling, SIGNAL(clicked()), this, SLOT(timestart()));//连接开始调度按钮
    connect(ui->button_stopScheduling, SIGNAL(clicked()), this, SLOT(timestop()));//连接暂停调度按钮

    //设置速度控制滑块
    ui->speed_control->setMaximum(2000);
    ui->speed_control->setMinimum(100);

}


app::~app()
{
    delete ui;
}


void app::test()
{
    vector<QString> v2 = { "1" };
    vector<QString> v3 = { "1", "2"};
    vector<QString> v4 = { "2", "3"};
    Procress* p1 = system->creatProcress(300, "1", 1, 5, 0);
    if (p1 == nullptr)
        qDebug() << "进程创建失败,前驱或进程名称错误\n";
    Procress* p2 = system->creatProcress(200, "2", 1, 2, 0);
    if (p2 == nullptr)
        qDebug() << "进程创建失败,前驱或进程名称错误\n";
    Procress* p3 = system->creatProcress(500, "3", 1, 5, 1, v3);
    if (p3 == nullptr)
        qDebug() << "进程创建失败,前驱或进程名称错误\n";
    Procress* p4 = system->creatProcress(600, "4", 1, 4, 1, v4);
    if (p4 == nullptr)
        qDebug() << "进程创建失败,前驱或进程名称错误\n";
    Procress* p5 = system->creatProcress(200, "5", 3, 1, 0);
    if (p5 == nullptr)
        qDebug() << "进程创建失败,前驱或进程名称错误\n";
    Procress* p6 = system->creatProcress(500, "6", 1, 8, 0);
    if (p6 == nullptr)
        qDebug() << "进程创建失败,前驱或进程名称错误\n";
    Procress* p7 = system->creatProcress(300, "7", 2, 1, 0);
    if (p7 == nullptr)
        qDebug() << "进程创建失败,前驱或进程名称错误\n";
    Procress* p8 = system->creatProcress(300, "8", 2, 1, 0);
    Procress* p9 = system->creatProcress(300, "9", 2, 1, 0);
    showinfo();
    //system->showtable();
}

void app::showinfo()
{
    int row = 0;
    Procress* head[6] = {};
    head[0] = system->get_readyhead()->next;
    head[1] = system->get_backhead()->next;
    head[2] = system->get_runhead()->next;
    head[3] = system->get_suspendhead()->next;
    head[4] = system->get_endhead()->next;
    head[5] = system->get_blockhead()->next;
    QStandardItemModel* queue[6] = {};
    queue[0] = readyqueue;
    queue[1] = backqueue;
    queue[2] = runqueue;
    queue[3] = suspendqueue;
    queue[4] = endqueue;
    queue[5] = blockqueue;
    Procress* pt;
    for(int i = 0; i < 6; i++)
    {
        row = 0;
        queue[i]->removeRows(0, queue[i]->rowCount());//清空该表的所有记录
        pt = head[i];
        while(pt!=nullptr)
        {
            pt->show(queue[i], row);
            row++;
            pt = pt->next;
        }
    }
    vector<undistributtable> undist = system->get_undisttable();
    memallocationqueue->removeRows(0, memallocationqueue->rowCount());//清除内存表中的记录
    for(int i = 0; i < undist.size(); i++)
    {
        memallocationqueue->setItem(i, 0, new QStandardItem(QString::number(undist[i].startadd)));
        memallocationqueue->setItem(i, 1, new QStandardItem(QString::number(undist[i].length)));
        QString _state;
        if(undist[i].state == "unmalloced")
            _state = "空闲";
        else
            _state = undist[i].state;
        memallocationqueue->setItem(i, 2, new QStandardItem(_state));
        if(_state == "空闲")
        {
            memallocationqueue->item(i,0)->setBackground(QColor(240,249,235));
            memallocationqueue->item(i,1)->setBackground(QColor(240,249,235));
            memallocationqueue->item(i,2)->setBackground(QColor(240,249,235));
        }
        else
        {
            memallocationqueue->item(i,0)->setBackground(QColor(254,245,230));
            memallocationqueue->item(i,1)->setBackground(QColor(254,245,230));
            memallocationqueue->item(i,2)->setBackground(QColor(254,245,230));
        }
        memallocationqueue->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        memallocationqueue->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        memallocationqueue->item(i, 2)->setTextAlignment(Qt::AlignCenter);
    }
    //更新内存使用情况
    vector<undistributtable> table = system->get_undisttable();
    double use = 0;
    int mem_sum = system->get_mainmem();
    for(int i = 0; i < table.size(); i++)
    {
        if(table[i].state != "unmalloced")
            use += table[i].length;
    }
    double use_rate = use/3300 * 100;

    ui->progressBar_free->setValue(100-use_rate);
    ui->progressBar_memuse->setValue(use_rate);
}


void app::on_button_scheduling_clicked()
{
    system->scheduling();
    showinfo();
}


void app::on_button_run_clicked()
{
    system->run();
    showinfo();
}


//菜单栏中挂起槽函数实现
void app::Suspend()
{
    bool ok;
    QString pid = QInputDialog::getText(this, "进程挂起",
                 "请输入需要挂起的进程pid", QLineEdit::Normal, "", &ok);
    if(ok)
        qDebug()<<pid;
    Procress* pt = system->get_readyhead()->next;
    while(pt != nullptr)
    {
        if(pid == pt->get_pid())
        {
            system->suspend(pt);
            showinfo();
            return;
        }
        else
            pt = pt->next;

    }
    if(pt == nullptr)
        QMessageBox::critical(this, "输入信息非法", "该进程不存在");
}

void app::Unsuspend()
{
    bool ok;
    QString pid = QInputDialog::getText(this, "进程解挂",
                 "请输入需要解挂的进程pid", QLineEdit::Normal, "", &ok);
    if(ok)
        qDebug()<<pid;
    Procress* pt = system->get_suspendhead()->next;
    while(pt != nullptr)
    {
        if(pid == pt->get_pid())
        {
            system->unsuspend(pt);
            showinfo();
            break;
        }
        else
            pt = pt->next;

    }
    if(pt == nullptr)
        QMessageBox::critical(this, "输入信息非法", "该进程不存在");
}

void app::timestart()
{
    QMessageBox::information(this, "开始调度", "进程调度已开始");
    timer_scheduling->start();//开始定时
    QThread::msleep(500);
    timer_run->start();
}

void app::timestop()
{
    timer_scheduling->stop();//暂停定时器
    timer_run->stop();
    QMessageBox::information(this, "暂停调度", "进程调度已暂停");
}

void app::Scheduling()
{
    system->scheduling();
    showinfo();
}

void app::Run()
{
    system->run();
    showinfo();
}

void app::on_button_addProgress_clicked()
{
    QString pid = ui->input_pid->text();
    int space = ui->input_space->text().toInt();
    int runtime = ui->input_runtime->text().toInt();
    int priority = ui->input_priority->text().toInt();
    int attribute = ui->input_attribute->currentIndex();
    QString preprogress = ui->input_preprogress->text();
    QStringList _inputpre = preprogress.split(";");
    vector<QString> inputpre;
    for(int i = 0; i < _inputpre.size(); i++)
        inputpre.push_back(_inputpre[i]);
    //绘制下拉复选框
    //检查是否重名
    vector<QString> pidvector = system->get_pidvector();
    if(count(pidvector.begin(), pidvector.end(), pid))
        QMessageBox::critical(this, "输入信息非法", "进程pid已经存在");
    else if(space <= 0)
        QMessageBox::critical(this, "输入信息非法", "进程所需空间不得小于0");
    else if(runtime <= 0)
        QMessageBox::critical(this, "输入信息非法", "进程运行时间不得小于0");
    else if(priority <= 0)
        QMessageBox::critical(this, "输入信息非法", "进程优先级不得小于0");
    else if(attribute == 1)
    {
        //检查前驱节点是否合法
        for (int i = 0; i < inputpre.size(); i++)
        {
            if (count(pidvector.begin(), pidvector.end(), inputpre[i]))
                continue;
            else
            {
                QString a = "进程" + inputpre[i] + "不存在";
                QMessageBox::critical(this, "输入信息非法", a);
            }
        }
        system->creatProcress(space,pid,runtime,priority,attribute,inputpre);
        showinfo();
    }
    else
        if(attribute == 0)
        {
            system->creatProcress(space,pid,runtime,priority,attribute);
            showinfo();
        }
}



void app::on_button_randomadd_clicked()
{
    //随机生成五个进程
    vector<QString> pid = system->get_pidvector();
    int num = 0;
    int i = 0;
    while(num < 5)
    {
        int _name = QRandomGenerator::global()->bounded(200);
        QString name = QString::number(_name);
        for(i = 0; i < pid.size(); i++)
        {
            if(name == pid[i])
                break;
        }
        if(i >= pid.size())
        {
            //没有重复的
            int space = QRandomGenerator::global()->bounded(50);
            space = space * 10;
            int priority = QRandomGenerator::global()->bounded(10);
            int runtime = QRandomGenerator::global()->bounded(9);
            runtime++;//保证runtime大于0
            int attribute = 0;
            system->creatProcress(space,name,runtime,priority,attribute);
            num++;
        }
    }
    showinfo();
}

void app::set_cpunum()
{
    int num = 1;
    bool ok;
    QString _num = QInputDialog::getText(this, "设置CPU数量",
                     "请输入CPU数量", QLineEdit::Normal, "", &ok);
    if(ok)
        qDebug()<<_num;
    num = _num.toInt();
    if(num <= 0)
        QMessageBox::critical(this, "输入信息非法", "CPU数量不得小于0");
    else
        system->set_cpunum(num);
}

//设置执行速度
void app::on_speed_control_sliderMoved(int position)
{
    timer_run->setInterval(2000-position);
    timer_scheduling->setInterval(2000-position);
}


void CalcVertexes(double startX, double startY, double endX, double endY, double& x1, double& y1, double& x2, double& y2)
{
    /*
     * @brief 求得箭头两点坐标
     */

    double arrowLength = 10;      // 箭头长度，一般固定
    double arrowDegrees = 0.5;    // 箭头角度，一般固定

    // 求 y / x 的反正切值
    double angle = atan2(endY - startY, endX - startX) + 3.1415926;

    // 求得箭头点 1 的坐标
    x1 = endX + arrowLength * cos(angle - arrowDegrees);
    y1 = endY + arrowLength * sin(angle - arrowDegrees);

    // 求得箭头点 2 的坐标
    x2 = endX + arrowLength * cos(angle + arrowDegrees);
    y2 = endY + arrowLength * sin(angle + arrowDegrees);
}

void app::paintEvent(QPaintEvent *event)
{
    draw_pre_after();
    update();
}

Procress* app::searchNode(QString pid)
{
    Procress* pt = system->get_readyhead()->next;
    while(pt!=nullptr)
    {
        if(pt->get_pid() == pid)
            break;
        else
            pt = pt->next;
    }
    if(pt==nullptr)
        pt = system->get_runhead()->next;
    while(pt!=nullptr)
    {
        if(pt->get_pid() == pid)
            break;
        else
            pt = pt->next;
    }
    if(pt==nullptr)
        pt = system->get_endhead()->next;
    while(pt!=nullptr)
    {
        if(pt->get_pid() == pid)
            break;
        else
            pt = pt->next;
    }
    if(pt==nullptr)
        pt = system->get_backhead()->next;
    while(pt!=nullptr)
    {
        if(pt->get_pid() == pid)
            break;
        else
            pt = pt->next;
    }
    if(pt==nullptr)
        pt = system->get_suspendhead()->next;
    while(pt!=nullptr)
    {
        if(pt->get_pid() == pid)
            break;
        else
            pt = pt->next;
    }
    if(pt==nullptr)
        pt = system->get_blockhead()->next;
    while(pt!=nullptr)
    {
        if(pt->get_pid() == pid)
            return pt;
        else
            pt = pt->next;
    }
    return pt;

}


void app::draw_pre_after()
{
    //先绘制节点
    pid = system->get_pidvector();//节点向量
    for(int i = 0; i < pid.size(); i++)
    {
        int row,col,x,y;
        row = i / 5;
        col = i % 5;
        if(row % 2 == 0)
            x = 1050 + 50 * col;
        else
            x = 1075 + 50 * col;
        if(col % 2 == 0)
            y = 390 + 50 * row;
        else
            y = 365 + 50 * row;
        Procress* pt = searchNode(pid[i]);
        vector<QString> preprogress = pt->get_preprogress();
        for(int j = 0; j < preprogress.size(); j++)
        {
            //连接节点i与其前驱j
            //x,y为当前节点的位置
            //找到前驱节点在pid中的位置
            int m = 0;
            for(m = 0; m < pid.size(); m++)
                if(pid[m] == preprogress[j])
                    break;
            int mx,my;
            row = m / 5;
            col = m % 5;
            if(row % 2 == 0)
                mx = 1050 + 50 * col;
            else
                mx = 1075 + 50 * col;
            if(col % 2 == 0)
                my = 390 + 50 * row;
            else
                my = 365 + 50 * row;
            draw_line(x+5,y+3,mx+5,my+3);
        }
        draw_circle(x,y,pid[i]);

    }
    //再绘制节点直接的关系
}

void app::draw_circle(int x, int y, QString pid)
{
    painter->begin(this);
    QPen pen;
    pen.setColor(QColor(47,69,84));
    pen.setWidth(13);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawEllipse(x,y,13,13);
    pen.setColor(QColor(255,255,255));
    painter->setPen(pen);
    painter->drawText(x-3,y+8,pid);
    painter->end();
}

void app::draw_line(int lineHStartPos,int lineVStartPos,int lineHEndPos,int lineVEndPos)
{

    painter->begin(this);
    QPen pen;
    pen.setColor(QColor(61,157,255));
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing, true);
    QLineF line(lineHStartPos, lineVStartPos, lineHEndPos, lineVEndPos);
    painter->drawLine(line);
    // 箭头的两点坐标
    double x1, y1, x2, y2;
    // 求得箭头两点坐标
    CalcVertexes(lineHStartPos, lineVStartPos, lineHEndPos, lineVEndPos, x1, y1, x2, y2);
    painter->drawLine(lineHEndPos, lineVEndPos, x1, y1); // 绘制箭头一半
    painter->drawLine(lineHEndPos, lineVEndPos, x2, y2); // 绘制箭头另一半
    painter->end();
}

void app::IO()
{
    bool ok;
    QString pid = QInputDialog::getText(this, "进程IO",
                 "请输入需要IO的进程pid", QLineEdit::Normal, "", &ok);
    if(ok)
        qDebug()<<pid;
    Procress* pt = system->get_runhead()->next;
    while(pt != nullptr)
    {
        if(pid == pt->get_pid())
        {
            system->IO(pt);
            showinfo();
            break;
        }
        else
            pt = pt->next;

    }
    if(pt == nullptr)
        QMessageBox::critical(this, "输入信息非法", "该进程不存在");
}

void app::IOend()
{
    bool ok;
    QString pid = QInputDialog::getText(this, "IO结束",
                 "请输入结束IO的进程pid", QLineEdit::Normal, "", &ok);
    if(ok)
        qDebug()<<pid;
    Procress* pt = system->get_blockhead()->next;
    while(pt != nullptr)
    {
        if(pid == pt->get_pid())
        {
            system->endIO(pt);
            showinfo();
            break;
        }
        else
            pt = pt->next;

    }
    if(pt == nullptr)
        QMessageBox::critical(this, "输入信息非法", "该进程不存在");
}
