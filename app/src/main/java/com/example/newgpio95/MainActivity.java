package com.example.newgpio95;

import android.os.Bundle;
import android.util.Log;

public class MainActivity  {


    protected void onCreate(Bundle savedInstanceState) {

        GPIO95 gpio95 = new GPIO95();
        try {
            gpio95.init("/dev/ttyMT3", 115200, 0);

            gpio95.readGpioMessage(msg -> {
                Log.e("TAG", "onMessage: ------------------ " + msg);
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

    }


}