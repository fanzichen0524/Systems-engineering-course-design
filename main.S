/*
 * main.S for MIPSfpga running on Nexys4-DDR and DE2-115
 * FPGA target boards
 *
 * Description:
 * Reads the pushbuttons and switches on the FPGA board and writes 
 * the values to the green and red LEDs. This is the Switches&LEDs
 * program from the MIPSfpga Getting Started Guide. 
 *
 * Note: Because the Nexys4-DDR board only has one set of LEDs, the
 * pushbutton values will not be shown on that board. Only the switch 
 * values will output to the LEDs on that board.
 */

# $10 = sw, $11 = pb
.globl main
  
main:
   lui   $8, 0xb060 #LED
   lui   $9, 0xb0c0  #switch

   addiu $12, $8, 4     # $12 = LEDG address offset 
   addiu $13, $9, 4     # $13 = SW address offset

   lui $14,0x0000
   
   lui $15,0xffff
   ori $15,$15,0xffff

   sw $14,0($12)	#output
   sw $15,0($13)    #input
   
	
readIO: 	
   lw    $10, 0($9)    # read switches: sw = SW values
   sw    $10, 0($8)     # write switch values to red LEDs 
   beq   $0, $0, readIO # repeat
   nop                  # branch delay slot
