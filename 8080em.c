#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ConditionCodes {
  uint8_t z : 1;
  uint8_t s : 1;
  uint8_t p : 1;
  uint8_t cy : 1;
  uint8_t ac : 1;
  uint8_t pad : 3;
} ConditionCodes;

typedef struct State8080 {
  uint8_t a;
  uint8_t b;
  uint8_t c;
  uint8_t d;
  uint8_t e;
  uint8_t h;
  uint8_t l;
  uint16_t sp;
  uint16_t pc;
  uint8_t *memory;
  struct ConditionCodes cc;
  uint8_t int_enable;
} State8080;

void unimplementedInst(State8080 *state) {
  printf("Error: Unimplemented Instruction\n");
  exit(1);
}

int Parity(int x, int size) {
  int p = 0;
  x = (x & ((1 << size) - 1));
  for (int i = 0; i < size; i++) {
    if (x & 0x1)
      p++;
    x = x >> 1;
  }
  return (0 == (p & 0x1));
}

int Emulate8080p(State8080 *state) {
  unsigned char *opcode = &state->memory[state->pc];
  switch (*opcode) {
  case 0x00: // NOP
    break;
  case 0x01: // LXI    B, word
  {
    state->c = opcode[1];
    state->b = opcode[2];
    state->pc += 2; // advance Program Counter by 2 bytes
  } break;
  case 0x02: // STAX  B
  {
    state->a = state->c;
    state->a = state->b;
    state->pc++;
  } break;
  case 0x03: // INX  B
  {
    uint16_t bc = (state->b << 8) | (state->c);
    bc++;
    state->b = (bc & 0xff00) >> 8;
    state->c = bc & 0xff;
  } break;
  case 0x04: // INR  B
  {
    uint8_t answer = state->b + 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->b = answer;
  } break;
  case 0x05: // DCR B
  {
    uint8_t answer = state->b - 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->b = answer;
  } break;
  case 0x06: // MVI  B, D8
  {
    state->b = opcode[1];
    state->pc++;
  } break;
  case 0x07: // RLC
  {
    uint16_t ac = (uint16_t)state->a;
    ac = ac << 1;
    uint16_t carry = (ac & 0x100);
    if (carry) {
      ac = 0x100 + ac;
      ac = ac & 0xff;
      ac++;
    } else {
      ac = 0x100 + ac;
      ac = ac & 0xff;
    }
  } break;
  case 0x08:
    break;
  case 0x09: // DAD  B
  {
    uint32_t hl = (state->h << 8) | state->l;
    uint32_t bc = (state->b << 8) | state->c;
    uint32_t res = hl + bc;
    state->h = (res & 0xff00) >> 8;
    state->l = res & 0xff;
    state->cc.cy = ((res & 0xffff0000) > 0);
  } break;
  case 0x0a: // LDAX B
  {
    state->a = state->c;
    state->a = state->b;
    state->pc++;
  } break;
  case 0x0b: // DCX  B
  {
    uint16_t bc = (state->b << 8) | (state->c);
    bc--;
    state->b = (bc & 0xff00) >> 8;
    state->c = bc & 0xff;
  } break;
  case 0x0c: // INR  C
  {
    uint8_t answer = state->c + 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->c = answer;
  } break;
  case 0x0d: // DCR  C
  {
    uint8_t answer = state->c - 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->c = answer;
  } break;
  case 0x0e:
    unimplementedInst(state);
    break;
  case 0x0f:
    unimplementedInst(state);
    break;
  case 0x10:
    unimplementedInst(state);
    break;
  case 0x11:
    unimplementedInst(state);
    break;
  case 0x12:
    unimplementedInst(state);
    break;
  case 0x13: // INX  D
  {
    uint16_t de = (state->d << 8) | (state->e);
    de++;
    state->d = (de & 0xff00) >> 8;
    state->e = de & 0xff;
  } break;
  case 0x14: // INR  D
  {
    uint8_t answer = state->d + 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->d = answer;
  } break;
  case 0x15: // DCR  D
  {
    uint8_t answer = state->d - 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->d = answer;
  } break;
  case 0x16:
    unimplementedInst(state);
    break;
  case 0x17:
    unimplementedInst(state);
    break;
  case 0x18:
    unimplementedInst(state);
    break;
  case 0x19:
    unimplementedInst(state);
    break;
  case 0x1a:
    unimplementedInst(state);
    break;
  case 0x1b:
    unimplementedInst(state);
    break;
  case 0x1c: // INR E
  {
    uint8_t answer = state->e + 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->e = answer;
  } break;
  case 0x1d: // DCR  E
  {
    uint8_t answer = state->e - 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->e = answer;
  } break;
  case 0x1e:
    unimplementedInst(state);
    break;
  case 0x1f:
    unimplementedInst(state);
    break;
  case 0x20:
    unimplementedInst(state);
    break;
  case 0x21:
    unimplementedInst(state);
    break;
  case 0x22:
    unimplementedInst(state);
    break;
  case 0x23: // INX  H
  {
    uint16_t hl = (state->h << 8) | (state->l);
    hl++;
    state->h = (hl & 0xff00) >> 8;
    state->l = hl & 0xff;
  } break;
  case 0x24: // INR  H
  {
    uint8_t answer = state->h + 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->h = answer;
  } break;
  case 0x25: // DCR  H
  {
    uint8_t answer = state->h - 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->h = answer;
  } break;
  case 0x26:
    unimplementedInst(state);
    break;
  case 0x27:
    unimplementedInst(state);
    break;
  case 0x28:
    unimplementedInst(state);
    break;
  case 0x29:
    unimplementedInst(state);
    break;
  case 0x2a:
    unimplementedInst(state);
    break;
  case 0x2b:
    unimplementedInst(state);
    break;
  case 0x2c: // INR  L
  {
    uint8_t answer = state->l + 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->l = answer;
  } break;
  case 0x2d: // DCR  L
  {
    uint8_t answer = state->l - 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->l = answer;
  } break;
  case 0x2e:
    unimplementedInst(state);
    break;
  case 0x2f:
    unimplementedInst(state);
    break;
  case 0x30:
    unimplementedInst(state);
    break;
  case 0x31:
    unimplementedInst(state);
    break;
  case 0x32:
    unimplementedInst(state);
    break;
  case 0x33: // INX  SP
    state->sp++;
    break;
  case 0x34: // INR  M
  {
    uint16_t offset = (state->h << 8) | (state->l);
    uint16_t answer = state->memory[offset];
    answer++;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->memory[offset] = answer;
  } break;
  case 0x35: // DCR  M
  {
    uint16_t offset = (state->h << 8) | (state->l);
    uint16_t answer = state->memory[offset];
    answer--;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->memory[offset] = answer;
  } break;
  case 0x36: // MVI  M, byte
  {
    // AC set if lower nibble of h was zero prior to dec
    uint16_t offset = (state->h << 8) | state->l;
    state->memory[offset] = opcode[1];
    state->pc++;
  } break;
  case 0x37:
    unimplementedInst(state);
    break;
  case 0x38:
    unimplementedInst(state);
    break;
  case 0x39:
    unimplementedInst(state);
    break;
  case 0x3a:
    unimplementedInst(state);
    break;
  case 0x3b:
    unimplementedInst(state);
    break;
  case 0x3c: // INR A
  {
    uint8_t answer = state->a + 1;
    state->cc.z = (answer == 0);
    state->cc.s = (0x80 == (answer & 0x80));
    state->cc.ac = (answer > 0xf);
    state->cc.p = Parity(answer, 8);
    state->a = answer;
  } break;
  case 0x3d:
    unimplementedInst(state);
    break;
  case 0x3e:
    unimplementedInst(state);
    break;
  case 0x3f:
    unimplementedInst(state);
    break;
  case 0x40:
    unimplementedInst(state);
    break;
  case 0x41:
    unimplementedInst(state);
    break;
  case 0x42:
    unimplementedInst(state);
    break;
  case 0x43:
    unimplementedInst(state);
    break;
  case 0x44:
    unimplementedInst(state);
    break;
  case 0x45:
    unimplementedInst(state);
    break;
  case 0x46:
    unimplementedInst(state);
    break;
  case 0x47:
    unimplementedInst(state);
    break;
  case 0x48:
    unimplementedInst(state);
    break;
  case 0x49:
    unimplementedInst(state);
    break;
  case 0x4a:
    unimplementedInst(state);
    break;
  case 0x4b:
    unimplementedInst(state);
    break;
  case 0x4c:
    unimplementedInst(state);
    break;
  case 0x4d:
    unimplementedInst(state);
    break;
  case 0x4e:
    unimplementedInst(state);
    break;
  case 0x4f:
    unimplementedInst(state);
    break;
  case 0x50:
    unimplementedInst(state);
    break;
  case 0x51:
    unimplementedInst(state);
    break;
  case 0x52:
    unimplementedInst(state);
    break;
  case 0x53:
    unimplementedInst(state);
    break;
  case 0x54:
    unimplementedInst(state);
    break;
  case 0x55:
    unimplementedInst(state);
    break;
  case 0x56:
    unimplementedInst(state);
    break;
  case 0x57:
    unimplementedInst(state);
    break;
  case 0x58:
    unimplementedInst(state);
    break;
  case 0x59:
    unimplementedInst(state);
    break;
  case 0x5a:
    unimplementedInst(state);
    break;
  case 0x5b:
    unimplementedInst(state);
    break;
  case 0x5c:
    unimplementedInst(state);
    break;
  case 0x5d:
    unimplementedInst(state);
    break;
  case 0x5e:
    unimplementedInst(state);
    break;
  case 0x5f:
    unimplementedInst(state);
    break;
  case 0x60:
    unimplementedInst(state);
    break;
  case 0x61:
    unimplementedInst(state);
    break;
  case 0x62:
    unimplementedInst(state);
    break;
  case 0x63:
    unimplementedInst(state);
    break;
  case 0x64:
    unimplementedInst(state);
    break;
  case 0x65:
    unimplementedInst(state);
    break;
  case 0x66:
    unimplementedInst(state);
    break;
  case 0x67:
    unimplementedInst(state);
    break;
  case 0x68:
    unimplementedInst(state);
    break;
  case 0x69:
    unimplementedInst(state);
    break;
  case 0x6a:
    unimplementedInst(state);
    break;
  case 0x6b:
    unimplementedInst(state);
    break;
  case 0x6c:
    unimplementedInst(state);
    break;
  case 0x6d:
    unimplementedInst(state);
    break;
  case 0x6e:
    unimplementedInst(state);
    break;
  case 0x6f:
    unimplementedInst(state);
    break;
  case 0x70:
    unimplementedInst(state);
    break;
  case 0x71:
    unimplementedInst(state);
    break;
  case 0x72:
    unimplementedInst(state);
    break;
  case 0x73:
    unimplementedInst(state);
    break;
  case 0x74:
    unimplementedInst(state);
    break;
  case 0x75:
    unimplementedInst(state);
    break;
  case 0x76:
    unimplementedInst(state);
    break;
  case 0x77:
    unimplementedInst(state);
    break;
  case 0x78:
    unimplementedInst(state);
    break;
  case 0x79:
    unimplementedInst(state);
    break;
  case 0x7a:
    unimplementedInst(state);
    break;
  case 0x7b:
    unimplementedInst(state);
    break;
  case 0x7c:
    unimplementedInst(state);
    break;
  case 0x7d:
    unimplementedInst(state);
    break;
  case 0x7e:
    unimplementedInst(state);
    break;
  case 0x7f:
    unimplementedInst(state);
    break;
  case 0x80: // ADD    B
  {
    // performing operation at higher precision to capture the carry
    uint16_t answer = (uint16_t)state->a + (uint16_t)state->b;

    // Zero Flag
    if ((answer & 0xff) == 0)
      state->cc.z = 1;
    else
      state->cc.z = 0;

    // Sign Flag (if bit 7 is set)
    if (answer & 0x80)
      state->cc.s = 1;
    else
      state->cc.s = 0;

    // Carry Flag
    if (answer > 0xff)
      state->cc.cy = 1;
    else
      state->cc.cy = 0;

    // Parity
    state->cc.p = Parity(answer, 8);

    state->a = answer & 0xff;
  } break;
  case 0x81: // ADD    C
  {
    uint16_t answer = (uint16_t)state->a + (uint16_t)state->c;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x82: // ADD    D
  {
    uint16_t answer = (uint16_t)state->a + (uint16_t)state->d;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x83: // ADD E
  {
    uint16_t answer = (uint16_t)state->a + (uint16_t)state->e;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x84: // ADD    H
  {
    uint16_t answer = (uint16_t)state->a + (uint16_t)state->h;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x85: // ADD    L
  {
    uint16_t answer = (uint16_t)state->a + (uint16_t)state->l;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x86: // ADD    M
  {
    uint16_t offset = (state->h << 8) | (state->l);
    uint16_t answer = (uint16_t)state->a + state->memory[offset];
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x87: // ADD    A
  {
    uint16_t answer = (uint16_t)state->a + (uint16_t)state->a;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x88: // ADC  B
  {
    uint16_t answer =
        (uint16_t)state->a + (uint16_t)state->b + (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x89: // ADC  C
  {
    uint16_t answer =
        (uint16_t)state->a + (uint16_t)state->c + (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x8a: // ADC  D
  {
    uint16_t answer =
        (uint16_t)state->a + (uint16_t)state->d + (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x8b: // ADC  E
  {
    uint16_t answer =
        (uint16_t)state->a + (uint16_t)state->e + (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x8c: // ADC  H
  {
    uint16_t answer =
        (uint16_t)state->a + (uint16_t)state->h + (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x8d: // ADC  L
  {
    uint16_t answer =
        (uint16_t)state->a + (uint16_t)state->l + (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x8e: // ADC  M
  {
    uint16_t offset = (state->h << 8) | (state->l);
    uint16_t answer =
        (uint16_t)state->a + state->memory[offset] + (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x8f: // ADC  A
  {
    uint16_t answer =
        (uint16_t)state->a + (uint16_t)state->a + (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x90: // SUB  B
  {
    uint16_t answer = (uint16_t)state->a - (uint16_t)state->b;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x91: // SUB  C
  {
    uint16_t answer = (uint16_t)state->a - (uint16_t)state->c;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x92: // SUB  D
  {
    uint16_t answer = (uint16_t)state->a - (uint16_t)state->d;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x93: // SUB  E
  {
    uint16_t answer = (uint16_t)state->a - (uint16_t)state->e;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x94: // SUB  H
  {
    uint16_t answer = (uint16_t)state->a - (uint16_t)state->h;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x95: // SUB  L
  {
    uint16_t answer = (uint16_t)state->a - (uint16_t)state->l;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x96: // SUB  M
  {
    uint16_t offset = (state->h << 8) | (state->l);
    uint16_t answer = (uint16_t)state->a - state->memory[offset];
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x97: // SUB  A
  {
    uint16_t answer = (uint16_t)state->a - (uint16_t)state->a;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x98: // SBB  B
  {
    uint16_t answer =
        (uint16_t)state->a - (uint16_t)state->b - (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x99: // SBB  C
  {
    uint16_t answer =
        (uint16_t)state->a - (uint16_t)state->c - (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x9a: // SBB  D
  {
    uint16_t answer =
        (uint16_t)state->a - (uint16_t)state->d - (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x9b: // SBB  E
  {
    uint16_t answer =
        (uint16_t)state->a - (uint16_t)state->e - (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x9c: // SBB  H
  {
    uint16_t answer =
        (uint16_t)state->a - (uint16_t)state->h - (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x9d: // SBB  L
  {
    uint16_t answer =
        (uint16_t)state->a - (uint16_t)state->l - (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x9e: // SBB  M
  {
    uint16_t offset = (state->h << 8) | (state->l);
    uint16_t answer =
        (uint16_t)state->a - state->memory[offset] - (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0x9f: // SBB  A
  {
    uint16_t answer =
        (uint16_t)state->a - (uint16_t)state->a - (uint16_t)state->cc.cy;
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = Parity(answer, 8);
    state->a = answer & 0xff;
  } break;
  case 0xa0:
    unimplementedInst(state);
    break;
  case 0xa1:
    unimplementedInst(state);
    break;
  case 0xa2:
    unimplementedInst(state);
    break;
  case 0xa3:
    unimplementedInst(state);
    break;
  case 0xa4:
    unimplementedInst(state);
    break;
  case 0xa5:
    unimplementedInst(state);
    break;
  case 0xa6:
    unimplementedInst(state);
    break;
  case 0xa7:
    unimplementedInst(state);
    break;
  case 0xa8:
    unimplementedInst(state);
    break;
  case 0xa9:
    unimplementedInst(state);
    break;
  case 0xaa:
    unimplementedInst(state);
    break;
  case 0xab:
    unimplementedInst(state);
    break;
  case 0xac:
    unimplementedInst(state);
    break;
  case 0xad:
    unimplementedInst(state);
    break;
  case 0xae:
    unimplementedInst(state);
    break;
  case 0xaf:
    unimplementedInst(state);
    break;
  case 0xb0:
    unimplementedInst(state);
    break;
  case 0xb1:
    unimplementedInst(state);
    break;
  case 0xb2:
    unimplementedInst(state);
    break;
  case 0xb3:
    unimplementedInst(state);
    break;
  case 0xb4:
    unimplementedInst(state);
    break;
  case 0xb5:
    unimplementedInst(state);
    break;
  case 0xb6:
    unimplementedInst(state);
    break;
  case 0xb7:
    unimplementedInst(state);
    break;
  case 0xb8:
    unimplementedInst(state);
    break;
  case 0xb9:
    unimplementedInst(state);
    break;
  case 0xba:
    unimplementedInst(state);
    break;
  case 0xbb:
    unimplementedInst(state);
    break;
  case 0xbc:
    unimplementedInst(state);
    break;
  case 0xbd:
    unimplementedInst(state);
    break;
  case 0xbe:
    unimplementedInst(state);
    break;
  case 0xbf:
    unimplementedInst(state);
    break;
  case 0xc0: // RNZ
    if (0 == state->cc.z) {
      state->pc =
          state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
      state->sp += 2;
    }
    break;
  case 0xc1:
    unimplementedInst(state);
    break;
  case 0xc2: // JNZ  adr
    if (0 == state->cc.z)
      state->pc = (opcode[2] << 8) | opcode[1];
    else
      state->pc += 2;
    break;
  case 0xc3: // JMP adr
    state->pc = (opcode[2] << 8) | opcode[1];
    break;
  case 0xc4: // CNZ adr
    if (0 == state->cc.z) {
      uint16_t ret = state->pc + 2;
      state->memory[state->sp - 1] = (ret >> 8) & 0xff;
      state->memory[state->sp - 2] = (ret & 0xff);
      state->sp = state->sp - 2;
      state->pc = (opcode[2] << 8) | opcode[1];
    } else
      state->pc += 2;
    break;
  case 0xc5:
    unimplementedInst(state);
    break;
  case 0xc6:
    unimplementedInst(state);
    break;
  case 0xc7: // RST 0
  {
    uint16_t ret = state->pc + 2;
    state->memory[state->sp - 1] = (ret >> 8) & 0xff;
    state->memory[state->sp - 2] = (ret & 0xff);
    state->sp = state->sp - 2;
    state->pc = 0x00;
  } break;
  case 0xc8: // RZ
    if (1 == state->cc.z) {
      state->pc =
          state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
      state->sp += 2;
    }
    break;
  case 0xc9: // RET
    state->pc = state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
    state->sp += 2;
    break;
  case 0xca: // JZ adr
    if (1 == state->cc.z)
      state->pc = (opcode[2] << 8) | opcode[1];
    else
      state->pc += 2;
    break;
  case 0xcb: //????
    unimplementedInst(state);
    break;
  case 0xcc: // CZ adr
    if (1 == state->cc.z) {
      uint16_t ret = state->pc + 2;
      state->memory[state->sp - 1] = (ret >> 8) & 0xff;
      state->memory[state->sp - 2] = (ret & 0xff);
      state->sp = state->sp - 2;
      state->pc = (opcode[2] << 8) | opcode[1];
    } else
      state->pc += 2;
    break;
  case 0xcd: { // CALL adr
    uint16_t ret = state->pc + 2;
    state->memory[state->sp - 1] = (ret >> 8) & 0xff;
    state->memory[state->sp - 2] = (ret & 0xff);
    state->sp = state->sp - 2;
    state->pc = (opcode[2] << 8) | opcode[1];
  } break;
  case 0xce:
    unimplementedInst(state);
    break;
  case 0xcf: // RST 1
  {
    uint16_t ret = state->pc + 2;
    state->memory[state->sp - 1] = (ret >> 8) & 0xff;
    state->memory[state->sp - 2] = (ret & 0xff);
    state->sp = state->sp - 2;
    state->pc = 0x08;
  } break;
  case 0xd0: // RNC
    if (0 == state->cc.cy) {
      state->pc =
          state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
      state->sp += 2;
    }
    break;
  case 0xd1:
    unimplementedInst(state);
    break;
  case 0xd2:
    unimplementedInst(state);
    break;
  case 0xd3:
    unimplementedInst(state);
    break;
  case 0xd4: // CNC adr
    if (0 == state->cc.cy) {
      uint16_t ret = state->pc + 2;
      state->memory[state->sp - 1] = (ret >> 8) & 0xff;
      state->memory[state->sp - 2] = (ret & 0xff);
      state->sp = state->sp - 2;
      state->pc = 0x00;
    } else
      state->pc += 2;
    break;
  case 0xd5:
    unimplementedInst(state);
    break;
  case 0xd6:
    unimplementedInst(state);
    break;
  case 0xd7:
    unimplementedInst(state);
    break;
  case 0xd8: // RC
    if (1 == state->cc.cy) {
      state->pc =
          state->memory[state->sp] | (state->memory[state->sp + 1] << 8);
      state->sp += 2;
    }
    break;
  case 0xd9:
    unimplementedInst(state);
    break;
  case 0xda:
    unimplementedInst(state);
    break;
  case 0xdb:
    unimplementedInst(state);
    break;
  case 0xdc: // CC adr
    if (1 == state->cc.cy) {
      uint16_t ret = state->pc + 2;
      state->memory[state->sp - 1] = (ret >> 8) & 0xff;
      state->memory[state->sp - 2] = (ret & 0xff);
      state->sp = state->sp - 2;
      state->pc = 0x00;
    } else
      state->pc += 2;
    break;
  case 0xdd:
    unimplementedInst(state);
    break;
  case 0xde:
    unimplementedInst(state);
    break;
  case 0xdf:
    unimplementedInst(state);
    break;
  case 0xe0:
    unimplementedInst(state);
    break;
  case 0xe1:
    unimplementedInst(state);
    break;
  case 0xe2:
    unimplementedInst(state);
    break;
  case 0xe3:
    unimplementedInst(state);
    break;
  case 0xe4:
    unimplementedInst(state);
    break;
  case 0xe5:
    unimplementedInst(state);
    break;
  case 0xe6:
    unimplementedInst(state);
    break;
  case 0xe7:
    unimplementedInst(state);
    break;
  case 0xe8:
    unimplementedInst(state);
    break;
  case 0xe9:
    unimplementedInst(state);
    break;
  case 0xea:
    unimplementedInst(state);
    break;
  case 0xeb:
    unimplementedInst(state);
    break;
  case 0xec:
    unimplementedInst(state);
    break;
  case 0xed:
    unimplementedInst(state);
    break;
  case 0xee:
    unimplementedInst(state);
    break;
  case 0xef:
    unimplementedInst(state);
    break;
  case 0xf0:
    unimplementedInst(state);
    break;
  case 0xf1:
    unimplementedInst(state);
    break;
  case 0xf2:
    unimplementedInst(state);
    break;
  case 0xf3:
    unimplementedInst(state);
    break;
  case 0xf4:
    unimplementedInst(state);
    break;
  case 0xf5:
    unimplementedInst(state);
    break;
  case 0xf6:
    unimplementedInst(state);
    break;
  case 0xf7:
    unimplementedInst(state);
    break;
  case 0xf8:
    unimplementedInst(state);
    break;
  case 0xf9:
    unimplementedInst(state);
    break;
  case 0xfa:
    unimplementedInst(state);
    break;
  case 0xfb:
    unimplementedInst(state);
    break;
  case 0xfc:
    unimplementedInst(state);
    break;
  case 0xfd:
    unimplementedInst(state);
    break;
  case 0xfe:
    unimplementedInst(state);
    break;
  case 0xff:
    unimplementedInst(state);
    break;
  }
  return 0; // placeholder
}

int disassemble(unsigned char *buffer, int pc); // disassembler decl
int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "rb");
  if (f == NULL) {
    printf("Error: could not open %s\n", argv[1]);
    exit(1);
  }
  // Read code into into buffer
  fseek(f, 0L, SEEK_END);
  int fsize = ftell(f); // grabbing total file size
  fseek(f, 0L, SEEK_SET);
  unsigned char *buffer = malloc(fsize);
  fread(buffer, fsize, 1, f);
  fclose(f);
  // Perform Disassembly
  int pc = 0;
  while (pc < fsize) {
    pc += disassemble(buffer, pc);
  }

  return 0;
}

int disassemble(unsigned char *buffer, int pc) {
  // Performs actual disassembly of 8080 Machine Code
  // Params:
  //	unsigned char *buffer - Buffer containing the machine code
  //	int pc - Program Counter
  //
  // Returns:
  //	int - New Program Counter Location

  unsigned char *opcode = &buffer[pc];
  int opbytes = 1;
  printf("%04x ", pc);
  switch (*opcode) {
  case 0x00:
    printf("NOP");
    break;
  case 0x01:
    printf("LXI\tB, #$0x%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0x02:
    printf("STAX\tB");
    break;
  case 0x03:
    printf("INX\tB");
    break;
  case 0x04:
    printf("INR\tB");
    break;
  case 0x05:
    printf("DCR\tB");
    break;
  case 0x06:
    printf("MVI\tB, #$0x%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0x07:
    printf("RLC");
    break;
  case 0x09:
    printf("DAD\tB");
    break;
  case 0x0a:
    printf("LDAX\tB");
    break;
  case 0x0b:
    printf("DCX\tB");
    break;
  case 0x0c:
    printf("INR\tC");
    break;
  case 0x0d:
    printf("DCR\tC");
    break;
  case 0x0e:
    printf("MVI\tC, #$0x%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0x0f:
    printf("RRC");
    break;
  case 0x10:
    break;
  case 0x11:
    printf("LXI\tD, #$0x%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0x12:
    printf("STAX\tD");
    break;
  case 0x13:
    printf("INX\tD");
    break;
  case 0x14:
    printf("INR\tD");
    break;
  case 0x15:
    printf("DCR\tD");
    break;
  case 0x16:
    printf("MVI\tD, #$0x%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0x17:
    printf("RAL");
    break;
  case 0x18:
    break;
  case 0x19:
    printf("DAD\tD");
    break;
  case 0x1a:
    printf("LDAX\tD");
    break;
  case 0x1b:
    printf("DCX\tD");
    break;
  case 0x1c:
    printf("INR\tE");
    break;
  case 0x1d:
    printf("DCR\tE");
    break;
  case 0x1e:
    printf("MVI\tE, #$0x%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0x1f:
    printf("RAR");
    break;
  case 0x20:
    break;
  case 0x21:
    printf("LXI\tH, #$0x%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0x22:
    printf("SHLD\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0x23:
    printf("INX\tH");
    break;
  case 0x24:
    printf("INR\tH");
    break;
  case 0x25:
    printf("DCR\tH");
    break;
  case 0x26:
    printf("MVI\tH, #$0x%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0x27:
    printf("DAA");
    break;
  case 0x28:
    break;
  case 0x29:
    printf("DAD\tH");
    break;
  case 0x2a:
    printf("LHLD\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0x2b:
    printf("DCX\tH");
    break;
  case 0x2c:
    printf("INR\tL");
    break;
  case 0x2d:
    printf("DCR\tL");
    break;
  case 0x2e:
    printf("MVI\tL, #$0x%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0x2f:
    printf("CMA");
    break;
  case 0x30:
    break;
  case 0x31:
    printf("LXI\tSP, #$0x%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0x32:
    printf("STA\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0x33:
    printf("INX\tSP");
    break;
  case 0x34:
    printf("INR\tM");
    break;
  case 0x35:
    printf("DCR\tM");
    break;
  case 0x36:
    printf("MVI\tM, #$0x%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0x37:
    printf("STC");
    break;
  case 0x38:
    break;
  case 0x39:
    printf("DAD\tSP");
    break;
  case 0x3a:
    printf("LDA\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0x3b:
    printf("DCX\tSP");
    break;
  case 0x3c:
    printf("INR\tA");
    break;
  case 0x3d:
    printf("DCR\tA");
    break;
  case 0x3e:
    printf("MVI\tA, #$0x%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0x3f:
    printf("CMC");
    break;
  case 0x40:
    printf("MOV\tB, B");
    break;
  case 0x41:
    printf("MOV\tB, C");
    break;
  case 0x42:
    printf("MOV\tB, D");
    break;
  case 0x43:
    printf("MOV\tB, E");
    break;
  case 0x44:
    printf("MOV\tB, H");
    break;
  case 0x45:
    printf("MOV\tB, L");
    break;
  case 0x46:
    printf("MOV\tB, M");
    break;
  case 0x47:
    printf("MOV\tB, A");
    break;
  case 0x48:
    printf("MOV\tC, B");
    break;
  case 0x49:
    printf("MOV\tC, C");
    break;
  case 0x4a:
    printf("MOV\tC, D");
    break;
  case 0x4b:
    printf("MOV\tC, E");
    break;
  case 0x4c:
    printf("MOV\tC, F");
    break;
  case 0x4d:
    printf("MOV\tC, L");
    break;
  case 0x4e:
    printf("MOV\tC, M");
    break;
  case 0x4f:
    printf("MOV\tC, A");
    break;
  case 0x50:
    printf("MOV\tD, B");
    break;
  case 0x51:
    printf("MOV\tD, C");
    break;
  case 0x52:
    printf("MOV\tD, D");
    break;
  case 0x53:
    printf("MOV\tD, E");
    break;
  case 0x54:
    printf("MOV\tD, H");
    break;
  case 0x55:
    printf("MOV\tD, L");
    break;
  case 0x56:
    printf("MOV\tD, M");
    break;
  case 0x57:
    printf("MOV\tD, A");
    break;
  case 0x58:
    printf("MOV\tE, B");
    break;
  case 0x59:
    printf("MOV\tE, C");
    break;
  case 0x5a:
    printf("MOV\tE, D");
    break;
  case 0x5b:
    printf("MOV\tE, E");
    break;
  case 0x5c:
    printf("MOV\tE, H");
    break;
  case 0x5d:
    printf("MOV\tE, L");
    break;
  case 0x5e:
    printf("MOV\tE, M");
    break;
  case 0x5f:
    printf("MOV\tE, A");
    break;
  case 0x60:
    printf("MOV\tH, B");
    break;
  case 0x61:
    printf("MOV\tH, C");
    break;
  case 0x62:
    printf("MOV\tH, D");
    break;
  case 0x63:
    printf("MOV\tH, E");
    break;
  case 0x64:
    printf("MOV\tH, H");
    break;
  case 0x65:
    printf("MOV\tH, L");
    break;
  case 0x66:
    printf("MOV\tH, M");
    break;
  case 0x67:
    printf("MOV\tH, A");
    break;
  case 0x68:
    printf("MOV\tL, B");
    break;
  case 0x69:
    printf("MOV\tL, C");
    break;
  case 0x6a:
    printf("MOV\tL, D");
    break;
  case 0x6b:
    printf("MOV\tL, E");
    break;
  case 0x6c:
    printf("MOV\tL, H");
    break;
  case 0x6d:
    printf("MOV\tL, L");
    break;
  case 0x6e:
    printf("MOV\tL, M");
    break;
  case 0x6f:
    printf("MOV\tL, A");
    break;
  case 0x70:
    printf("MOV\tM, B");
    break;
  case 0x71:
    printf("MOV\tM, C");
    break;
  case 0x72:
    printf("MOV\tM, D");
    break;
  case 0x73:
    printf("MOV\tM, E");
    break;
  case 0x74:
    printf("MOV\tM, H");
    break;
  case 0x75:
    printf("MOV\tM, L");
    break;
  case 0x76:
    printf("HLT");
    break;
  case 0x77:
    printf("MOV\tM, A");
    break;
  case 0x78:
    printf("MOV\tA, B");
    break;
  case 0x79:
    printf("MOV\tA, C");
    break;
  case 0x7a:
    printf("MOV\tA, D");
    break;
  case 0x7b:
    printf("MOV\tA, E");
    break;
  case 0x7c:
    printf("MOV\tA, H");
    break;
  case 0x7d:
    printf("MOV\tA, L");
    break;
  case 0x7e:
    printf("MOV\tA, M");
    break;
  case 0x7f:
    printf("MOV\tA, A");
    break;
  case 0x80:
    printf("ADD\tB");
    break;
  case 0x81:
    printf("ADD\tC");
    break;
  case 0x82:
    printf("ADD\tD");
    break;
  case 0x83:
    printf("ADD\tE");
    break;
  case 0x84:
    printf("ADD\tH");
    break;
  case 0x85:
    printf("ADD\tL");
    break;
  case 0x86:
    printf("ADD\tM");
    break;
  case 0x87:
    printf("ADD\tA");
    break;
  case 0x88:
    printf("ADC\tB");
    break;
  case 0x89:
    printf("ADC\tC");
    break;
  case 0x8a:
    printf("ADC\tD");
    break;
  case 0x8b:
    printf("ADC\tE");
    break;
  case 0x8c:
    printf("ADC\tH");
    break;
  case 0x8d:
    printf("ADC\tL");
    break;
  case 0x8e:
    printf("ADC\tM");
    break;
  case 0x8f:
    printf("ADC\tA");
    break;
  case 0x90:
    printf("SUB\tB");
    break;
  case 0x91:
    printf("SUB\tC");
    break;
  case 0x92:
    printf("SUB\tD");
    break;
  case 0x93:
    printf("SUB\tE");
    break;
  case 0x94:
    printf("SUB\tH");
    break;
  case 0x95:
    printf("SUB\tL");
    break;
  case 0x96:
    printf("SUB\tM");
    break;
  case 0x97:
    printf("SUB\tA");
    break;
  case 0x98:
    printf("SBB\tB");
    break;
  case 0x99:
    printf("SBB\tC");
    break;
  case 0x9a:
    printf("SBB\tD");
    break;
  case 0x9b:
    printf("SBB\tE");
    break;
  case 0x9c:
    printf("SBB\tH");
    break;
  case 0x9d:
    printf("SBB\tL");
    break;
  case 0x9e:
    printf("SBB\tM");
    break;
  case 0x9f:
    printf("SBB\tA");
    break;
  case 0xa0:
    printf("ANA\tB");
    break;
  case 0xa1:
    printf("ANA\tC");
    break;
  case 0xa2:
    printf("ANA\tD");
    break;
  case 0xa3:
    printf("ANA\tE");
    break;
  case 0xa4:
    printf("ANA\tH");
    break;
  case 0xa5:
    printf("ANA\tL");
    break;
  case 0xa6:
    printf("ANA\tM");
    break;
  case 0xa7:
    printf("ANA\tA");
    break;
  case 0xa8:
    printf("XRA\tB");
    break;
  case 0xa9:
    printf("XRA\tC");
    break;
  case 0xaa:
    printf("XRA\tD");
    break;
  case 0xab:
    printf("XRA\tE");
    break;
  case 0xac:
    printf("XRA\tH");
    break;
  case 0xad:
    printf("XRA\tL");
    break;
  case 0xae:
    printf("XRA\tM");
    break;
  case 0xaf:
    printf("XRA\tA");
    break;
  case 0xb0:
    printf("ORA\tB");
    break;
  case 0xb1:
    printf("ORA\tC");
    break;
  case 0xb2:
    printf("ORA\tD");
    break;
  case 0xb3:
    printf("ORA\tE");
    break;
  case 0xb4:
    printf("ORA\tH");
    break;
  case 0xb5:
    printf("ORA\tL");
    break;
  case 0xb6:
    printf("ORA\tM");
    break;
  case 0xb7:
    printf("ORA\tA");
    break;
  case 0xb8:
    printf("CMP\tB");
    break;
  case 0xb9:
    printf("CMP\tC");
    break;
  case 0xba:
    printf("CMP\tD");
    break;
  case 0xbb:
    printf("CMP\tE");
    break;
  case 0xbc:
    printf("CMP\tH");
    break;
  case 0xbd:
    printf("CMP\tL");
    break;
  case 0xbe:
    printf("CMP\tM");
    break;
  case 0xbf:
    printf("CMP\tA");
    break;
  case 0xc0:
    printf("RNZ");
    break;
  case 0xc1:
    printf("POP\tB");
    break;
  case 0xc2:
    printf("JNZ\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xc3:
    printf("JMP\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xc4:
    printf("CNZ\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xc5:
    printf("PUSH\tB");
    break;
  case 0xc6:
    printf("ADI\t#$%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0xc7:
    printf("RST\t0");
    break;
  case 0xc8:
    printf("RZ");
    break;
  case 0xc9:
    printf("RET");
    break;
  case 0xca:
    printf("JZ\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xcb:
    printf("JMP\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xcc:
    printf("CZ\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xcd:
    printf("CALL\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xce:
    printf("ACI\t#$%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0xcf:
    printf("RST\t1");
    break;
  case 0xd0:
    printf("RNC");
    break;
  case 0xd1:
    printf("POP\tD");
    break;
  case 0xd2:
    printf("JNC\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xd3:
    printf("OUT\t#$%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0xd4:
    printf("CNC\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xd5:
    printf("PUSH\tD");
    break;
  case 0xd6:
    printf("SUI\t#$%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0xd7:
    printf("RST\t2");
    break;
  case 0xd8:
    printf("RC");
    break;
  case 0xd9:
    printf("RET");
    break;
  case 0xda:
    printf("JC\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xdb:
    printf("IN\t#$%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0xdc:
    printf("CC\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xdd:
    printf("CALL\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xde:
    printf("SBI\t#$%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0xdf:
    printf("RST\t3");
    break;
  case 0xe0:
    printf("RPO");
    break;
  case 0xe1:
    printf("POP\tH");
    break;
  case 0xe2:
    printf("JPO\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xe3:
    printf("XTHL");
    break;
  case 0xe4:
    printf("CPO\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xe5:
    printf("PUSH\tH");
    break;
  case 0xe6:
    printf("ANI\t#$%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0xe7:
    printf("RST\t4");
    break;
  case 0xe8:
    printf("RPE");
    break;
  case 0xe9:
    printf("PCHL");
    break;
  case 0xea:
    printf("JPE\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xeb:
    printf("XCHG");
    break;
  case 0xec:
    printf("CPE\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xed:
    printf("CALL\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xee:
    printf("XRI\t#$%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0xef:
    printf("RST\t5");
    break;
  case 0xf0:
    printf("RP");
    break;
  case 0xf1:
    printf("POP\tPSW");
    break;
  case 0xf2:
    printf("JP\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xf3:
    printf("DI");
    break;
  case 0xf4:
    printf("CP\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xf5:
    printf("PUSH\tPSW");
    break;
  case 0xf6:
    printf("ORI\t#$%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0xf7:
    printf("RST\t6");
    break;
  case 0xf8:
    printf("RM");
    break;
  case 0xf9:
    printf("SPHL");
    break;
  case 0xfa:
    printf("JM\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xfb:
    printf("EI");
    break;
  case 0xfc:
    printf("CM\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xfd:
    printf("CALL\t$%02x%02x", opcode[2], opcode[1]);
    opbytes = 3;
    break;
  case 0xfe:
    printf("CPI\t#$%02x", opcode[1]);
    opbytes = 2;
    break;
  case 0xff:
    printf("RST\t7");
    break;

  default: // Unknown instruction handling
    printf("Unknown Instruction: 0x%02x", *opcode);
    break;
  }
  printf("\n");
  return opbytes;
}
