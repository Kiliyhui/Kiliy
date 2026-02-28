#include "secondwnd.h"
#include "ui_secondwnd.h"
#include<QNetworkRequest>             //数据请求头文件
#include<QNetworkReply>               //数据响应头文件
#include<QJsonArray>                  //json数组头文件
#include<QJsonDocument>               //json文档头文件
#include<QJsonObject>                 //json对象头文件
#include<QJsonParseError>             //json错误头文件
#include<QDebug>                      //信息调试类
#include<QUrl>                        //网址的头文件

secondwnd::secondwnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::secondwnd)
{
    ui->setupUi(this);

    //实例化管理员
    manager = new QNetworkAccessManager;          //就有了一个网络管理员，以后跟deepseek联系就用他

    //设置ui界面的的聊天框只读
    ui->chatMsgBox->setReadOnly(true);
}

secondwnd::~secondwnd()
{
    delete ui;
}

void secondwnd::on_sendBtn_clicked()
{
    //1、将ui界面上的问题获取下来
    QString text = ui->sendMsgBox->toPlainText();
    ui->sendMsgBox->clear();                        //清空输入文本

    //2、将消息放到聊天界面
    ui->chatMsgBox->insertPlainText("--->");
    ui->chatMsgBox->insertPlainText(text);          //展示文本
    ui->chatMsgBox->insertPlainText("\n\n");        //问答之间用空格隔开

    //3、将自己要发送的消息进行封包处理
    QNetworkRequest request;                  //请求对象包
    //设置包头
    request.setUrl(QUrl("https://api.deepseek.com/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Authorization", "Bearer sk-734d579c789441d0ad49b29a5021f156");

    //设置角色
    QJsonArray message;                      //定义角色数组
    //定义系统对象
    QJsonObject systemMsg;
    systemMsg["role"] = "system";     //角色名
    systemMsg["content"] = "You are a helpful assistant";   //角色描述
    //定义用户对象
    QJsonObject userMsg;
    userMsg["role"] = "user";          //角色名
    userMsg["content"] = text;         //用户的问题
    //将两个对象放入数组中
    message.append(systemMsg);
    message.append(userMsg);

    //设置其他属性数据
    QJsonObject requestBody;
    requestBody["messages"] = message;             //角色
    requestBody["model"] = "deepseek-chat";        //模式
    requestBody["max_tokens"] = 2048;              //回复的最大字数1024
    requestBody["stream"] = true;                  //以字节流的形式回复
    requestBody["temperature"] = 1;                //温度

    //4、将上面的json对象发送给deepseek   使用网络管理员发送
    //当消息发送给deepseek处理后，回复的消息也是由该管理员的返回值接受
    QNetworkReply *reply =  manager->post(request, QJsonDocument(requestBody).toJson());

    //5、处理回复的消息：当reply对象中有消息回复时，该对象就会自动发射一个readyRead信号
    //我们可以将该信号连接到自定义的处理逻辑中处理相关内容
    connect(reply, &QNetworkReply::readyRead, this, [=](){
        //将收到的消息进行处理
        //判断是否能够读取一行信息
        if(reply->canReadLine())
        {
            //能读取一行，将消息放入变量中
            QString line = reply->readLine().trimmed();        //将消息读取出来并且将\n删除

            //对收到的一行消息处理
            if(line.startsWith("data: "))
            {
                //说明是回复的消息
                line.remove(0, 6);              //将消息的无用内容删除
                //解析json数据
                QJsonParseError error;          //如果消息解析错误，将错误信息放入该变量中
                QJsonDocument doc  =  QJsonDocument::fromJson(line.toUtf8(), &error);   //将字符串解析成json文档
                if(error.error == QJsonParseError::NoError)
                {
                    QString content = doc.object()["choices"].toArray().first().toObject()["delta"].toObject()["content"].toString();

                    //判断内容是否为空
                    if(!content.isEmpty())
                    {
                        //将聊天框的光标放到消息最后
                        ui->chatMsgBox->moveCursor(QTextCursor::End);
                        //将消息放入到聊天框中
                        ui->chatMsgBox->insertPlainText(content);
                    }
                }

            }
        }

    });

    //6、当接收消息结束后，reply会自动发射一个finished信号，我们可以在该信号后处理相关逻辑
    connect(reply, &QNetworkReply::finished, this, [=](){
        //将聊天框的光标放到消息最后
        ui->chatMsgBox->moveCursor(QTextCursor::End);
        ui->chatMsgBox->insertPlainText("\n\n\n");
        reply->deleteLater();              //销毁空间
    });

}
