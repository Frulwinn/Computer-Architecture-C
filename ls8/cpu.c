#include "cpu.h"
#include <stdio.h>
#include <stlib.h>

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
  char line = [1024]

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
    while (fgets(line, 1024, fp) ! = NULL) {
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

  while (running) {
    //4 variables opA, opB, IR (use ram read in these var)
    unsigned char opA = 0;
    unsigned char opB = 0;
    unsigned char IR = cpu_ram_read;
    //need another variable using bitwise to read by shifting 6
    unsigned int num_op = IR >> 6;

    //if else what to do when you have 2 or 1 operand
    //ram read cpu
    // Get the appropriate value(s) of the operands following this instruction
    //read the bytes at PC+1 and PC+2 from RAM into variables operandA and operandB
    if (num_op == 2) {
      opA = cpu_ram_read(cpu->PC + 1);
      opB = cpu_ram_read(cpu->PC + 2);
    } else if (num_op == 1) {
      opA = cpu_ram_read(cpu->PC + 1);
    } else {
      return 0;
    }

    // Get the value of the current instruction (in address PC).
    // Figure out how many operands this next instruction requires
    int pc_instruction = IR;
    
    //state with hex decimal?

    //add IR
    //switch() over it to decide on a course of action.
    switch (IR) {
      case LDI:
        print("LDI\n");
        break;

      case PRN:
        print("PRN\n");
        break;

      case HLT:
        running = 0
        break;

      default:
        exit(1);
      }
    // Do whatever the instruction should do according to the spec.
    // Move the PC to the next instruction.
    cpu->PC = num_op++;
  }
  return 0;
}

//Added RAM functions
void cpu_ram_read(struct cpu *cpu) 
{
  //think about index
  //read from a specific index in Ram
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu) 
{
  //think about index
  //specified index in ram is being written to the pc
  cpu->ram[index] = cpu->PC
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  //int PC = 0;
  cpu->PC = 0
  //first, the PC, registers, and RAM should be cleared to zero
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
}
