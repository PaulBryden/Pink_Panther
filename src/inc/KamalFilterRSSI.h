/*
 * File:   KamalFilterRSSI.h
 * Author: longdh
 *
 * Created on July 12, 2016, 1:04 PM
 */

#ifndef KAMALFILTERRSSI_H
#define KAMALFILTERRSSI_H

class KamalFilterRSSI {
public:
    KamalFilterRSSI(double q, double r, double p);
    virtual ~KamalFilterRSSI();

    void init(double x){ _x = x ;}
    void setProcessNoiseCovariance(double i){ _q = i; _q_init = i;}
    void setMeasurementNoiseCovariance(double i){_r = i; _r_init = i ;}
    void setEstimatiomErrorCovariance(double i){_p = i; _p_init = i ;}
    virtual double kalmanUpdate(double measurement);
    void reset(){_q = _q_init; _r = _r_init ; _p = _p_init;};

    double getProcessNoiseCovariance(){ return _q;}
    double getMeasurementNoiseCovariance(){return _r;}
    double getEstimatiomErrorCovariance(){return _p;}
    double getKalmanGain() { return _k;}

private:
    double _q; //process noise covariance
    double _q_init;
    double _r; //measurement noise covariance
    double _r_init;
    double _x; //value
    double _p; //estimation error covariance
    double _p_init;
    double _k; //kalman gain
};

#endif /* KAMALFILTERRSSI_H */