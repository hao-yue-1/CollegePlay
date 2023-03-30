//
// Created by yue on 2022/1/31.
//

#ifndef BALANCECAR_CONTROL_H
#define BALANCECAR_CONTROL_H

void MotorSpeedCtrl(int speed_l,int speed_r);
int LimitPWM(int pwm);
void BalancePIDCtrl(float mid);

#endif //BALANCECAR_CONTROL_H
