#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) 
{
  //think about index
  //read value from a specific index in Ram
  return cpu->ram[index];
}

//missing index and value in parameters
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value) 
{
  //think about index
  //write ram index to value
  cpu->ram[index] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
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
  char line[1024];

    //open file
    //error checking
    if ((fp = fopen(filename, "r")) == NULL) {
      printf("error opening file %s\n", filename);
      exit(2);
    }

    //saving, terminating, at file reading the lines and storing it in the RAM
    while (fgets(line, 1024, fp) != NULL) {
        //if there is an error
        char *endptr;
        unsigned char val = strtoul(line, &endptr, 2);

        //error checking if you get jibberish code
        if (line == endptr) {
          printf("This is just a comment: %s", line);
          continue;
        }
        //write in ram
        cpu_ram_write(cpu, val, address++);
        printf("%02x\n", val);
    }
    fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
//hack to fix warning unused parameter
// (void)cpu;
// (void)regA;
// (void)regB;

  unsigned char *reg = cpu->registers;
  unsigned char valB = reg[regB];

  switch (op) {
    //Multiply the values in two registers together and 
    //store the result in registerA
    case ALU_MUL:
      reg[regA] *= valB;
      break;

    //Add the value in two registers and store the result in registerA.
    case ALU_ADD:
      reg[regA] += valB;
      break;

    //Increment (add 1 to) the value in the given register.
    case ALU_INC:
      reg[regA]++;
      break;

    //Decrement (subtract 1 from) the value in the given register.
    case ALU_DEC:
      reg[regA]--;
      break;

    //Compare the values in two registers.
    //If they are equal, set the Equal E flag to 1, otherwise set it to 0.
    //If registerA is less than registerB, set the Less-than L flag to 1, otherwise set it to 0.
    //If registerA is greater than registerB, set the Greater-than G flag to 1, otherwise set it to 0.
    case ALU_CMP:
      // Clear the < > = flags before setting the appropriate one
      cpu->FL &= ~0b111;

      if (reg[regA] == valB) {
        // Set the last bit of FL to 1
        cpu->FL = cpu->FL | (1 << 0);
      } else if (reg[regA] > valB) {
        // Set the second-to-last bit of FL to 1
        cpu->FL = cpu->FL | (1 << 1);
      } else {
        // Set the third-to-last bit of FL to 1
        cpu->FL = cpu->FL | (1 << 2);
      }
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  cpu->registers[7] = 120;

  while (running) {
    //4 variables opA, opB, IR (use ram read in these var)
    unsigned char opA = 0;
    unsigned char opB = 0;
    //included complete parameters for func
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    //need another variable using bitwise to read by shifting 6 only need 2
    unsigned int num_op = IR >> 6;

    //if else what to do when you have 2 or 1 operand
    //ram read cpu
    // Get the appropriate value(s) of the operands following this instruction
    //read the bytes at PC+1 and PC+2 from RAM into variables operandA and operandB
    if (num_op == 2) {
      //added complete parameter
      opA = cpu_ram_read(cpu, (cpu->PC + 1));
      opB = cpu_ram_read(cpu, (cpu->PC + 2));
    } else if (num_op == 1) {
      //added complete parameter
      opA = cpu_ram_read(cpu, (cpu->PC + 1));
      
    } 

    // Get the value of the current instruction (in address PC).
    // Figure out how many operands this next instruction requires
    //int pc_instruction = (IR);

    //add IR
    //switch() over it to decide on a course of action.
    switch (IR) {
      
      case LDI:
        //Set the value of a register to an integer
        cpu->registers[opA] = opB;
        break;

      case PRN:
        //Print numeric value stored in the given register.
        //Print to the console the decimal integer value that is stored in the given register
        printf("%d\n", cpu->registers[opA]);
        break;

      case HLT:
        //Halt the CPU (and exit the emulator).
        running = 0;
        break;

      case PUSH:
        
        break;

      case POP:
      
        break;


      default:
        printf("Unrecognized instruction\n");
        exit(1);
        break;
      }
    // Do whatever the instruction should do according to the spec.
    // Move the PC to the next instruction.
    //incrementing pc by the number of arguments passed to the instructions executed
    cpu->PC += num_op + 1;
  } //closing while
  
}



/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  //int PC = 0;
  cpu->PC = 0;
  //first, the PC, registers, and RAM should be cleared to zero
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
}
