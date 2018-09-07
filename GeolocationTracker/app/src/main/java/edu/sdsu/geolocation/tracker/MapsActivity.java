package edu.sdsu.geolocation.tracker;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptor;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.GroundOverlayOptions;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.LatLngBounds;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.concurrent.locks.ReentrantLock;

public class MapsActivity extends FragmentActivity implements OnMapReadyCallback,GoogleMap.OnMarkerClickListener, GoogleMap.OnMapClickListener {

    //vars
    private GoogleMap mMap;
    private LatLng defaultLatLng;
    private Marker currMarker;
    private int numUsers;

    //constants
    private static final float DEFAULT_ZOOM = 19f;
    private static final double DEFAULT_LAT = 32.7776245;
    private static final double DEFAULT_LNG = -117.07048;

    private ArrayList<Person> mPeople;
    private ArrayList<Marker> mMarkers;

    //creates thread that is later used for consuming web services
    Thread callback = new Thread(new MapsCallback(this));

    //============================
    //MAP SET-UP FUNCTIONS
    //============================
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_maps);

        //var initializing
        defaultLatLng = new LatLng(DEFAULT_LAT, DEFAULT_LNG);
        numUsers = 0;
        mPeople = new ArrayList<>(0);
        mMarkers = new ArrayList<>(0);

        //initializes map, calls onMapReady
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);
    }

    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;

        //customizes pin window based on adapter & sets up onclick listeners
        mMap.setInfoWindowAdapter(new CustomInfoWindowAdapter(MapsActivity.this));
        mMap.setOnMarkerClickListener(MapsActivity.this);
        mMap.setOnMapClickListener(MapsActivity.this);

        //adds picture of third floor onto map
        createOverlay(R.raw.eng_fourth_floor, 32.777431, -117.070890,
                32.777818, -117.070070);
        moveCamera(defaultLatLng, DEFAULT_ZOOM);

        //starts connecting to database
       callback.start();

    }

    //===============================
    //Updates Markers and People
    //===============================

    private void updateMarkers(){

        for(int i = 0; i<mMarkers.size(); i++){
            mMarkers.get(i).setPosition(mPeople.get(i).getLatLng());
            mMarkers.get(i).setTitle((mPeople.get(i).getName()));

            //formats lat & lng of person
            DecimalFormat numberFormat = new DecimalFormat("#.00000");
            double lat = mPeople.get(i).getLatLng().latitude;
            double lng = mPeople.get(i).getLatLng().longitude;
            lat = Double.parseDouble(numberFormat.format(lat));
            lng = Double.parseDouble(numberFormat.format(lng));

            //snippet is sent as JSON data and is later converted to HTML text for formatting
            JSONObject infoWindowData = new JSONObject();
            try{
                infoWindowData.put("timestamp",mPeople.get(i).getTimestamp());
                infoWindowData.put("lat", lat);
                infoWindowData.put("lng", lng);
                infoWindowData.put("status", mPeople.get(i).getStatus());
            } catch (JSONException e) {
                e.printStackTrace();
            }
            mMarkers.get(i).setSnippet(infoWindowData.toString());
        }

        //checks if people are outside & then updates markers accordingly
        checkIfPeopleOutside();
        updateMarkerIcons();
    }

    //based on their location, sets status to "good" or "outside"
    //thread is used to speed up computation
    private void checkIfPeopleOutside(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                for(int i = 0; i<mPeople.size(); i++){
                    double lat = mPeople.get(i).getLatLng().latitude;
                    double lng = mPeople.get(i).getLatLng().longitude;

                    if(lat > 32.777818 || lat < 32.777431 || lng >-117.070070 || lng < -117.070890 ){
                        mPeople.get(i).setStatus("outside");
                    } else {
                        mPeople.get(i).setStatus("good");
                    }
                }
            }
        }).start();

    }

    //updates icons to be red/green/blue
    private void updateMarkerIcons(){

        for(int i = 0; i<mMarkers.size(); i++) {
            if(mPeople.get(i).getName().equals("User0")){
                if (currMarker != null && mMarkers.get(i).equals(currMarker))
                    mMarkers.get(i).setIcon(BitmapDescriptorFactory.fromResource(R.mipmap.ic_person_light_blue));
                else
                    mMarkers.get(i).setIcon(BitmapDescriptorFactory.fromResource(R.mipmap.ic_person_blue));
            }
            else if (mPeople.get(i).getStatus().equals("outside")) {
                if (currMarker != null && mMarkers.get(i).equals(currMarker))
                    mMarkers.get(i).setIcon(BitmapDescriptorFactory.fromResource(R.mipmap.ic_person_light_red));
                else
                    mMarkers.get(i).setIcon(BitmapDescriptorFactory.fromResource(R.mipmap.ic_person_red));
            } else {
                if (currMarker != null && mMarkers.get(i).equals(currMarker))
                    mMarkers.get(i).setIcon(BitmapDescriptorFactory.fromResource(R.mipmap.ic_person_light_green));
                else
                    mMarkers.get(i).setIcon(BitmapDescriptorFactory.fromResource(R.mipmap.ic_person_green));
            }
        }

    }

    //===============================
    //Connects to RESTful webservice
    //===============================

    //obtains sensor data from database
    public void consumeWebService() {
        java.util.concurrent.locks.Lock l = new ReentrantLock();
        l.lock();
        try {
            RequestQueue queue = Volley.newRequestQueue(this);
            String url ="http://marconi.sdsu.edu:8080/GeoLocation/resources/app";

            // Request a string response from the provided URL.
            StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                    new Response.Listener<String>() {
                        @Override
                        public void onResponse(String response) {
                            processJSON(response);
                        }
                    }, new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    System.err.println(error.getMessage());
                }
            });

            // Add the request to the RequestQueue.
            queue.add(stringRequest);
            // access the resource protected by this lock
        } finally {
            l.unlock();
        }
    }

    //processes JSON string
    private void processJSON(String response) {
        java.util.concurrent.locks.Lock l = new ReentrantLock();
        l.lock();
        try {

            try {

                JSONArray mArray = new JSONArray(response);
                numUsers = mArray.length();

                //creates a person for every user detected
                addPeopleToApp(numUsers - mPeople.size());

                for(int i = 0; i< numUsers; i++) {

                    JSONObject mainObject = mArray.getJSONObject(i);
                    String timestamp = mainObject.getString("timestamp");
                    String alt = mainObject.getString("alt");
                    String deviceName = mainObject.getString("device");
                    Double latitude = Double.parseDouble(mainObject.getString("lat"));
                    Double longitude = Double.parseDouble(mainObject.getString("long"));

                    modifyLocation(i, timestamp, alt, latitude, longitude, deviceName);
                }
                updateMarkers();

                //keeps window updated each time marker changes location
                if(currMarker!=null)
                    currMarker.showInfoWindow();

            } catch (JSONException ex) {
                System.err.println(ex.getMessage());
            }

        } finally {
            l.unlock();
        }

    }

    //changes person location in app
    private void modifyLocation(int index, String timestamp, String alt, Double latitude, Double longitude, String deviceName) {
        java.util.concurrent.locks.Lock l = new ReentrantLock();
        l.lock();
        try {

            mPeople.get(index).setLatLng(new LatLng(latitude, longitude));
            mPeople.get(index).setName(deviceName);

            //removes extra double quotes from timestamp string
            String time = timestamp.replaceAll("^\"|\"$", "");

            mPeople.get(index).setTimestamp(time);

        } finally {
            l.unlock();
        }
    }

    //===============================
    //On-Click Event Methods
    //===============================

    //sets currMarker to marker that was clicked
    @Override
    public boolean onMarkerClick(Marker marker) {
        for(int i=0; i<mMarkers.size(); i++){
            if(marker.equals(mMarkers.get(i)))
                currMarker = marker;
        }
        updateMarkerIcons();

        //returning false makes it so default onMarkerClick method is also called
        return false;
    }

    @Override
    public void onMapClick(LatLng point){
        //resets clicked marker color
        currMarker = null;
        updateMarkerIcons();
    }

    //===============================
    //Helper Methods
    //===============================

    //moves camera
    private void moveCamera(LatLng latlng, float zoom){
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(latlng, zoom));
    }

    //places image over map
    private void createOverlay(int resource, double south, double west
            , double north , double east) {

        BitmapDescriptor bitmapDescriptor = BitmapDescriptorFactory.fromResource(resource);

        LatLng southWest = new LatLng(south, west);
        LatLng northEast = new LatLng (north, east);

        LatLngBounds LatLngBounds = new LatLngBounds(southWest, northEast);


        GroundOverlayOptions groundOverlayOptions = new GroundOverlayOptions();
        groundOverlayOptions.positionFromBounds(LatLngBounds);
        groundOverlayOptions.image(bitmapDescriptor);
        groundOverlayOptions.transparency(0.0f);

        mMap.addGroundOverlay(groundOverlayOptions);

    }

    private void addPeopleToApp(int num){

        if(num == 0)
            return;

        //initializes people and markers to default values
        for(int i = 0; i<num; i++){
            mPeople.add(new Person("DEFAULT_NAME", Integer.toString(i+1),defaultLatLng, "good"));
            mMarkers.add(mMap.addMarker(new MarkerOptions()
                    .position(defaultLatLng)
                    .title(mPeople.get(i).getName())
                    .icon(BitmapDescriptorFactory.fromResource(R.mipmap.ic_person_red))));
        }
    }
}

