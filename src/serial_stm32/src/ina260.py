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


from std_msgs.msg import Float64

if __name__ == '__main__':
    try:
        rospy.init_node('ina260', anonymous=True)
        r = rospy.Rate(10)  
        current = rospy.Publisher(
            "current", Float64, queue_size=10)
        voltage = rospy.Publisher(
            "voltage", Float64, queue_size=10)

        # timeoutを秒で設定（default:None)ボーレートはデフォルトで9600
        ser = serial.Serial("/dev/ttyUSB0", 115200, timeout=3)

        rec_count = 0
        rec_data = [0]*8
        rec_flag = 0


        while not rospy.is_shutdown():
            line = ser.read()  # 行終端'¥n'までリードする
            
            # tmp = int.from_bytes(line, byteorder='big')
            # tmp = struct.unpack('>HH', line)
            tmp = np.frombuffer(line, dtype=np.uint8)
            tmp = int(tmp)
            #print(tmp)
            if tmp == 118:
                rec_flag = 'v'
            elif rec_flag == 'v':
                rec_data[rec_count] = tmp
                print(tmp)
                #print(rec_count)
                rec_count += 1
                if rec_count == 8:
                    rec_flag = 0
                    rec_count = 0

                    current = float(rec_data[0] | rec_data[1] << 8 | rec_data[2] << 16 | rec_data[3] << 24 | rec_data[4] << 32 | rec_data[5] << 40 | rec_data[6] << 48 | rec_data[7] << 55)

                    print(current)

                    data1 = Float64()
                    #current.publish(data1)

                    data2 = Float64()
                    voltage.publish(data2)


    except rospy.ROSInterruptException:
        ser.close()
        pass
