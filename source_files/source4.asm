.sample program for SIC/XE architecture
.test program for proj1-4
.this file should assemble without errors
prog	start	0	.start of sample
bsrch	EQU 	14
OFFB	EQU 	8
TEST 	EQU 	TEST1
TEST1	EQU 	TEST2
TEST3	EQU 	TEST4
TEST4	EQU 	TEST5
		BASE	delta
		LDB 	#delta
.storage allocation section
test	byte	C'12345'
test2	byte	x'007b'

TEST5	word	00001
eta		BYTE	C'EOF'
	 	byte 	X'FFFE'
foo 	word	2000
theta	RESB	1000
delta	word 	2
		Byte	C'Out.		through. the .trees	and over the .fields.'
		end 	prog	.end of sample program
