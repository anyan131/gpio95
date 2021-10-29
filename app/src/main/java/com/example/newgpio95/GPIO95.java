package com.example.newgpio95;

import android.util.Log;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Create by Yanhaoliang
 * e-mail:Yanhaoliang184@163.com
 * on 2021/10/29 16:16
 */
public class GPIO95 {

    static {
        System.loadLibrary("gpio95-lib");
    }


    public FileInputStream fileInputStream;
    public FileDescriptor fileDescriptor;



    public FileInputStream getFileInputStream() {

        return fileInputStream;
    }


    public FileDescriptor init(String path, int bat, int flag) throws Exception {

        fileDescriptor = open(path, bat, flag);

        if (fileDescriptor == null) {
            throw new Exception("GPIO is not turned on Exception");
        }

        fileInputStream = new FileInputStream(fileDescriptor);


        return fileDescriptor;
    }


    public void readGpioMessage(OnGPIOCallBack onGPIOCallBack) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    byte[] bys = new byte[5120];
                    int len = 0;
                    try {
                        len = getFileInputStream().read(bys);

                        String s = new String(bys, 0, len);
                        String[] split = s.split("\n");
                        for (int i = 0; i < split.length; i++) {
                            if (split[i].contains("$GNGGA")) {
                                Log.e("GPIO95", "run: $GNGGA = " + split[i]);
                            }
                        }
                        Log.e("GPIO95", "接收: " + s);
                        onGPIOCallBack.onMessage(s);

                        Thread.sleep(1000);
                    } catch (IOException e) {
                        e.printStackTrace();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();

    }

    public native FileDescriptor open(String path, int bat, int flag);

    public native void close();


}
