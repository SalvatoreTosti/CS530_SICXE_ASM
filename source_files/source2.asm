.sample program for SIC/XE architecture
.
prog    start   0    .start of sample program
        lds     #3
        ldt     #300
        ldx     #0
addlf   lda     alpha,x   .loop control     xxxxxxx
        add     beta,x
        sta     gamma,x   qqqqqqqqqqqqqqq     .bad line here                
        addr    s,x
        compr   x,t 
        jlt     addlf   
        .
. storage allocation section        
alpha   resw    100
beta    resw    100
gamma   resw    100   
        end     prog    .end of sample program     
