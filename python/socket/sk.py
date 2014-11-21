#!/bin/env python
#-*- coding: utf-8 -*-

import socket
import os
import time
import threading
import sys

def deal_cli():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    sv_ip = '127.0.0.1'
    #sv_ip = '10.32.146.9'
    port = 9999

    s.connect((sv_ip, port))

    msg = '0123456789'
    msg = msg * 100
    
    while True:
        try:
            s.sendall(msg)
            reply = s.recv(1024)
        except:
            break
        time.sleep(0.1)
try:
    n = 0
    while n < 999:
        t = threading.Thread(target=deal_cli)
        t.start()
        n += 1

    deal_cli()
except:
    print('over')
    sys.exit(-1)
