package com.ahmed.fun_gl;

import android.content.Intent;
import android.net.Uri;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import androidx.recyclerview.widget.RecyclerView;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;

public class ModelListAdapter extends RecyclerView.Adapter<ModelListAdapter.ViewHolder> {

    private ArrayList<String> localDataSet;

    private HashMap<String, Uri> uri_list= new HashMap<>();

    /**
     * Provide a reference to the type of views that you are using
     * (custom ViewHolder)
     */
    public static class ViewHolder extends RecyclerView.ViewHolder {
        private final TextView modelName;
        private final ImageButton modelDelete;

        public ViewHolder(View view) {
            super(view);
            // Define click listener for the ViewHolder's View

            modelName = (TextView) view.findViewById(R.id.modelItemName);
            modelDelete = (ImageButton) view.findViewById(R.id.modelItemDelete);
        }

        public TextView getModelName() {
            return modelName;
        }

        public ImageButton getDeleteButton() {
            return modelDelete;
        }


    }

    /**
     * Initialize the dataset of the Adapter
     *
     * @param dataSet String[] containing the data to populate views to be used
     *                by RecyclerView
     */
    public ModelListAdapter(ArrayList<String> dataSet) {
        localDataSet = dataSet;
    }

    public void remove(String itemName)
    {
        int pos = localDataSet.indexOf(itemName);
        localDataSet.remove(itemName);
        notifyItemRemoved(pos);
    }

    // Create new views (invoked by the layout manager)
    @Override
    public ViewHolder onCreateViewHolder(ViewGroup viewGroup, int viewType) {
        // Create a new view, which defines the UI of the list item
        View view = LayoutInflater.from(viewGroup.getContext())
                .inflate(R.layout.model_item_view, viewGroup, false);

        return new ViewHolder(view);
    }

    // Replace the contents of a view (invoked by the layout manager)
    @Override
    public void onBindViewHolder(ViewHolder viewHolder, final int position) {


        // Get element from your dataset at this position and replace the
        // contents of the view with that element
        viewHolder.getModelName().setText(localDataSet.get(position));

        viewHolder.getDeleteButton().setOnClickListener(v -> remove(viewHolder.getModelName().getText().toString()));

        viewHolder.itemView.setOnClickListener(v -> {
            String name = viewHolder.getModelName().getText().toString();
            ModelSelectionActivity activity= new ModelSelectionActivity();
            activity.displayObj(uri_list.get(name));
            viewHolder.itemView.getContext().startActivity(new Intent(viewHolder.itemView.getContext(), ModelViewActivity.class));
        });
    }

    // Return the size of your dataset (invoked by the layout manager)
    @Override
    public int getItemCount() {
        return localDataSet.size();
    }

    public boolean hasItemName(String itemName)
    {
        return localDataSet.contains(itemName);
    }

    public void add_item_uri(String item, Uri uri)
    {
        uri_list.put(item, uri);
    }
}
