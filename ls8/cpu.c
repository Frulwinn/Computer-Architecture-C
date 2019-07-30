#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
  FILE *fp;
  char line = '//'

  //check
    if (argc != 2) {
        printf("usage: filesio filename\n");
        return 1;
    }

    fp = fopen(argv[1], "r");

    //error checking
    if (fp == NULL) {
        printf("error opening file %s\n", argv[1]);
    }

    //saving, terminating, at file
    while (fgets(line, '//', fp) ! = NULL) {
        //if there is an error
        char *endptr;
        unsigned char val = strtoul(line, &endptr, 2);

        //error checking if you get jibberish code
        if (line == endptr) {
            printf("This is just a comment: %s", line);
        }
        printf("%02x\n", val);
    }
    fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  int PC = 0; //value of current instruction
  int IR[] = {
    LDI,
    PRN,
    HLT
    };//instruction register

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char x = ram[PC];
    // 2. Figure out how many operands this next instruction requires
    registers[operandA] += registers[operandB];
    // 3. Get the appropriate value(s) of the operands following this instruction
    //read the bytes at PC+1 and PC+2 from RAM into variables operandA and operandB
    cpu_ram_read(&cpu); 
    ram[PC+1] = operandA;
    ram[PC+2] = operandB;
    
    // 4. switch() over it to decide on a course of action.
    switch(x) {
      case LDI:
        print("LDI\n");
        break;

      case PRN:
        print("PRN\n");
        break;

      default:
        print("exit loop\n");
        HLT;
        running = 0;
        break;
      }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    PC++;
  }
  return 0;
}

//Added RAM functions
void cpu_ram_read(struct cpu *cpu) {
  //access the RAM inside the struct cpu
  return cpu->ram;//[cpu->pc];
}

void cpu_ram_write(struct cpu *cpu) {
  //access the RAM inside the struct cpu
  return cpu->ram;
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  //int PC = 0;
  cpu->pc = 0
  //first, the PC, registers, and RAM should be cleared to zero
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
}
