/*** 
   * Program Name: lli_undef_fix
   *
   * File Name: test_APInt_wrap.cpp
   *
   * File Description: 
   *
   * <$project_name) was written by Christian A. Schreiner at University of
   * Utah.  Copyright (C) 2014-2014 by University of Utah.  All rights
   * reserved.  You may use, examine, or modify this file only in accordance
   * with the GNU Public License, or, alternately, by special written
   * arrangement with the author.  This file comes with no warranties.  If you
   * use it and something breaks, you are solely responsible for cleaning up
   * afterwards.
   *
   */

/*** Configuration Management Information:
   * 
   * $Source: /home/cas/templates/auto_uut/RCS/cas_tmpl.c,v $
   * $File: $
   * $Author: cas $
   * $Date: 2014/05/02 19:51:11 $
   * $Revision: 1.5 $
   * 
   */

/*** module summary
   *
   */

/*** **************************************************************************
   *   includes
   * **************************************************************************
   */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "llvm/ADT/APInt.h"

using namespace std;

/*** **************************************************************************
   *   declarations
   * **************************************************************************
   */

/*** ==========================================================================
   *   un-submoduled function prototypes
   * ==========================================================================
   */

/*** **************************************************************************
   *   source code
   * **************************************************************************
   */

/*** --------------------------------------------------------------------------
   * function boolSignedToString()
   * --------------------------------------------------------------------------
   * Description: 
   *
   * Method: 
   *
   * Reentrancy: 
   *
   * Inputs: 
   *  sign: true to give "s" (signed), false for "u" (unsigned)
   *    
   * Outputs: none
   *
   * Return Value: "s" or "u"
   */
std::string boolSignedToString( bool sign )
{{
  return (std::string) ( sign ? "s" : "u" );
}}

/*** --------------------------------------------------------------------------
   * function testAddWrapBehavior1Word()
   * --------------------------------------------------------------------------
   * Description: 
   *	induce wraparound, and prints out the answers.
   *
   * Method: 
   *
   * Reentrancy: 
   *
   * Inputs: 
   *    numBits: the number of significant bits in these integers
   *    aaVal, aaSign: number of the first operand, and true iff it is signed
   *    op: '+' for addition, '-' for subtraction
   *    bbVal, bbSign: number of the second operand, and true iff it is signed
   *    expect: the expected result
   *
   * Outputs: none
   *
   * Return Value: a value recommended for the return value for main(~)
   *
   */
int testAddWrapBehavior1Word( const unsigned numBits, 
			      const uint64_t aaVal, const bool aaSign,
			      char op, 
			      const uint64_t bbVal, const bool bbSign, 
			      std::string expect, std::string flagExpect )
{{
  llvm::APInt aa( numBits, aaVal, aaSign );
  llvm::APInt bb( numBits, bbVal, bbSign );
  llvm::APInt cc;

  switch ( op ) {
  case '+':
    cc= aa + bb;
    break;
  case '-':
    cc= aa - bb;
    break;
  default:
    assert(0);
  } //switch

  cout << numBits << " bits: \"" << 
      aa.toString(16, false)  << boolSignedToString(aaSign) <<
      "\" " << op << " \"" << 
      bb.toString(16, false)  << boolSignedToString(bbSign) <<
      "\" = \"" << cc.toString(16, false) << 
      "\"\n";
  cout << "   (should be " << expect << ") \n";
  cout << "   (" << aa.flagsToString() << ")+(" << bb.flagsToString() <<
      ") = (" << cc.flagsToString() << 
      ", should be \"" << flagExpect << "\") \n";
  return 0;
}}

/*** --------------------------------------------------------------------------
   * function test_wraparound_behavior()
   * --------------------------------------------------------------------------
   * Description: runs some APInts through various exercises that should 
   *	induce wraparound, and prints out the answers.
   *
   * Method: 
   *
   * Reentrancy: 
   *
   * Inputs: 
   *    argc and argv: as passed to main(~)
   *
   * Outputs: none
   *
   * Return Value: a value recommended for the return value for main(~)
   *
   */
int testWrapBehavior( int argc, char* argv[] )
{{
  int result= 0;

  cout << "\n" << "test unsigned 16-bit wrap\n";
  result+= testAddWrapBehavior1Word( 
      16, 0xfff0, true, '+', 0x0020, true, "16=0x10", "001" );
  result+= testAddWrapBehavior1Word( 
      16, 0xfff0, false, '+', 0x0020, false, "16=0x10", "001" );

  cout << "\n" << "test signed 16-bit wrap\n";
  result+= testAddWrapBehavior1Word( 
      16, 0x7ff1, true, '+', 0x0031, true, "0x8022", "010" );
  result+= testAddWrapBehavior1Word( 
      16, 0x7ff1, false, '+', 0x0031, false, "0x8022", "010" );

  cout << "\n" << "test unsigned 64-bit wrap\n";
  result+= testAddWrapBehavior1Word( 
      64, 0xdd426c4dfd91efa1, true, '+', 0xab1b79340627716f, true, 
      "0x885DE58203B96110+carry", "011" );
  result+= testAddWrapBehavior1Word( 
      64, 0xdd426c4dfd91efa1, false, '+', 0xab1b79340627716f, false, 
      "0x885DE58203B96110+carry", "011" );


   
  return result;
}}

/*** --------------------------------------------------------------------------
   * function main()
   * --------------------------------------------------------------------------
   * Description: main function
   *
   * Method: 
   *
   * Reentrancy: 
   *
   * Inputs: 
   *  argc: the number of command line arguments
   *  argv[]: array of pointers to strings, each string holds a command 
   *	line argument
   *    
   * Outputs: none
   *
   * Return Value: 0 for success, otherwise for failure
   *
   */
int main( int argc, char* argv[] )
{{
  int argc2= argc- 2;
  char** argv2= &( argv[2] );
  return testWrapBehavior( argc2, argv2 );
}}


// template is 16 lines long
/*** --------------------------------------------------------------------------
   * function name()
   * --------------------------------------------------------------------------
   * Description: 
   *
   * Method: 
   *
   * Reentrancy: 
   *
   * Inputs: 
   *    
   * Outputs: 
   *
   * Return Value: 
   *
   */
//void name()



