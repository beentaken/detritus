      PROGRAM PRIMES
*
*     Program to print the first N primes (N .LE. 10000)
*
      INTEGER P(10000)
      INTEGER CAND
*
      P(1)=2
      NP=1
      READ(*,*)N
      CAND=1
      J=1
      MAX=4
      DO I = 2, N
         P(I)=NEXT(P,J,MAX,CAND)
      END DO
      WRITE(*,'(10I8)') (P(I),I=1,N)
      END
*
*
      FUNCTION NEXT(P,J,MAX,CAND)
*
      INTEGER P(*)
      INTEGER CAND
      LOGICAL PRIME, TEST
*
      PRIME=.FALSE.
      DO WHILE (.NOT. PRIME)       
         CAND=CAND+2
         IF (CAND .GT. MAX) THEN
            J=J+1
            MAX=P(J)*P(J)
         ENDIF
         PRIME=TEST(CAND,P,J)
      END DO
      NEXT=CAND
      RETURN
      END
*
*
      LOGICAL FUNCTION TEST(CAND,P,J)
*
      INTEGER P(*)
      INTEGER CAND
*
      TEST=.TRUE.
      DO I = 1, J
         IF (CAND/P(I)*P(I) .EQ. CAND) TEST =.FALSE.
      END DO
      RETURN
      END
