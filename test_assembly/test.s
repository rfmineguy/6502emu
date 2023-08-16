.segment "ZEROPAGE"
.byte 3

.segment "CODE"
vec_reset:
  lda $4312, X

.segment "VECTORS"
.word $0000         ;nmi?
.word vec_reset     ;reset vector
.word $0000         ;irq?
