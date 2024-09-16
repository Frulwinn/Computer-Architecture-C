#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"


unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char index)
{
  cpu->ram[index] = value;
}

//push a value on the CPU stack
void cpu_push(struct cpu *cpu, unsigned char value)
{
  cpu->registers[7]--;
  cpu_ram_write(cpu, value, cpu->registers[7]);
}

//pop a value from the CPU stack
unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char value = cpu_ram_read(cpu, cpu->registers[7]);
  cpu->registers[7]++;

  return value;
}

void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  int address = ADDR_PROGRAM_ENTRY;

  //open the source file
  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  //read all the lines and store them in RAM
  while (fgets(line, sizeof line, fp) != NULL) {

    //convert string to a number
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);;

    //ignore lines from which no numbers were read
    if (endchar == line) {
      continue;
    }

    //store in ram
    cpu_ram_write(cpu, byte, address++);
  }
}

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{

  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
      break;

    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;

    case ALU_INC:
      cpu->registers[regA]++;
      break;

    case ALU_DEC:
      cpu->registers[regA]--;
      break;

    case ALU_CMP:
      //clear the < > = flags before setting the appropriate one
      cpu->FL &= ~0b111;

      if (cpu->registers[regA] == regB) {
        //set the last bit of FL to 1
        cpu->FL = cpu->FL | (1 << 0);
      } else if (cpu->registers[regA] > regB) {
        //set the second-to-last bit of FL to 1
        cpu->FL = cpu->FL | (1 << 1);
      } else {
        //set the third-to-last bit of FL to 1
        cpu->FL = cpu->FL | (1 << 2);
      }
      break;
  }
}

void cpu_run(struct cpu *cpu)
{

  int running = 1; // True until we get a HLT instruction

  while (running) {
    unsigned char opA = 0;
    unsigned char opB = 0;
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned int num_op = IR >> 6;

    if (num_op == 2) {
      opA = cpu_ram_read(cpu, (cpu->PC + 1));
      opB = cpu_ram_read(cpu, (cpu->PC + 2));
    } else if (num_op == 1) {
      opA = cpu_ram_read(cpu, (cpu->PC + 1));
    } 

    //this line is shifting the instruction by 4 bits to access
    //the flag that indicates whether the PC might be set, and
    //then seeing if the bit is set to 0 or 1
    int instruction_set_pc = (IR >> 4) & 1;

    switch (IR) {
      case LDI:
        cpu->registers[opA] = opB;
        break;

      case PRN:
        printf("%d\n", cpu->registers[opA]);
        break;

      case MUL:
        alu(cpu, ALU_MUL, opA, opB);
        break;

      case ADD:
        alu(cpu, ALU_ADD, opA, opB);
        break;

      case HLT:
        running = 0;
        break;

      // case PUSH:
      //   cpu_push(cpu, cpu->registers[opA]);
      //   break;

      // case POP:
      //   cpu->registers[opA] = cpu_pop(cpu);
      //   break;

      default:
        fprintf(stderr, "PC %02x: unknown instruction %02x\n", cpu->PC, IR);
        exit(2);
    }

    if (!instruction_set_pc) {
      //increment PC by the number of arguments that were passed to the instruction we just executed
      cpu->PC += num_op + 1;
    }
  }
}

void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->FL = 0;

  //zero registers and RAM
  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);

  //initialize SP
  cpu->registers[7] = ADDR_EMPTY_STACK;
}
