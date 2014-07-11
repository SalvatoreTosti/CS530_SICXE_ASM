.Test source file for all combinations of label, opcode, operand and comment.
	Start	$1000
tst	LDa 	ALPHA
	BASE	ALPHA
	SVC 	3
	LDA 	$500
	lda 	$100,x
	lDa 	@1500
	LdA 	#1000
	+LDA	#50 
	LDA 	ALPHA
	LDA 	BETA
	+LDA	ALPHA
	LDA 	ALPHA,X
	LDA 	BETA,X
	+LDA	ALPHA,x
	LDA 	@ALPHA
	LDA 	@BETA
	+lDA	@ALPHA
	LDA 	#ALPHA
	LDA 	#BETA
	+LDA 	#ALPHA
	
ALPHA	Resw 	750
BETA	rESW	2
