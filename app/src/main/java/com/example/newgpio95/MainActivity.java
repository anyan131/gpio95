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

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private ActivityMainBinding binding;

    private FileInputStream mFileInputStream;
    private FileOutputStream mFileOutputStream;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());


        TextView tv = binding.sampleText;

        FileDescriptor open = open("/dev/ttyMT3", 115200, 0);
        if (open == null) {
            tv.setText("error");
        }

        mFileInputStream = new FileInputStream(open);



        new Thread(new InputThread()).start();


    }

    class InputThread implements Runnable {

        @Override
        public void run() {

            while (true) {
                byte[] bys = new byte[5120];
                int len = 0;
                try {
                    len = mFileInputStream.read(bys);
                    String s = new String(bys, 0, len);
                    String[] split = s.split("\n");
                    for (int i = 0; i < split.length; i++) {
                        if (split[i].contains("$GNGGA")){
                            Log.e("MainActivity", "run: $GNGGA = "+split[i] );
                        }
                    }
                    Log.e("MainActivity", "接收: " + s);

                    Thread.sleep(1000);
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native FileDescriptor open(String path, int bat, int flag);

    public native void close();

}