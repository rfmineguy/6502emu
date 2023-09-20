.segment "ZEROPAGE"

.segment "CODE"
vec_reset:
  lda #45
  sta $00
  ora $3245, X
  inx
booger:
  inx
  lda #97
  sta $3243
loop:
  lda #43
  lda #43
  lda #43
  lda #43
  lda #43
  jmp loop
  bcc loop

.segment "VECTORS"
.word $0000         ;nmi?
.word vec_reset     ;reset vector
.word $0000         ;irq?
