#!/bin/env python
# -*- coding: utf-8 -*-

import random

result = random.randint(0, 20)
count = 0
temp = input("write a number:")
guess = int(temp)
count += 1

while (guess != result) and (count < 5):
    if guess == result:
        print("猜对了")
    elif guess < result:
        print("猜小了")
    else:
        print("猜大了")
    count += 1
    temp = input("write a number:")
    guess = int(temp)

if count >= 5:
    print("已经猜了超过5次，GameOver")
else:
    print("End...")
