#include "faceframe.h"
#include "ui_faceframe.h"

faceframe::faceframe(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::faceframe)
{
    ui->setupUi(this);
    //给级联分类器装载人脸模型
    if(!c.load("E:\\Opencv\\haarcascade_frontalface_alt2.xml"))
    {
        QMessageBox::information(this, "提示", "级联分类器加载失败");
        return;
    }

}

faceframe::~faceframe()
{
    delete ui;
}



    //定时器函数
void faceframe::timerEvent(QTimerEvent *e)
{
    //判断是否为展示摄像头定时器到位
    if(e->timerId() == cameraTimerId)
    {
        //1、从摄像头中读取一张图像
        camera.read(src);
        //2、将图像进行镜像处理
        flip(src, src, 1);
        //3、将opencv的BGR图转变为RGB图，以便使用
        cvtColor(src, rgb, CV_BGR2RGB);
        //4、将rgb图重新调整大小，以便于在ui界面上展示
        cv::resize(rgb,rgb, Size(800,400));
        //5、将彩色图转变成灰度
        cvtColor(rgb, gray, CV_RGB2GRAY);
        //6、均衡化处理：放大图像的特征
        cv::equalizeHist(gray, dest);
        //7、从均衡化图像上，获取人脸矩形区域
        c.detectMultiScale(dest, faces);
        //8、将人脸矩形框绘制到彩色图上
        for(uint i=0; i<faces.size(); i++)
        {
            rectangle(rgb, faces[i], Scalar(255, 0, 0));
        }
        //此时就得到了Mat类型的处理后的图像
        //9、将opencv的Mat图转为QT中的QPixmap图
        //需要借助QImage图进行转换
        QImage img = QImage(rgb.data,                        //图像的数据
                            rgb.cols,                        //图像的宽度
                            rgb.rows,                        //图像的高度
                            rgb.cols*rgb.channels(),         //每一行的字节数
                            QImage::Format_RGB888);          //图像格式
        //将图像展示到ui界面
        ui->cameraLab->setPixmap(QPixmap::fromImage(img));
    }



    //判断是否为人脸录入的定时器到位
    if(e->timerId() == inputFaceTimerId)
    {
        qDebug() << "人脸录入中，请稍后...";
        //判断摄像头中是否有人脸
        if(faces.empty())      return;         //表示没有人脸，继续处理下一张
        //程序执行至此，表示可以将人脸录入
        Mat face = rgb(faces[0]);              //从rgb图像上截取矩形框框起来的人脸区域
        //1、重新设置大小
        cv::resize(face,face, Size(100,100));
        //2、灰度处理
        cvtColor(face,face, CV_RGB2GRAY);
        //3、均衡化处理
        cv::equalizeHist(face,face);
        //此时将将一张图像处理好了，放入到采集容器中
        input_faces.push_back(face);
        input_labs.push_back(1);                      //脸的编号
        //成功采集一张图像
        count++;
        //4、判断是否采集了50张图像：如果成功采集50张图像，则进行将图像模型转变成数据模型
        if(count==50)
        {
            //将采集的50张图像模型转变成文件模型
            recognizer->update(input_faces, input_labs);
            //保存到本地磁盘中
            recognizer->save("E:\\Opencv\\CH.xml");
            //后续处理工作
            QMessageBox::information(this, "提示", "人脸录入成功");
            //清空容器
            input_faces.clear();
            input_labs.clear();
            count = 0;               //计数器清零
            this->killTimer(inputFaceTimerId);         //关闭定时器

        }

    }



    //判断是否为人脸识别定时器到位
    if(e->timerId() == checkTimerId)
    {
        qDebug() << "人脸检测中，请稍后...";
        //判断是否有人脸
        if(faces.empty())     return;

        //此时说明摄像头中有人脸，可以进行人脸识别
        Mat face = rgb(faces[0]);           //截取图像上的人脸区域

        //1、重新设置尺寸
        cv::resize(face,face,Size(100,100));
        //2、灰度处理
        cvtColor(face,face,CV_RGB2GRAY);
        //3、均衡化处理
        cv::equalizeHist(face,face);
        //4、人脸检测
        int lab = -1;             //接收识别后的人脸标签
        double conf = 0.0;        //检测的值
        recognizer->predict(face, lab, conf);

        qDebug() << "lab = "<<lab<<"   conf = "<<conf;

        //对检测后的结果进行处理
        if(lab!=-1 || conf<=90)
        {
            //人脸检测成功
            //发射一个信号，表示登录成功
            emit face_jump();
            //关闭人脸识别的定时器
            this->killTimer(checkTimerId);
            this->killTimer(cameraTimerId);
            //关闭摄像头
            camera.release();
            //关闭当前界面
            this->close();
        }
     }

}





//一.打开摄像头按钮对应的槽函数
void faceframe::on_openCameraBtn_clicked()
{
    //开启摄像头
    if(!camera.open(0))
    {
        QMessageBox::information(this, "提示", "摄像头启动失败");
        return;
    }

    //程序执行至此，表示摄像头已经成功打开了
    //启动一个定时器，每隔30毫秒从摄像头中拿取一张图像，展示到ui界面上
    cameraTimerId = this->startTimer(30);         //每隔30毫秒，该函数会让系统自动调用timerEvent函数
}

//二.关闭摄像头按钮对应的槽函数
void faceframe::on_closeCameraBtn_clicked()
{
    //关闭定时器
    this->killTimer(cameraTimerId);
    //关闭摄像头
    camera.release();
    //清空摄像头中的内容
    ui->cameraLab->clear();
}

//三.人脸录入按钮对应的槽函数
void faceframe::on_inputFaceBtn_clicked()
{
    //给人脸识别模型创建一个空的对象
    recognizer = LBPHFaceRecognizer::create();

    //启动一个人脸录入的定时器
    inputFaceTimerId = this->startTimer(50);            //每隔50毫秒采集一张图像

    count = 0;              //表示从第一张开始采集
}

//四.人脸登录按钮对应的槽函数
void faceframe::on_pushButton_4_clicked()
{
    //判断人脸文件是否存在
    QFile file("E:\\Opencv\\CH.xml");
    if(!file.exists())
    {
        QMessageBox::information(this, "提示", "人脸模型不存在，请先录入人脸");
        return;
    }

    //1、给人脸识别器加载人脸模型文件：CH.xml
    recognizer = FaceRecognizer::load<LBPHFaceRecognizer>("E:\\Opencv\\CH.xml");
    //2、设置人脸识别的标准
    recognizer->setThreshold(90);            //检测结果低于90说明成功，否则失败
    //3、启动人脸识别的定时器
    checkTimerId = this->startTimer(300);            //每个300毫秒检测一次
}
