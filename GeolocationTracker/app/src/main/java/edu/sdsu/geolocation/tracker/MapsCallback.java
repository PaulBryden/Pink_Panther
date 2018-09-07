package edu.sdsu.geolocation.tracker;

public class MapsCallback implements Runnable{

    private MapsActivity mActivity;

    public MapsCallback(MapsActivity activity){
        mActivity = activity;
    }

    @Override public void run(){
        while(true){
            mActivity.consumeWebService();

            try{
                Thread.sleep(1000);

            }catch(Exception e){
                e.printStackTrace();
            }
        }
    }




}
