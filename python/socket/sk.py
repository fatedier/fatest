#!/bin/env python
#-*- coding: utf-8 -*-

import socket
import os
import time
import threading

def deal_cli():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    sv_ip = '127.0.0.1'
    port = 9999

    s.connect((sv_ip, port))

    msg = '0123456789'
    msg = msg * 100
    
    while True:
        s.sendall(msg)
        reply = s.recv(1024)
        time.sleep(1)

n = 0
while n < 999:
    t = threading.Thread(target=deal_cli)
    t.start()
    n += 1

deal_cli()
