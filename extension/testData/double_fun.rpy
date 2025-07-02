def double(a):
    a = a + a
    return a

def double_n_times(a, b):
    for i in range(1, b):
        a = double(a)
    return a

x = double_n_times(5, 10)
