#!/usr/bin/env python
# coding: UTF-8

from __future__ import print_function

import serial
import rospy
import roslib
import numpy as np
import datetime
import csv
import time

import struct


from std_msgs.msg import Int32MultiArray,Float32MultiArray

if __name__ == '__main__':
    try:
        rospy.init_node('encoder', anonymous=True)
        r = rospy.Rate(30)  
        encoder_data = rospy.Publisher(
            "encoder_data", Float32MultiArray, queue_size=10)
        encoder_speed = rospy.Publisher(
            "encoder_speed", Float32MultiArray, queue_size=10)

        # timeoutを秒で設定（default:None)ボーレートはデフォルトで9600
        ser = serial.Serial("/dev/ttyUSB1", 115200, timeout=3)

        encoder_rec_data = [0, 0]
        encoder_rec_data_before = [0, 0]
        encoder_cal_speed = [0, 0]
        rec_count = 0
        rec_data = [0]*11
        rec_flag = 0


        while not rospy.is_shutdown():
            line = ser.read()  # 行終端'¥n'までリードする
            
            # tmp = int.from_bytes(line, byteorder='big')
            # tmp = struct.unpack('>HH', line)
            tmp = np.frombuffer(line, dtype=np.uint8)
            tmp = int(tmp)
            if tmp == 255:
                rec_flag = 1
            elif rec_flag == 1:
                rec_data[rec_count] = tmp
                rec_count += 1
                if rec_count == 11:
                    # print(rec_data)
                    rec_flag = 0
                    rec_count = 0


                    #degreeで計算
                    encoder_rec_data[0] = (rec_data[0] << 7)+rec_data[1] + \
                        (rec_data[2] << 28)+(rec_data[3] << 21)+(rec_data[4] << 14)
                    encoder_rec_data[1] = (rec_data[5] << 7)+rec_data[6] + \
                        (rec_data[7] << 28)+(rec_data[8] << 21)+(rec_data[9] << 14)
                    encoder_rec_data[0] = (360.0*(encoder_rec_data[0])/(2**14))
                    encoder_rec_data[1] = (360.0*(encoder_rec_data[1])/(2**14))
                    # print("")
                    # print(rec_data)

                    #speed degreeで計算
                    encoder_cal_speed[0] = int(
                        (encoder_rec_data[0]-encoder_rec_data_before[0])/(rec_data[10]/1000.0))
                    encoder_cal_speed[1] = int(
                        (encoder_rec_data[1]-encoder_rec_data_before[1])/(rec_data[10]/1000.0))

                    print(
                        int(encoder_rec_data[0]), "\t",
                        encoder_cal_speed[0], "\t",
                        int(encoder_rec_data[1]), "\t",
                        encoder_cal_speed[1]
                    )

                    encoder_rec_data_before[0] = encoder_rec_data[0]
                    encoder_rec_data_before[1] = encoder_rec_data[1]


                    data1 = Float32MultiArray()
                    data1.data = [encoder_rec_data[0], encoder_rec_data[1],rec_data[10]]
                    encoder_data.publish(data1)

                    data2 = Float32MultiArray()
                    data2.data = [encoder_cal_speed[0], encoder_cal_speed[1]]
                    encoder_speed.publish(data2)


    except rospy.ROSInterruptException:
        ser.close()
        pass
