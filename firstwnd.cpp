#include "firstwnd.h"
#include "ui_firstwnd.h"
#include<QMessageBox>           //消息对话框

    //一.UI界面设置
firstwnd::firstwnd(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::firstwnd)
{
    ui->setupUi(this);
    //1.LOGO设置
    ui->LOGOLab->setPixmap(QPixmap(":/pictrue/610bd505d6c22f691cef282fe184b0a.jpg"));        //UI界面设置图片
    ui->LOGOLab->setScaledContents(true);

    //2.对用户名和密码的标签进行设置
    ui->usrNameLab->setPixmap(QPixmap(":/pictrue/userName.jpg"));   //给标签设置图片
    ui->usrNameLab->setScaledContents(true);                        //内容自适应
    ui->pwdLab->setPixmap(QPixmap(":/pictrue/passwd.jpg"));         //密码设置图片
    ui->pwdLab->setScaledContents(true);                            //内容自适应

    //3.对账号和密码输入框进行设置
    ui->usrNameEdit->setPlaceholderText("请输入账号");                //设置占位文本
    ui->usrNameEdit->setAlignment(Qt::AlignCenter);                  //内容居中显示
    ui->pwdEdit->setPlaceholderText("请输入密码");                    //设置占位文本
    ui->pwdEdit->setAlignment(Qt::AlignCenter);                      //内容居中显示
    ui->pwdEdit->setEchoMode(QLineEdit::Password);                  //回显模式问密文
}
firstwnd::~firstwnd()
{
    delete ui;
}

    //二.登录框设置
    //1.该函数是登陆按钮的点击信号对应的槽函数：无论何时登录按钮被点击了，该函数中的内容就立即执行
void firstwnd::on_loginBtn_clicked()
{
    //获取输入的账户和密码
    QString usrname=ui->usrNameEdit->text();        //获取账户
    QString pwd=ui->pwdEdit->text();                //获取密码

    //判断登录逻辑：当账号和密码相等时表示登录成功，否则登录失败
    if(usrname==pwd)
    {
        //登录成功
        QMessageBox::information(this, "提示", "登录成功");

        //向其他界面发送一个信号
        emit jump();

        //关闭当前界面
        this->close();


    }else
    {
        //登录失败
        QMessageBox::information(this, "提示", "账户或密码错误，请重新输入");
        ui->pwdEdit->clear();                    //清除密码输入框中的内容
    }
}


    //2.该函数是取消按钮的点击信号对应的槽函数；取消按钮被点击关闭当前界面
void firstwnd::on_cancelBtn_clicked()
{
    //关闭当前界面
    this->close();
}

    //3.人脸识别按钮对应的槽函数
void firstwnd::on_pushButton_clicked()
{
    //向其他界面发送一个信号
    emit jump_face();

    //将当前界面关闭
    this->close();
}
