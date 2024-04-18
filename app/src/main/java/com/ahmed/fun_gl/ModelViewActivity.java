package com.ahmed.fun_gl;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.Window;

import com.ahmed.fun_gl.databinding.ActivityMainBinding;

public class ModelViewActivity extends AppCompatActivity {

    MeshView mMeshView;

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

//        binding = ActivityMainBinding.inflate(getLayoutInflater());
//        setContentView(binding.getRoot());
//
//        // Example of a call to a native method
//        TextView tv = binding.sampleText;
//        tv.setText(stringFromJNI());

        //this.requestWindowFeature(Window.FEATURE_NO_TITLE);

        mMeshView = new MeshView(getApplication());
        setContentView(mMeshView);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mMeshView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mMeshView.onResume();
    }

}