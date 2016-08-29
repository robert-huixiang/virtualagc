/****************************************************************************
 *  ALU - ARITHMETIC UNIT subsystem
 *
 *  AUTHOR:     John Pultorak
 *  DATE:       9/22/01
 *  FILE:       ALU.h
 *
 *  VERSIONS:
 * 
 *  DESCRIPTION:
 *    Arithmetic Unit for the Block 1 Apollo Guidance Computer prototype (AGC4).
 *
 *  SOURCES:
 *    Mostly based on information from "Logical Description for the Apollo 
 *    Guidance Computer (AGC4)", Albert Hopkins, Ramon Alonso, and Hugh 
 *    Blair-Smith, R-393, MIT Instrumentation Laboratory, 1963.
 *
 *  NOTES: 
 *    
 *****************************************************************************
 */
#ifndef ALU_H
#define ALU_H

#include "reg.h"

class regB : public reg
  {
  public:
    regB() : reg(16, "%06o")
      {}
    virtual ~regB()
      {};
  };

class regCI : public reg
  {
  public:
    regCI() : reg(1, "%01o")
      {}
    virtual ~regCI()
      {};
  };

class regX : public reg
  {
  public:
    regX() : reg(16, "%06o")
      {}
    virtual ~regX()
      {};
  };

class regY : public reg
  {
  public:
    regY() : reg(16, "%06o")
      {}
    virtual ~regY()
      {};
  };

class regU : public reg
  {
  public:
    regU() : reg(16, "%06o")
      {}
    virtual unsigned read();
    virtual ~regU()
      {};
  };

class ALU
  {
  public:
    static unsigned glbl_BUS;  // mixes the RC and RU together for MASK

    // In the hardware AGC, all read pulses are enabled simultaneously
    // by CLK1. This simulator has to do the pulses one-at-a-time, so
    // they are executed in the following sequence to mimic the hardware:
    //
    // 1) all read pulses involving subsystems other than ALU are executed,
    //		These read pulses output to the glbl_READ_BUS. Only 0 or 1
    //		of these pulses should be active at any time (never 2 or more),
    //
    // 2) next, the read pulses for the ALU are executed. The ALU is treated
    //		differently because it is the only subsystem where several read
    //		pulses can be active simultaneously. In the original AGC, these
    //		pulses 'inclusive OR' their output to the glbl_READ_BUS, so the
    //		simulator has be implemented to execute all read pulses other than
    //		the ALU reads first, so the ALU will have the bus data it needs
    //		in order to do the inclusive OR.
    //		In the recreated AGC hardware design, the ALU is also the subsystem
    //		that links the glbl_READ_BUS to the glbl_WRITE_BUS.
    //
    //		The recreated ALU hardware design checks whether anything is being
    //		written to the glbl_READ_BUS by the other subsystems. If not, it
    //		outputs zeroes to the glbl_READ_BUS for input to the inclusive OR
    //		operation.
    //		It then transfers data on the glbl_READ_BUS to the glbl_WRITE_BUS
    //		using an inclusive OR with data generated by other ALU read pulses.
    //		The AGC sequencer uses this operation to set certain data lines.
    //
    // 3) finally, all write pulses are executed.

    static void execRP_ALU_RB();
    static void execRP_ALU_RC();
    static void execRP_ALU_RU();

    static void execRP_ALU_OR_RB14();
    static void execRP_ALU_OR_R1();
    static void execRP_ALU_OR_R1C();
    static void execRP_ALU_OR_R2();
    static void execRP_ALU_OR_R22();
    static void execRP_ALU_OR_R24();
    static void execRP_ALU_OR_R2000();
    static void execRP_ALU_OR_RSB();

    static void execWP_GENRST();
    static void execWP_WB();

    static void execWP_CI();
    static void execWP_WY();

    static void execWP_WX();
    static void execWP_WYx();

    static regB register_B;// next instruction
    static regCI register_CI;// ALU carry-in flip flop
    static regX register_X;// ALU X register
    static regY register_Y;// ALU Y register
    static regU register_U;// ALU sum
  };

#endif
