#ifndef FACEFRAME_H
#define FACEFRAME_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include<opencv2/face.hpp>
#include <vector>
#include <map>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTimerEvent>
#include<QtSerialPort/QtSerialPort>
#include<QtSerialPort/QSerialPortInfo>
using namespace  cv;
using namespace cv::face;
using namespace std;

namespace Ui {
class faceframe;
}

class faceframe : public QWidget
{
    Q_OBJECT

public:
    explicit faceframe(QWidget *parent = nullptr);
    ~faceframe();


signals:
    void face_jump();             //向其他界面发送人脸跳转信号



    //定义接收跳转信号的槽函数
public slots:
    void jump_face_slot()
    {
        this->show();
    }


private slots:
    void on_openCameraBtn_clicked();

    void on_closeCameraBtn_clicked();

    void on_inputFaceBtn_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::faceframe *ui;

    /******************************定义有关展示摄像头的相关变量************************/
    VideoCapture camera;                                //用于管理摄像头的变量
    Mat src;                                            //存储从摄像头中拿取的任意一张原始图像
    Mat rgb;                                            //存储rgb图，让qt能够识别
    Mat gray;                                           //用于存储灰度图
    Mat dest;                                           //存储均衡化图像：该图像会将图像中的特征放大并记录
    int cameraTimerId;                                  //展示人脸的定时器
    void timerEvent(QTimerEvent *e);                    //用于展示人脸的定时器
    CascadeClassifier c;                                //级联分类器，用来找到图像中的人脸矩形框
    vector<Rect> faces;                                 //存储图像上的人脸矩形框

    /****************************定义有关人脸录入的相关变量***************************/
    Ptr<LBPHFaceRecognizer> recognizer;                 //用于人脸录入和识别的对象
    vector<Mat> input_faces;                            //用于存储采集的要录入的人脸
    vector<int> input_labs;                             //存储要录入的人脸编号
    int inputFaceTimerId;                               //人脸录入的定时器
    int count ;                                         //记录采集的人脸个数

    /*****************************定义有关人脸识别的相关变量*****************************/
    int checkTimerId;                                  //人脸识别的定时器

};

#endif // FACEFRAME_H
