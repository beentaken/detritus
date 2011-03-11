      PROGRAM PRIMES4
C
C     PROGRAM TO PRINT THE FIRST N PRIMES (N.LE.10000)
C
      DIMENSION IP(10000)
      READ(*,*)N
      I=1
      J=1
      IP(1)=2
 10   I=I+2
      IS=SQRT(FLOAT(I))
      K=0
 15   K=K+1
      IF (I/IP(K)*IP(K).EQ.I) GOTO 10
      IF (IP(K).LE.IS) GOTO 15
      J=J+1
      IP(J)=I
      IF (J.LT.N) GOTO 10
      WRITE (*,30) (IP(I),I=1,N)
 30   FORMAT(10I8)
      END
