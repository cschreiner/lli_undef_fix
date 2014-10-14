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
#include <stdlib.h>
#include "llvm/ADT/APInt.h"

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
void test_wraparound_behavior( int argc, char* argv[] )
{{
    APInt aa( 16, 0xfff0, true );
    APInt bb( 16, 0xff20, true );
    APInt cc= aa + bb;
    printf ( "16 bits: \"%s\" + \"%s\" = \"%s\" (should be 0x0010) \n", 
	     aa.toString(), bb.toString(), cc.toString() );
    return 0;
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
  return test_wraparound_behavior( argc2, argv2 );
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


