#ifndef SECONDWND_H
#define SECONDWND_H

#include <QWidget>
#include<QNetworkAccessManager>                //网络管理员头文件

namespace Ui {
class secondwnd;
}

class secondwnd : public QWidget
{
    Q_OBJECT

public:
    explicit secondwnd(QWidget *parent = nullptr);
    ~secondwnd();

public slots:                 //该权限下定义的都是槽函数
    void jump_slot()
    {
        //将当前界面展示出来
        this->show();
    }


private slots:
    void on_sendBtn_clicked();

private:
    Ui::secondwnd *ui;

    //定义一个网络管理员指针
    QNetworkAccessManager *manager;
};

#endif // SECONDWND_H
