  b main

main:
  ldr sp, stack_pointer
  movz w0, #2
  movz w1, #3
  add w2, w0, w1
  mov w0, w2
  and x0, x0, x0


stack_pointer:
.int 0x8739a0
.int 0x0
