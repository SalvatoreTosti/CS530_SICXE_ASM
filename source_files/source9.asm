.Test file designed by Salvatore to run through bad cases

prog	start	0
		BASE	END
		ADDR	A,X
		COMPR	A,X
		DIVR	A,X
		MULR	A,X
		RMO 	A,X
		SHIFTL	A,1
		SHIFTR	A,1
		CLEAR	A
		TIXR	A
		SVC 	3
		FIX 	   
		FLOAT
		HIO
		NORM
		SIO
END		TIO
		
		
		.size 2s
		.ADDR	r1,r2
		.COMPR	r1,r2
		.DIVR	r1,r2
		.MULR	r1,r2
		.RMO 	r1,r2
		.SHIFTL	r1,n
		.SHIFTR	r1,n
		
		.CLEAR	r1	//handled
		.TIXR	r1	//handled
		
		.SVC 	n	//handled
		
		.size 1s
		.FIX
		.FLOAT
		.HIO
		.NORM
		.SIO
		.TIO
		
		
		
		
ALPHA	resw 100
BETA	word 1
DELTA	resw 50
		end 	prog
