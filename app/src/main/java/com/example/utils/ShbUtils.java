package com.example.utils;

import android.util.Log;

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.IOException;

/**
 * Create by Yanhaoliang
 * e-mail:Yanhaoliang184@163.com
 * on 2021/10/29 16:16
 */
public class ShbUtils {

    static {
        System.loadLibrary("shb-lib");
    }


    public native FileDescriptor open(String path, int bat, int flag);

    public native int openKvIo(int port, int flag);

    public native int openNormalIo(int port, int flag);

    public native void close();

}
