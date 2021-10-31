#include "opencvtools.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "daofactory.h"
#include "personentity.h"

//构造函数
OpencvTools::OpencvTools(QObject *parent) : QObject(parent)
{
    //1.打开摄像头
    initVideoCapture();
    //2.创建级联分类器
    initCascadeClassifier();
    //3.创建人脸识别器
    initFaceRecognizer();
    recognizer->setThreshold(70);//设置阈值，只有confidence值小于60的时候，才认为识别到了
}

//析构函数
OpencvTools::~OpencvTools()
{

}

//打开摄像头
void OpencvTools::initVideoCapture()
{
    if(!vc.open(0))
    {
        qDebug()<<"打开摄像头失败!!";
        return;
    }
    qDebug()<<"打开摄像头成功!!";
}

//创建级联分类器
void OpencvTools::initCascadeClassifier()
{
    classifier = CascadeClassifier("E:/opencv3.4-install/install/etc/haarcascades/haarcascade_frontalface_alt2.xml");
    if(classifier.empty())
    {
        qDebug()<<"创建级联分类器失败!!";
        return;
    }
    qDebug()<<"创建级联分类器成功!!";
}

//创建人脸识别器
void OpencvTools::initFaceRecognizer()
{
    QFile file("faces.xml");
    if(file.exists())//如果文件存在，加载
    {
        recognizer = FaceRecognizer::load<LBPHFaceRecognizer>("faces.xml");
    }
    else //如果模型文件不存在，直接创建
    {
        recognizer = LBPHFaceRecognizer::create();
    }
    qDebug()<<"创建人脸识别器成功!!";
}

//从摄像头中读取一帧
Mat OpencvTools::getOneFrame()
{
    vc>>frame;
//    findFace();//找脸
//    drawFace();//画脸
//    blackProcess();//黑化处理
//    qDebug()<<frame.cols; //长度 640
//    qDebug()<<frame.rows; //宽度 480
//    rectangle(frame,findRect,Scalar(255,255,255),2);
    return frame;
}

//对有效框外面的部分，做黑化处理
void OpencvTools::blackProcess()
{
    //将findRect有效矩形范围以外的所有像素点 -50，让其变得黑暗一些
    for(int y = 0; y < frame.rows; y++)
    {
        for(int x = 0; x < frame.cols; x++)
        {
            if(findRect.contains(Point(x,y)))//点在findRect矩形内，不-50
            {
                continue;//结束本次循环
            }
            //point可以得到 b g r 值
            Vec3b point = frame.at<Vec3b>(Point(x,y));
            frame.at<Vec3b>(Point(x,y))[0] = saturate_cast<uchar>(point[0]-50);//b
            frame.at<Vec3b>(Point(x,y))[1] = saturate_cast<uchar>(point[1]-50);//g
            frame.at<Vec3b>(Point(x,y))[2] = saturate_cast<uchar>(point[2]-50);//r
        }
    }
    rectangle(frame,findRect,Scalar(255,255,255),2);//将矩形框用白线画出来
}

//找脸
void OpencvTools::findFace()
{
    //1.找脸，确定找脸的图像，在有效区域的图像内找脸 findRect中
    Mat enable = frame(findRect); //将有效区域的图像截取出来
//    imshow("enable",enable);
    //2.在enable图像中找脸
    cvtColor(enable,enable,CV_BGR2GRAY);//降通道，找脸用灰度图
    classifier.detectMultiScale(enable,faces,1.1,3,0,Size(150,150),Size(200,200));
}

//画脸
void OpencvTools::drawFace()
{
    for(int i = 0; i < faces.size(); i++)
    {
        Rect temp = ChangeFaceRectPos(faces[i]);//先矫正矩形的位置坐标
        rectangle(frame,temp,Scalar(0,0,255),2);//将矫正后的矩形temp显示到frame中
//        qDebug()<<faces[i].x;
//        qDebug()<<faces[i].y;
    }
}

//修改找到脸矩形坐标 在 frame图像中的位置
Rect OpencvTools::ChangeFaceRectPos(const Rect &face)
{
    Rect temp = face;
    temp.x += findRect.x;//findRect.x 有效区左顶点x坐标
    temp.y += findRect.y;//findRect.y 有效区左顶点y坐标
    return temp;
}

//学习人脸
void OpencvTools::study(QString name)
{
    //1.找脸
    findFace();
    //3.画脸
    drawFace();
    //4.黑化处理
    blackProcess();
    if(faces.size() == 0)//如果图像里，没有脸，结束函数
    {
        return;
    }
    //2.将脸从frame中截取下来
    if(studyFaces.size() < STU_COUNT)
    {
        Rect temp = ChangeFaceRectPos(faces[0]);//矫正矩形坐标位置
        Mat aface = frame(temp);//截取
        cvtColor(aface,aface,CV_BGR2GRAY);//降通道
        resize(aface,aface,Size(128,128));//重置图像大小为128 * 128
        studyFaces.push_back(aface);//将截取的人脸放入容器
    }
    else //学习的人脸数量 == STU_COUNT 停止学习
    {
        PersonDao* dao = DaoFactory::getInstance()->getPersonDao();
        PersonEntity e(0,name);
        dao->addPersonInfo(e);//将学习名字存入数据库
        int maxId = -1;
        dao->getMaxID(e,maxId);
        qDebug()<<"maxid is " << maxId;

        for(int i = 0; i < STU_COUNT; i++)
        {
            studylabels.push_back(maxId);
        }
        recognizer->update(studyFaces,studylabels);//更新
        recognizer->save("faces.xml");//生成模型文件
        studyFaces.clear();
        studylabels.clear();
        emit sendStudyFinish();//发送信号
    }

}
//识别人脸
void OpencvTools::check()
{
    //1.找脸
    findFace();
    //2.画脸
    drawFace();
    //3.黑化处理
    blackProcess();
    if(faces.size() == 0)//如果没有人脸，结束函数，不需要识别
        return;
    Rect temp = ChangeFaceRectPos(faces[0]);//矫正矩形位置
    Mat aface = frame(temp);//截取脸
    cvtColor(aface,aface,CV_BGR2GRAY);//降通道
    resize(aface,aface,Size(128,128));//重置大小
    int label = -1;//用来保存识别到的人脸对应的label
    double confidence;//准确度 数值越小越准确
    if(recognizer->empty())//如果识别器中没有模型，结束，不进行预测
    {
        return;
    }
    //进行人脸预测
    recognizer->predict(aface,label,confidence);
    qDebug()<<label << " " << confidence;
    //预测之后，将预测结果发送给MainWindow
    emit sendCheckResult(label);//发送信号

}

