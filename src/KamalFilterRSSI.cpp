/*
 * File:   KamalFilterRSSI.cpp
 * Author: longdh
 *
 * Created on July 12, 2016, 1:04 PM
 */

#include "inc/KamalFilterRSSI.h"

KamalFilterRSSI::KamalFilterRSSI(double q, double r, double p)
        : _q(q), _q_init(q), _r(r), _r_init(0), _x(0), _p(p), _p_init(p), _k(_p / (_p + _r))
{

}

KamalFilterRSSI::~KamalFilterRSSI() {
}

/**
 * update data
 *
 * @param measurement
 * @return
 */
double KamalFilterRSSI::kalmanUpdate(double measurement) {
    //prediction update
    //omit _x = _x
    _p = _p + _q;

    //measurement update
    _k = _p / (_p + _r);
    _x = _x + _k * (measurement - _x);
    _p = (1 - _k) * _p;

    return _x;
}