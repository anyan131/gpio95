package com.example.newgpio95;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.example.newgpio95.databinding.ActivityMainBinding;

import java.io.ByteArrayInputStream;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {


    private ActivityMainBinding binding;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

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