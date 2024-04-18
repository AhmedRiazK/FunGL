package com.ahmed.fun_gl;

import android.content.ContentUris;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.util.Log;

public class ContentUriUtils {
    String getFilePath(Context context, Uri fileUri) {
        Uri uri = fileUri;
        String selection = null;
        String[] selectionArgs = new String[1];

        //For older versions (Before KitKat)
        if (Build.VERSION.SDK_INT >= 19 && DocumentsContract.isDocumentUri(context, uri)) {
            if (isExternalStorageDocument(uri)) {
                String docId = DocumentsContract.getDocumentId(uri);
                String[] split = docId.split(":");
                return Environment.getExternalStorageDirectory().toString() + "/" + split[1];
            } else if (isDownloadsDocument(uri)) {
                String id = DocumentsContract.getDocumentId(uri);
                uri = ContentUris.withAppendedId(
                        Uri.parse("content://downloads/public_downloads"), Integer.parseInt(id.split(":")[1]));
                Log.d("URI", uri.getPath());
            } else if (isMediaDocument(uri)) {
                String docId = DocumentsContract.getDocumentId(uri);
                String[] split = docId.split(":");
                String type = split[0];
                if (type.equals("image")) {
                    uri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                } else if (type.equals("video")) {
                    uri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                } else if (type.equals("audio")) {
                    uri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                }
                selection = "_id=?";
                selectionArgs[0] = split[1];
            }
        }
        if ("content".equals(uri.getScheme())) {
            if (isGooglePhotosUri(uri)) {
                return uri.getLastPathSegment();
            }
            String[] projection = {
                    MediaStore.Images.Media.DATA
            };
            try {
                Cursor cursor = context.getContentResolver().query(uri, projection, selection, selectionArgs, null);
                String path = null;
                if (cursor != null) {
                    int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
                    if (cursor.moveToFirst()) {
                        path = cursor.getString(column_index);
                        cursor.close();
                    }
                }
                return path;
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else if ("file".equals(uri.getScheme())) {
            return uri.getPath();
        }
        return null;
    }

    boolean isExternalStorageDocument(Uri uri) {
        return uri.getAuthority().equals("com.android.externalstorage.documents");
    }

    boolean isDownloadsDocument(Uri uri) {
        return uri.getAuthority().equals("com.android.providers.downloads.documents");
    }

    boolean isMediaDocument(Uri uri) {
        return uri.getAuthority().equals("com.android.providers.media.documents");
    }

    boolean isGooglePhotosUri(Uri uri) {
        return uri.getAuthority().equals("com.google.android.apps.photos.content");
    }

}
