      PROGRAM PRIMES1
C
C     PROGRAM TO PRINT THE FIRST N PRIMES
C
      READ(*,*)N
      I=1
      J=0
 10   I=I+1
      DO 20 K=2,I-1
      IF (I-I/K*K) 20,10,20
 20   CONTINUE
      WRITE(*,*)I
      J=J+1
      IF (J-N) 10,30,30
 30   CONTINUE
      END
