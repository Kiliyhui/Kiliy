#include "firstwnd.h"                            //引入第一个界面的头文件
#include "secondwnd.h"                           //引入第二个界面的头文件
#include<QDebug>                                 //信息调试类
#include<QSslSocket>                             //ssl套接字
#include<faceframe.h>                            //第三个界面的头文件
#include <QApplication>

int main(int argc, char *argv[])
{
    //用于测试是否能够进行连接deepseek的
    qDebug() << "支持状态："<<QSslSocket::supportsSsl();          //true
    qDebug() << "编译时："<<QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "运行时：" << QSslSocket::sslLibraryVersionString();


    QApplication a(argc, argv);
    firstwnd f;                       //实例化第一个界面
    f.show();                         //展示第一个界面

    secondwnd s;                      //实例化第二个界面
    //s.show();                         //展示第二个界面

    //将登录界面的跳转信号与第二个界面的槽函数连接
    QObject::connect(&f,                           //信号的发射者
                     &firstwnd::jump,              //发射的信号
                     &s,                           //信号的接受者
                     &secondwnd::jump_slot);       //接受信号后的处理逻辑

    //将人脸登录界面实例化出来
    faceframe F;

    //将登录界面的跳转信号与第三个界面的槽函数连接
    QObject::connect(&f, &firstwnd::jump_face, &F, &faceframe::jump_face_slot);

    //将人脸登录的信号与智能体界面的展示槽函数连接
    QObject::connect(&F, &faceframe::face_jump, &s, &secondwnd::jump_slot);






    return a.exec();
}
