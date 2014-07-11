.sample program for SIC/XE architecture
.
.This program should assemble correctly
.

prog    start   $1000 		   .start of sample program
        lds     #3
        ldt     #300	
        ldx     #0
		lda 	#alpha
		lda 	@alpha
		lda 	alpha
addlf   lda     alpha,x   .loop control     
        add     beta,x
        sta     gamma,x  
		byte	C' 33  .fjdklas yep'
        addr    s,x
        compr   x,t 
        jlt     addlf   
        .
. storage allocation section
delta	resb	$10        
alpha   resw    100
beta    resw    100
gamma   resw    100   
        end     prog    .end of sample program     
.		lds     #3
