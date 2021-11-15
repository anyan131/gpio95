
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <jni.h>


#include "android/log.h"

static const char *TAG = "serial_port";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

#define RS2251_A 80
#define RS2251_B 79
#define RS2251_C 78
#define  MYfile_NAME  "/dev/myGPIO"
#define  Phone_ONOFFIO  3
#define  Phone_CALL 94
//#define  Phone_Hl 95
#define  RED_ONOFF 5
#define  GPS_ONOFF 95
#define MYfile_UART "/dev/ttyMT2"
#define SERIAL_REC_NUM 4
// VTIME and VMIN is very important.
// VTIME: Time to wait for data (tenths of seconds)等待数据的时间（十分之几秒）
#define SERIAL_VTIME 1
// VMIN: Minimum number of characters to read 要读取的最小字符数
#define SERIAL_VMIN SERIAL_REC_NUM

static speed_t getBaudrate(jint baudrate) {
    switch (baudrate) {
        case 0:
            return B0;
        case 50:
            return B50;
        case 75:
            return B75;
        case 110:
            return B110;
        case 134:
            return B134;
        case 150:
            return B150;
        case 200:
            return B200;
        case 300:
            return B300;
        case 600:
            return B600;
        case 1200:
            return B1200;
        case 1800:
            return B1800;
        case 2400:
            return B2400;
        case 4800:
            return B4800;
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        case 230400:
            return B230400;
        case 460800:
            return B460800;
        case 500000:
            return B500000;
        case 576000:
            return B576000;
        case 921600:
            return B921600;
        case 1000000:
            return B1000000;
        case 1152000:
            return B1152000;
        case 1500000:
            return B1500000;
        case 2000000:
            return B2000000;
        case 2500000:
            return B2500000;
        case 3000000:
            return B3000000;
        case 3500000:
            return B3500000;
        case 4000000:
            return B4000000;
        default:
            return -1;
    }
}


extern "C" JNIEXPORT jobject JNICALL Java_com_example_utils_ShbUtils_open
        (JNIEnv *env, jobject thiz, jstring path, jint baudrate, jint flags) {
    int fd;
    speed_t speed;
    jobject mFileDescriptor;

    /* Check arguments */
    {
        speed = getBaudrate(baudrate);
        if (speed == -1) {
            /* TODO: throw an exception */
            LOGE("Invalid baudrate");
            return NULL;
        }
    }

    /* Opening device */
    {
        jboolean iscopy;
        const char *path_utf = (*env).GetStringUTFChars(path, &iscopy);
        LOGD("Opening serial port %s with flags 0x%x", path_utf, O_RDWR | flags);
        fd = open(path_utf, O_RDWR | flags);
        LOGD("open() fd = %d", fd);
        (*env).ReleaseStringUTFChars(path, path_utf);
        if (fd == -1) {
            /* Throw an exception */
            LOGE("Cannot open port");
            /* TODO: throw an exception */
            return NULL;
        }
    }

    /* Configure device */
    {
        struct termios cfg;
        LOGD("Configuring serial port");
        if (tcgetattr(fd, &cfg)) {
            LOGE("tcgetattr() failed");
            close(fd);
            /* TODO: throw an exception */
            return NULL;
        }

        cfmakeraw(&cfg);
        cfsetispeed(&cfg, speed);
        cfsetospeed(&cfg, speed);

        if (tcsetattr(fd, TCSANOW, &cfg)) {
            LOGE("tcsetattr() failed");
            close(fd);
            /* TODO: throw an exception */
            return NULL;
        }
    }

    /* Create a corresponding file descriptor */
    {
        jclass cFileDescriptor = (*env).FindClass("java/io/FileDescriptor");
        jmethodID iFileDescriptor = (*env).GetMethodID(cFileDescriptor, "<init>", "()V");
        jfieldID descriptorID = (*env).GetFieldID(cFileDescriptor, "descriptor", "I");
        mFileDescriptor = (*env).NewObject(cFileDescriptor, iFileDescriptor);
        (*env).SetIntField(mFileDescriptor, descriptorID, (jint) fd);
    }

    return mFileDescriptor;
}


/*
 * Class:     cedric_serial_SerialPort
 * Method:    close
 * Signature: ()V
 */
extern "C" JNIEXPORT void JNICALL Java_com_example_utils_ShbUtils_close
        (JNIEnv *env, jobject thiz) {
    jclass SerialPortClass = (*env).GetObjectClass(thiz);
    jclass FileDescriptorClass = (*env).FindClass("java/io/FileDescriptor");

    jfieldID mFdID = (*env).GetFieldID(SerialPortClass, "mFd", "Ljava/io/FileDescriptor;");
    jfieldID descriptorID = (*env).GetFieldID(FileDescriptorClass, "descriptor",
                                              reinterpret_cast<const char *>('I'));

    jobject mFd = (*env).GetObjectField(thiz, mFdID);
    jint descriptor = (*env).GetIntField(mFd, descriptorID);

    LOGD("close(fd = %d)", descriptor);
    close(descriptor);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_utils_ShbUtils_openKvIo(JNIEnv *env, jobject thiz, jint port, jint flag) {
    int fd;
    fd = open(MYfile_NAME, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(fd < 0)
        ("Can't open /dev/leds!\n");
    switch(flag)
    {
        //X0 0
        case 0: ioctl(fd,RS2251_A,0);  ioctl(fd,RS2251_B,0);   ioctl(fd,RS2251_C,0);  break;
            //X1 68k  220V的报警距离为：2.8m～3.2m
        case 1: ioctl(fd,RS2251_A,1);  ioctl(fd,RS2251_B,0);   ioctl(fd,RS2251_C,0);  break;
            //X2 336k  10kV的报警距离为：2.2m～2.8m
        case 2: ioctl(fd,RS2251_A,0);  ioctl(fd,RS2251_B,1);   ioctl(fd,RS2251_C,0);  break;
            //X3 470k 35kV的报警距离为：2.5m～3.5m
        case 3: ioctl(fd,RS2251_A,1);  ioctl(fd,RS2251_B,1);   ioctl(fd,RS2251_C,0);  break;
            //X4 680K  110kV的报警距离为：4.0m～6.0m
        case 4: ioctl(fd,RS2251_A,0);  ioctl(fd,RS2251_B,0);   ioctl(fd,RS2251_C,1);  break;
            //X5 820K  220kV的报警距离为：5.0m～9.0m
        case 5: ioctl(fd,RS2251_A,1);  ioctl(fd,RS2251_B,0);   ioctl(fd,RS2251_C,1);  break;
            //X6
        case 6: ioctl(fd,RS2251_A,1);  ioctl(fd,RS2251_B,1);   ioctl(fd,RS2251_C,1);  break;
        default:
            break;
    }

    close(fd);
    return 1;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_utils_ShbUtils_openNormalIo(JNIEnv *env, jobject thiz, jint port, jint flag) {
    int fd;
    fd = open(MYfile_NAME, O_RDWR | O_NOCTTY | O_NONBLOCK);

    ioctl(fd,port,flag);
    LOGI("Phone_ONOFFIO =%d",flag);
    close(fd);

    return 1;
}