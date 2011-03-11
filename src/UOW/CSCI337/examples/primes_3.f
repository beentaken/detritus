      PROGRAM PRIMES3
C
C     PROGRAM TO PRINT THE FIRST N PRIMES
C
      READ(*,*)N
      I=1
      J=0
 10   I=I+1
      IS = SQRT(FLOAT(I))
      DO 20 K=2,IS
      IF (I/K*K .EQ.I) GOTO 10
 20   CONTINUE
      WRITE(*,30)I
 30   FORMAT(I8,$)
      J=J+1
      IF (J/10*10 .EQ.J) WRITE (*,40)
 40   FORMAT()
      IF (J.LT.N) GOTO 10
      IF (J/10*10.NE.J) WRITE (*,40)
      END
