set_pin_output:
ldr w1, gpioint
ldr x2, gpio
str w1, [x2]

load_pin_addresses:
ldr w1, gpsetint
ldr x2, gpset
ldr x3, gpclr

loop_blink:
set_pin_high:
ldr w4, wait_time
str w1, [x2]

wait_high:
subs w4, w4, #1
b.ne wait_high

set_pin_low:
ldr w4, wait_time
str w1, [x3]

wait_low:
subs w4, w4, #1
b.ne wait_low

b loop_blink
and x0, x0, x0

gpio:
.int 0x3f200000
.int 0
gpset:
.int 0x3f20001c
.int 0
gpclr:
.int 0x3f200028
.int 0
gpioint:
.int 0x40
gpsetint:
.int 0x4
wait_time:
.int 0x000fffff
