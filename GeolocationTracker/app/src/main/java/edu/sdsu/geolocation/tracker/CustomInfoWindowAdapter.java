package edu.sdsu.geolocation.tracker;

import android.content.Context;
import android.text.Html;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.model.Marker;

import org.json.JSONException;
import org.json.JSONObject;

public class CustomInfoWindowAdapter implements GoogleMap.InfoWindowAdapter{

    private final View mWindow;
    private Context mContext;

    public CustomInfoWindowAdapter(Context context){
        mContext = context;
        mWindow = LayoutInflater.from(context).inflate(R.layout.custom_info_window, null);
    }

    private void renderWindowText(Marker marker, View view) throws JSONException {

        if(marker.getSnippet() == null || marker.getTitle() == null)
            return;

        //title
        String title = marker.getTitle();
        TextView tvTitle = (TextView) view.findViewById(R.id.title);

        if(marker.getTitle()!= null){
            tvTitle.setText(title);
        }

        //handles snippet
        if(marker.getSnippet() != null) {
            JSONObject windowInfo = new JSONObject(marker.getSnippet());

            //timestamp
            String timestamp = windowInfo.getString("timestamp");
            TextView tvIdentification = (TextView) view.findViewById(R.id.timestamp);
            tvIdentification.setText(Html.fromHtml("<b>Timestamp:</b> " + timestamp));

            //location
            String lat= windowInfo.getString("lat");
            String lng = windowInfo.getString("lng");
            TextView tvLocation = (TextView) view.findViewById((R.id.location));
            tvLocation.setText(Html.fromHtml("<b>Location:</b> "+ "("+ lat + ", " + lng + ")" ));

            //status
            String status = windowInfo.getString("status");
            TextView tvStatus = (TextView) view.findViewById(R.id.status);


            RelativeLayout titleBackground = (RelativeLayout) view.findViewById(R.id.titleBackground);
            //green = good, red = outside
            if(status.equals("good")){
                tvStatus.setText(Html.fromHtml("<b>Status:</b> " + "<font color=\"#00e673\">"+"Good"));
                titleBackground.setBackgroundColor(mContext.getResources().getColor(R.color.window_green));
            }
            else{
                tvStatus.setText(Html.fromHtml("<b>Status:</b> " + "<font color=\"#FF0000\">"+"Outside"));
                titleBackground.setBackgroundColor(mContext.getResources().getColor(R.color.window_red));
            }


        }

    }

    @Override
    public View getInfoWindow(Marker marker) {
        try {
            renderWindowText(marker, mWindow);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return mWindow;
    }

    @Override
    public View getInfoContents(Marker marker) {
        try {
            renderWindowText(marker, mWindow);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return mWindow;
    }
}
