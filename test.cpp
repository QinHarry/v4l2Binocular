#include <opencv2/opencv.hpp>
#include "test.h"

using namespace cv;

int main(){

    V4L2cap v4l2cap0 = V4L2cap("/dev/video1");
    V4L2cap v4l2cap1 = V4L2cap("/dev/video2");


    if(v4l2cap0.init_v4l2() == FALSE){
        printf("Init fail~~\n");
        exit(1);
    }

    if(v4l2cap1.init_v4l2() == FALSE){
        printf("Init fail~~\n");
        exit(1);
    }

    printf("second~~\n");
    if(v4l2cap0.v4l2_grab() == FALSE){
        printf("grab fail~~\n");
        exit(2);
    }

    if(v4l2cap1.v4l2_grab() == FALSE){
        printf("grab fail~~\n");
        exit(2);
    }

    v4l2cap0.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2cap0.buf.memory = V4L2_MEMORY_MMAP;
    v4l2cap1.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2cap1.buf.memory = V4L2_MEMORY_MMAP;

//    cvNamedWindow("one",CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("two",CV_WINDOW_AUTOSIZE);
    IplImage *img0,*img1;
    CvMat cvmat0,cvmat1;

    while(1){
        ioctl(v4l2cap0.fd,VIDIOC_DQBUF,&v4l2cap0.buf);
        v4l2cap0.buf.index = 0;
        cvmat0 = cvMat(IMAGEHEIGHT,IMAGEWIDTH,CV_8UC3,(void*)v4l2cap0.buffer);//CV_8UC3
        img0 = cvDecodeImage(&cvmat0,1);
        if(!img0)    printf("No img\n");
        cv::Mat mat_img0(img0);
        cvReleaseImage(&img0);
        ioctl(v4l2cap0.fd,VIDIOC_QBUF,&v4l2cap0.buf);

        ioctl(v4l2cap1.fd,VIDIOC_DQBUF,&v4l2cap1.buf);
        v4l2cap1.buf.index = 0;
        cvmat1 = cvMat(IMAGEHEIGHT,IMAGEWIDTH,CV_8UC3,(void*)v4l2cap1.buffer);//CV_8UC3
        img1 = cvDecodeImage(&cvmat1,1);
        if(!img1)    printf("No img\n");
        cv::Mat mat_img1(img1);
        cvReleaseImage(&img1);
        ioctl(v4l2cap1.fd,VIDIOC_QBUF,&v4l2cap1.buf);

        cv::imshow("one",mat_img0);
        cv::imshow("two",mat_img1);

        if((cvWaitKey(1)&255) == 27)    exit(0);
    }


    ioctl(v4l2cap0.fd,VIDIOC_STREAMOFF,&v4l2cap0.type);
    ioctl(v4l2cap1.fd,VIDIOC_STREAMOFF,&v4l2cap1.type);
    waitKey(0);
    return 0;
}
