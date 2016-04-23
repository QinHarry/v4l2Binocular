#ifndef V4L2CAP_H
#define V4L2CAP_H

#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <iostream>


#define IMAGEWIDTH 320
#define IMAGEHEIGHT 240

#define TRUE 1
#define FALSE 0

class V4L2cap
{
public:
   V4L2cap(const char* file_video);
   ~V4L2cap();

public:
   uchar *buffer;
   int fd;
   const char *file_video;
   struct v4l2_streamparm setfps;
   struct v4l2_capability cap;
   struct v4l2_fmtdesc fmtdesc;
   struct v4l2_format fmt,fmtack;
   struct v4l2_requestbuffers req;
   struct v4l2_buffer buf;
   enum   v4l2_buf_type type;

public:
   int init_v4l2(void);
   int v4l2_grab(void);
};

#endif
