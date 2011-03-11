      PROGRAM PRIMES6
*
*     Program to print the first N primes (N .LE. 1000000)
*
      IMPLICIT NONE
      INTEGER Primes(1000000)
      INTEGER Candidate, NumPrimes, Index, Maximum, I
      INTEGER NEXT
*
      Primes(1)=2
      WRITE(*,'(a$)')'Enter the number of primes to be found: '
      READ(*,*)NumPrimes
      Candidate = 1
      Index = 1
      Maximum = 4
      DO I = 2, NumPrimes
         Primes(I) = NEXT(Primes,Index,Maximum,Candidate)
      END DO
      WRITE(*,'(10I8)') (Primes(I),I = 1,NumPrimes)
*
      END
*
*
      INTEGER FUNCTION NEXT(Primes,Index,Maximum,Candidate)
*
*     Function to determine the next prime after the value of Candidate
*
      IMPLICIT NONE
      INTEGER Primes(*)
      INTEGER Candidate, Index, Maximum
      LOGICAL IsPrime, TEST
*
      IsPrime = .FALSE.
      DO WHILE (.NOT.IsPrime)       
         Candidate = Candidate+2
         IF (Candidate .GT. Maximum) THEN
            Index = Index+1
            Maximum = Primes(Index)*Primes(Index)
         ENDIF
         IsPrime = TEST(Candidate,Primes,Index)
      END DO
      NEXT = Candidate
      RETURN
*
      END
*
*
      LOGICAL FUNCTION TEST(Candidate,Primes,Index)
*
      IMPLICIT NONE
      INTEGER Primes(*)
      INTEGER Candidate, Index, I, Remainder
*
      TEST = .TRUE.
      DO I = 1, Index
         Remainder = Candidate-Candidate/Primes(I)*Primes(I)
         IF (Remainder .EQ. 0) TEST = .FALSE.
      END DO
      RETURN
*
      END
