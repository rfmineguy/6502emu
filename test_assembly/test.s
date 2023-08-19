.segment "ZEROPAGE"

.segment "CODE"
vec_reset:
  lda #$45
  sta $00

.segment "VECTORS"
.word $0000         ;nmi?
.word vec_reset     ;reset vector
.word $0000         ;irq?
