.segment "ZEROPAGE"

.segment "CODE"
vec_reset:
  lda #45
  sta $00
  ora $3245, X
  inx
loop:
  jmp loop

.segment "VECTORS"
.word $0000         ;nmi?
.word vec_reset     ;reset vector
.word $0000         ;irq?
