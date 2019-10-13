# Test file for Lab2_introToAVR


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

# Example test: test sequence from A0 !A0 A0
# Add tests below
test "PINA:0x02 => PORTB: 0x07"
set C_state = C_SMStart
setPINA 0x02
continue 2
expectPORTB 0x07
checkResult

test "PINA:0x03  => PORTB: 0x00, C_state = C_reset"
set C_state = C_initial
setPINA 0x03 
continue 2
expectPORTB 0x00
checkResult


test "PINA:0x03, 0x01 => PORTB: 0x01, C_state = C_wait1"
set C_state = C_initial
setPINA 0x03
continue 2
setPINA 0x01
continue 2
expectPORTB 0x01
checkResult


test "PINA:0x02 => PORTB: 0x06, C_state = C_wait"
set C_state = C_initial
setPINA 0x02
continue 2
checkResult
expectPORTB 0x06


test "PINA:0x02 0x03=> PORTB: 0x00, C_state = C_reset"
set C_state = C_initial
setPINA 0x02
continue 2
setPINA 0x03
continue 2
expectPORTB 0x00
checkResult

test "PINA:0x09 => PORTB: 0x06, C_state = C_wait1"
set C_state = C_initial
setPINA 0x09
continue 2
expectPORTB 0x06
checkResult

test "PINA:0x02, 0x00, 0x01 => PORTB: 0x07, C_state = C_wait"
set C_state = C_initial
setPINA 0x02
continue 2
setPINA 0x00
continue 2
expectPORTB 0x07
checkResult
# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
