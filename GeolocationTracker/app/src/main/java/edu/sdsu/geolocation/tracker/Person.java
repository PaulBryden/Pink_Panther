package edu.sdsu.geolocation.tracker;

import com.google.android.gms.maps.model.LatLng;

//Person class will store data of a person
public class Person {

    private String name;
    private String timestamp;
    private LatLng latLng;
    private String status;



    public Person(String name, String timestamp, LatLng latLng, String status) {
        this.name = name;
        this.timestamp = timestamp;
        this.latLng = latLng;
        this.status = status;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(String timestamp) {
        this.timestamp = timestamp;
    }

    public LatLng getLatLng() {
        return latLng;
    }

    public void setLatLng(LatLng latLng) {
        this.latLng = latLng;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }
}
