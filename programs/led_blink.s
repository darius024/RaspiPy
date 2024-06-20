set_pin_output:
ldr x1, gpioint
ldr x2, gpio
str x1, [x2]

load_pin_addresses:
ldr x1, gpsetint
ldr x2, gpset
ldr x3, gpclr
movz x0, #0

loop_blink:
set_pin_high:
ldr x4, wait_time
str x0, [x3]
str x1, [x2]

wait_high:
subs x4, x4, #1
b.ne wait_high

set_pin_low:
ldr x4, wait_time
str x0, [x2]
str x1, [x3]

wait_low:
subs x4, x4, #1
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
.int 0
gpsetint:
.int 0x4
.int 0
wait_time:
.int 0x000fffff
.int 0
