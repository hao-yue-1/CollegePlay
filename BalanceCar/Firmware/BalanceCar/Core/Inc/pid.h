//
// Created by yue on 2022/1/25.
//

#ifndef BALANCECAR_PID_H
#define BALANCECAR_PID_H

int Incremental_PI_L(int Encoder,int Target);
int Incremental_PI_R(int Encoder,int Target);

int Position_PD_Balance(float pitch,short gyro,float mid);
int Incremental_PI_Speed(int Encoder_L,int Encoder_R);

#endif //BALANCECAR_PID_H
