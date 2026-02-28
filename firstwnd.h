#ifndef FIRSTWND_H
#define FIRSTWND_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class firstwnd; }
QT_END_NAMESPACE

class firstwnd : public QWidget
{
    Q_OBJECT

public:
    firstwnd(QWidget *parent = nullptr);
    ~firstwnd();

signals:                     //该权限下定义的都是信号函数
    void jump();             //定义一个跳转信号

    void jump_face();         //向人脸识别界面跳转的信号



private slots:
    void on_loginBtn_clicked();

    void on_cancelBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::firstwnd *ui;
};
#endif // FIRSTWND_H
