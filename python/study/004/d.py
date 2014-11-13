#!/bin/env python
# -*- coding: utf-8 -*-

import random

result = random.randint(0, 20)
count = 0
temp = input("write a number:")
guess = int(temp)

while count < 5:
    count += 1
    if guess == result:
        print("猜对了")
        break
    elif guess < result:
        print("猜小了")
    else:
        print("猜大了")

    if count < 5:
        temp = input("write a number:")
        guess = int(temp)

if count >= 5:
    print("已经猜了5次，GameOver")
else:
    print("End...")
