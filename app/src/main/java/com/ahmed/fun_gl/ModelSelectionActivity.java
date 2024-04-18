package com.ahmed.fun_gl;

import static android.provider.MediaStore.createWriteRequest;

import android.Manifest;
import android.app.Activity;
import android.app.PendingIntent;
import android.content.ContentResolver;
import android.content.Context;
import android.content.IntentSender;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;

import androidx.activity.result.ActivityResult;
import androidx.activity.result.ActivityResultCallback;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.provider.MediaStore;
import android.provider.OpenableColumns;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.ahmed.fun_gl.databinding.ActivityModelSelectionBinding;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;

public class ModelSelectionActivity extends AppCompatActivity {

    private AppBarConfiguration appBarConfiguration;
    private ActivityModelSelectionBinding binding;

    private RecyclerView recyclerView;

    private static ContentResolver contentResolver;

    private Context ctx;

    ArrayList<String> list_model;

    ModelListAdapter modelListAdapter;

    // Request code for selecting a PDF document.
    private static final int PICK_PDF_FILE = 2;

    public static void copy(File src, File dst) throws IOException {
        try (InputStream in = Files.newInputStream(src.toPath())) {
            try (OutputStream out = Files.newOutputStream(dst.toPath())) {
                // Transfer bytes from in to out
                byte[] buf = new byte[1024];
                int len;
                while ((len = in.read(buf)) > 0) {
                    out.write(buf, 0, len);
                }
            }
        }
    }


    ActivityResultLauncher<Intent> mGetContent = registerForActivityResult(new ActivityResultContracts.StartActivityForResult(),
            new ActivityResultCallback<ActivityResult>() {
                @Override
                public void onActivityResult(ActivityResult result) {
                    if (result.getResultCode() == Activity.RESULT_OK) {

                        Intent data = result.getData();
                        assert data != null;
                        Uri uri = data.getData();
                        assert uri != null;
                        Cursor cursor = getContentResolver().query(uri, null, null, null, null);

                        assert cursor != null;
                        int nameIndex = cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME);

                        cursor.moveToFirst();

                        String fileName = cursor.getString(nameIndex);
                        cursor.close();

                        if(!modelListAdapter.hasItemName(fileName))//TODO check for file format
                        {
                            list_model.add(fileName);
                            modelListAdapter.add_item_uri(fileName, uri);

                            //Environment.getExternalStorageDirectory().toString() + "/Download/" + fileName
                            Log.d("ModelName", new ContentUriUtils().getFilePath(ModelSelectionActivity.this, uri));


                            /*

                            String copyFileName = "/data/data/com.ahmed.fun_gl/copyfile.obj";
                            try {
                                copy(new File(new ContentUriUtils().getFilePath(ModelSelectionActivity.this, uri)), new File(copyFileName));
                            } catch (IOException e) {
                                throw new RuntimeException(e);
                            }
                            Toast.makeText(ModelSelectionActivity.this, new ContentUriUtils().getFilePath(ModelSelectionActivity.this, uri), Toast.LENGTH_LONG).show();
                            try {
                                FileReader fileReader = new FileReader(new ContentUriUtils().getFilePath(ModelSelectionActivity.this, uri));
                                char[] temp = new char[10];
                                fileReader.read(temp, 0, 2);
                            } catch (FileNotFoundException e) {
                                throw new RuntimeException(e);
                            } catch (IOException e) {
                                throw new RuntimeException(e);
                            }
*/

                            modelListAdapter.notifyItemInserted(modelListAdapter.getItemCount());
                        }
                        else
                        {
                            Toast.makeText(ModelSelectionActivity.this, "Duplicate File", Toast.LENGTH_SHORT).show();
                        }
                    }
                }
            });

    private void openFile() {
        //Intent intent = new Intent(Intent.ACTION_PICK,/*Intent.ACTION_OPEN_DOCUMENT*/);
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("application/octet-stream");
        mGetContent.launch(intent);
    }

    public void displayObj(Uri uri)
    {
        StringBuilder stringBuilder = new StringBuilder();
        try (
                InputStream inputStream = contentResolver.openInputStream(uri);
                BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream)))
        {
            String line;
            NativeLib.parseInit();
            while((line = reader.readLine()) != null)
            {
                NativeLib.parseLine(line);
                //stringBuilder.append(line).append("\n");
            }
            NativeLib.prepareVertices();
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityModelSelectionBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        list_model = new ArrayList<String>();

        modelListAdapter = new ModelListAdapter(list_model);

        contentResolver = getContentResolver();

        ctx = ModelSelectionActivity.this;

        //setSupportActionBar(binding.toolbar);
        recyclerView = findViewById(R.id.modelListView);
        // Set the layout manager to arrange the items
        recyclerView.setAdapter(modelListAdapter);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));

//        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_model_selection);
//        appBarConfiguration = new AppBarConfiguration.Builder(navController.getGraph()).build();
//        NavigationUI.setupActionBarWithNavController(this, navController, appBarConfiguration);

        binding.fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAnchorView(R.id.fab)
//                        .setAction("Action", null).show();

                openFile();
            }
        });
    }

//    @Override
//    public boolean onSupportNavigateUp() {
//        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_model_selection);
//        return NavigationUI.navigateUp(navController, appBarConfiguration)
//                || super.onSupportNavigateUp();
//    }
}