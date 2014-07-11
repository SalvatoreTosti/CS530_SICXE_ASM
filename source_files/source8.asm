.Test file designed by Salvatore to run through all cases

prog	start	0		
		BASE	END
		add 	ALPHA
		+add	BETA
		addf	ALPHA
		+addf	BETA
		addr	A,X
		and 	ALPHA
		+and	BETA
		clear	A
		comp	ALPHA
		+comp	BETA
		compf	ALPHA
		+compf	BETA
		div 	ALPHA
		+div	BETA
		divf	ALPHA
		+divf	BETA
		divr	S,B
		fix 	
		float	
		hio 	
		j		ALPHA
		+j		BETA
		jeq 	ALPHA
		+jeq	BETA
		jgt 	ALPHA
		+jgt	BETA
		jlt 	ALPHA
		+jlt	BETA
		jsub	ALPHA
		+jsub	BETA
		lda 	ALPHA
		+lda	BETA
		ldb 	ALPHA
		+ldb	BETA
		ldch	ALPHA
		+ldch	BETA
		ldf 	ALPHA
		+ldf	BETA
		ldl 	ALPHA
		+ldl	BETA
		lds 	ALPHA
		+lds	BETA
		ldt 	ALPHA
		+ldt	BETA
		ldx 	ALPHA
		+ldx	BETA
		lps 	ALPHA
		+lps	BETA
		mul 	ALPHA
		+mul	BETA
		mulf	ALPHA
		+mulf	BETA
		mulr	A,X
		norm	
		or		ALPHA
		+or 	BETA
		rd		ALPHA
		+rd 	BETA
		rmo 	A,X
		rsub	
		shiftl	A,2
		shiftr	A,2
		sio 	
		ssk 	ALPHA
		+ssk	BETA
		sta 	ALPHA
		+sta	BETA
		stb 	ALPHA
		+stb	BETA
		stch	ALPHA
		+stch	BETA
		stf 	ALPHA
		+stf	BETA
		sti 	ALPHA
		+sti	BETA
		stl 	ALPHA
		+stl	BETA	
		sts 	ALPHA
		+sts	BETA
		stsw	ALPHA
		+stsw	BETA
		stt 	ALPHA
		+stt	BETA
		stx 	ALPHA
		+stx	BETA
		sub 	ALPHA
		+sub	BETA
		subf	ALPHA
		+subf	BETA
		subr	A,X
		svc 	4
		td		ALPHA
		+td 	BETA
		tio 	
		tix 	ALPHA
		+tix	BETA
		wd		ALPHA
END		+wd 	BETA
		
ALPHA	resw 100
BETA	word 1
DELTA	resw 50
		end 	prog
