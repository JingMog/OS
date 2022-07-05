#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QTimer>
#include <QDateTime>
#include <QThread>
#include <QTime>
#include <QRandomGenerator>
#include <QPainter>
#include <QPen>

#include "os.h"

QT_BEGIN_NAMESPACE
namespace Ui { class app; }
QT_END_NAMESPACE

class app : public QMainWindow
{
    Q_OBJECT

public:
    app(QWidget *parent = nullptr);
    ~app();
    void test();
    void showinfo();//在tableView中显示所有的队列信息

private slots:
    void on_button_scheduling_clicked();
    void on_button_run_clicked();
    void on_button_addProgress_clicked();//添加进程
    void Suspend();//菜单栏中挂起槽函数
    void Unsuspend();//菜单栏中的解挂槽函数
    void set_cpunum();//菜单栏中的设置cpunum槽函数
    void Scheduling();//调度槽函数
    void Run();//运行槽函数
    void timestart();//启动定时器，开始调度
    void timestop();//关闭定时器，暂停调度
    void on_button_randomadd_clicked();//随机生成5个进程
    void on_speed_control_sliderMoved(int position);
    void IO();
    void IOend();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::app *ui;
    os* system;
    QStandardItemModel* readyqueue;//就绪队列表格
    QStandardItemModel* backqueue;//后备队列表格
    QStandardItemModel* runqueue;//运行队列表格
    QStandardItemModel* suspendqueue;//挂起队列表格
    QStandardItemModel* endqueue;//结束队列表格
    QStandardItemModel* blockqueue;//阻塞队列
    QStandardItemModel* memallocationqueue;//内存分配情况
    QMenu *readytableMenue;//就绪队列菜单
    QMenu *suspendtableMenue;//挂起队列菜单
    QTimer* timer_scheduling;//定时器
    QTimer* timer_run;//定时器
    QPainter* painter;
    void draw_pre_after();
    void draw_circle(int x, int y, QString pid);
    void draw_line(int lineHStartPos,int lineVStartPos,int lineHEndPos,int lineVEndPos);
    Procress* searchNode(QString pid);
    vector<QString> pid;
};
#endif // APP_H
