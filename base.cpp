#include "test.h"

V4L2cap::V4L2cap(const char* file_video){
    this->file_video = file_video;
}

V4L2cap:: ~V4L2cap(){

}

int V4L2cap::init_v4l2(){
    std::cout<<11111<<std::endl;
    if ((fd = open(this->file_video, O_RDWR)) == -1){
        printf("Opening video device error\n");
        return FALSE;
    }
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1){
            printf("unable Querying Capabilities\n");
            return FALSE;
    }
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("Support format: \n");
    while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc) != -1){
        printf("\t%d. %s\n",fmtdesc.index+1,fmtdesc.description);
        fmtdesc.index++;
    }
    //set fmt
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = IMAGEWIDTH;
    fmt.fmt.pix.height = IMAGEHEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG; //*************************V4L2_PIX_FMT_YUYV****************
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1){
        printf("Setting Pixel Format error\n");
        return FALSE;
    }
    if(ioctl(fd,VIDIOC_G_FMT,&fmt) == -1){
        printf("Unable to get format\n");
        return FALSE;
    }
    return TRUE;
}

int V4L2cap::v4l2_grab(){
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1)
    {
        printf("Requesting Buffer error\n");
        return FALSE;
    }
    //5 mmap for buffers
    buffer = (uchar*)malloc(req.count * sizeof(*buffer));
    if(!buffer){
        printf("Out of memory\n");
        return FALSE;
    }
    unsigned int n_buffers;
    for(n_buffers = 0;n_buffers < req.count; n_buffers++){
    //struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = n_buffers;
    if(ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1){
        printf("Querying Buffer error\n");
        return FALSE;
        }

    buffer = (uchar*)mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

    if(buffer == MAP_FAILED){
        printf("buffer map error\n");
        return FALSE;
        }
    printf("Length: %d\nAddress: %p\n", buf.length, buffer);
    printf("Image Length: %d\n", buf.bytesused);
    }
    //6 queue
    for(n_buffers = 0;n_buffers <req.count;n_buffers++){
        buf.index = n_buffers;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        if(ioctl(fd,VIDIOC_QBUF,&buf)){
            printf("query buffer error\n");
            return FALSE;
        }
    }
    //7 starting
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd,VIDIOC_STREAMON,&type) == -1){
        printf("stream on error\n");
        return FALSE;
    }
    return TRUE;
}
