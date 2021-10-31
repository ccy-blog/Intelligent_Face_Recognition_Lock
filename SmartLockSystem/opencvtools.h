#ifndef OPENCVTOOLS_H
#define OPENCVTOOLS_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <QDebug>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::face;

#define STU_COUNT 5  //学习人脸的数量


class OpencvTools : public QObject
{
    Q_OBJECT
public:
    explicit OpencvTools(QObject *parent = 0);
    ~OpencvTools();
//信号
signals:
    void sendStudyFinish();//学习完成的信号
    void sendCheckResult(int label);//将预测的结果发送给MainWindow

//槽函数
public slots:

public:
    //成员函数
    void initVideoCapture();//初始化VideoCapture 实现打开摄像头
    void initCascadeClassifier();//创建级联分类器
    void initFaceRecognizer();//创建人脸识别器
    Mat getOneFrame();//读取一帧
    void blackProcess();//黑化处理
    void findFace();//找脸
    void drawFace();//画脸
    Rect ChangeFaceRectPos(const Rect& face);//由于找脸是在有效区的图像内找，但是画脸是在frame中
    void study(QString name);//开始学习，录入人脸，生成模型
    void check();//用于识别人脸方法

public:
    //成员变量
    VideoCapture vc;//视频类对象
    Mat frame; //用来保存从摄像头中读取出的一帧
    //(640-200)/2 == 220 x     200矩形的长度
    //(480-200)/2 == 140 y     200矩形的宽度
    Rect findRect = Rect(220, 140, 200, 200);//220 140 左顶点xy坐标 找脸的有效矩形范围
    CascadeClassifier classifier;//级联分类器，用来找脸
    vector<Rect> faces;//用来保存找到的多张人脸的矩形容器 vector
    Ptr<FaceRecognizer> recognizer;//人脸识别器，用来判断是谁的
    vector<Mat> studyFaces;//用来保存学习的人脸，用于生成模型
    vector<int> studylabels;//用来保存学习人脸对应的label



};

#endif // OPENCVTOOLS_H
