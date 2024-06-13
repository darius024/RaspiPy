set_pin_output:
ldr w1, gpioint
ldr w2, gpio
str w1, [w2]

load_pin_addresses:
ldr w1, gpsetint
ldr w2, gpset
ldr w3, gpclr
movz w0, #0

loop_blink:
set_pin_high:
ldr w4, wait_time
str w0, [w3]
str w1, [w2]

wait_high:
subs w4, w4, #1
b.ne wait_high

set_pin_low:
ldr w4, wait_time
str w0, [w2]
str w1, [w3]

wait_low:
subs w4, w4, #1
b.ne wait_low

b loop_blink
and x0, x0, x0

gpio:
.int 0x3f200000
gpset:
.int 0x3f20001c
gpclr:
.int 0x3f200028
gpioint:
.int 0x40
gpsetint:
.int 0x4
wait_time:
.int 0x000fffff
