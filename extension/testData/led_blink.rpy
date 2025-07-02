def LOOP():
    GPIO_2 = 1
    wait = WAIT(1)
    GPIO_2 = 0
    wait = WAIT(1)
    return LOOP()
loop = LOOP()
