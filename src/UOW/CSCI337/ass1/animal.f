*
*     $Id: animal.f 2 2007-07-19 13:00:48Z pdezwart $
*
      BLOCK DATA
*
      IMPLICIT NONE
      CHARACTER stuff(1024) * 64
      INTEGER links(1024, 2), floater
      COMMON /stuff/ stuff
      COMMON /links/ links, floater
*
      DATA stuff, links, floater /1024 * ' ', 2048 * 0, 0/
*
      END
*
      PROGRAM ANIMAL
*
*     Program that attempts to guess what animal a person is thinking of
*     using a naieve decision tree.
*
      IMPLICIT NONE
      LOGICAL playing, ingame, YORN, GAME
      INTEGER tmp, NEWSTUFF
*
      playing = .TRUE.
      tmp = NEWSTUFF('horse')
      CALL LOADSAVE(0)
      DO WHILE (playing .EQV. .TRUE.)
         ingame = .TRUE.
	 WRITE(*, '(a$)') 'Think of an animal (press Enter to continue)'
	 READ(*, *)
	 DO WHILE (ingame .EQV. .TRUE.)
	    ingame = GAME()
	 END DO
	 WRITE(*, '(a$)') 'Thank you. Play again? '
	 playing = YORN()
      END DO
      CALL LOADSAVE(1)
*
      END
*
      SUBROUTINE LOADSAVE(op)
*
      IMPLICIT NONE
      CHARACTER file * 64
      CHARACTER opname(2)*6
      CHARACTER stuff(1024) * 64
      INTEGER i, num, op, STRLEN
      INTEGER links(1024, 2), floater
      COMMON /stuff/ stuff
      COMMON /links/ links, floater
*
      num = 0
      DATA opname /'loaded','saved'/
      WRITE(*, '(a$)') 'Enter saved game file (blank for no file): '
      READ(*, '(a64)') file
      IF (STRLEN(file) .GT. 0) THEN
         OPEN(UNIT=42, FILE=file, STATUS='UNKNOWN', ACCESS='DIRECT',
     C      FORM='UNFORMATTED', RECL=72)
	    IF (op .EQ. 0) THEN
	       READ(UNIT=42, REC=1) floater
	    ELSE
	       WRITE(UNIT=42, REC=1) floater
	    END IF
            DO i = 1, floater
	       IF (op .EQ. 0) THEN
                  READ(UNIT=42, REC=i+1) stuff(i), links(i,1),
     C               links(i,2)
	       ELSE
                  WRITE(UNIT=42, REC=i+1) stuff(i), links(i,1),
     C               links(i,2)
	       END IF
	       IF (links(i,1) .EQ. 0) THEN
		  num = num + 1
	       END IF
            END DO
	 CLOSE(UNIT=42)
         WRITE(*, '(a, i4, 3(a))') '(',num,' animals ',opname(op+1),')'
      END IF
*
      END
*
      LOGICAL FUNCTION GAME()
*
*     Play one iteration of the game
*
      IMPLICIT NONE
      INTEGER index, old, new, STRLEN, NEWSTUFF
      LOGICAL YORN
      CHARACTER stuff(1024) * 64
      INTEGER links(1024, 2), floater
      COMMON /stuff/ stuff
      COMMON /links/ links, floater
*
      index = 1
      GAME = .TRUE.
      DO WHILE (GAME .EQV. .TRUE.)
         IF (links(index, 1) .EQ. 0) THEN
            CALL ASKA(index)
	    IF (YORN() .EQV. .FALSE.) THEN
	       old = NEWSTUFF(stuff(index))
	       new = NEWSTUFF(' ')
	       CALL WHAT(new)
	       CALL GUISH(index, stuff(old), stuff(new))
	       WRITE(*, '(a$)') 'For a'
	       CALL HANDLEV(stuff(new))
	       WRITE(*, '(3(a$))') ' ', stuff(new)(1:STRLEN(stuff(new))),
     C            ' the answer would be? '
	       IF (YORN() .EQV. .TRUE.) THEN
		  links(index, 1) = new
		  links(index, 2) = old
	       ELSE
		  links(index, 1) = old
		  links(index, 2) = new
               END IF
	    ELSE
	       WRITE(*, '(a)') 'Excellent, I win!'
	    END IF
	    GAME = .FALSE.
         ELSE
	    WRITE(*, '(2(a$))') stuff(index)(1:STRLEN(stuff(index))), '? '
	    IF (YORN() .EQV. .TRUE.) THEN
	       index = links(index, 1)
	    ELSE
	       index = links(index, 2)
	    END IF
         END IF
      END DO
      GAME = .FALSE.
      RETURN
      END
*
      SUBROUTINE LOWER(string)
*
*     Turn an entire string in to lower case
*
      IMPLICIT NONE
      CHARACTER string * (*)
      INTEGER i
*
      DO i = 1, LEN(string)
	 IF (string(i:i) .NE. ' ' .AND. ICHAR(string(i:i)) .GE.
     C      ICHAR('A') .AND. ICHAR(string(i:i)) .LE. ICHAR('Z')) THEN
	    string(i:i) = CHAR(ICHAR(string(i:i)) + ICHAR(' '))
	 ENDIF 
      END DO
*
      END
*
      SUBROUTINE GUISH(index, old, new)
*
      IMPLICIT NONE
      INTEGER index, last, STRLEN
      CHARACTER stuff(1024) * 64, c, old*(*), new*(*)
      INTEGER links(1024, 2), floater
      COMMON /stuff/ stuff
      COMMON /links/ links, floater
*
      WRITE(*, '(a$)') 'What question would distinguish a'
      CALL HANDLEV(old(1:1))
      WRITE(*, '(3(a$))') ' ', old(1:STRLEN(old)), ' from a'
      CALL HANDLEV(new(1:1))
      WRITE(*, '(3(a$))') ' ', new(1:STRLEN(new)), '?\n'
      READ(*, '(a64)') stuff(index)
      CALL LOWER(stuff(index))
      stuff(index)(1:1) = CHAR(ICHAR(stuff(index)(1:1)) - ICHAR(' '))
      last = STRLEN(stuff(index))
      c = stuff(index)(last:last)
      IF (c .LT. 'a' .OR. c .GT. 'z') THEN
	 stuff(index)(last:last) = ' '
      END IF
*
      END
*
      SUBROUTINE WHAT(index)
*
      IMPLICIT NONE
      INTEGER index, i
      CHARACTER stuff(1024) * 64
      INTEGER links(1024, 2), floater
      COMMON /stuff/ stuff
      COMMON /links/ links, floater
*
      WRITE(*, '(a$)') 'What is your animal? '
      READ(*, '(a64)') stuff(index)
      CALL LOWER(stuff(index))
      IF (stuff(index)(1:1) .EQ. 'a') THEN
	 i = 1
	 DO WHILE (stuff(index)(i:i) .NE. ' ')
	    i = i + 1
	 END DO
	 IF (stuff(index)(i + 1:i + 1) .NE. ' ') THEN
	    stuff(index) = stuff(index)(i + 1:64)
	 END IF
      END IF
*
      END
*
      SUBROUTINE ASKA(index)
*
      IMPLICIT NONE
      INTEGER index, STRLEN
      CHARACTER stuff(1024) * 64
      INTEGER links(1024, 2), floater
      COMMON /stuff/ stuff
      COMMON /links/ links, floater
*
      WRITE(*, '(a$)') 'Is your animal a'
      CALL HANDLEV(stuff(index))
      WRITE(*, '(3(a$))') ' ', stuff(index)(1:STRLEN(stuff(index))),
     C   '? '
*
      END
*
      INTEGER FUNCTION NEWSTUFF(contents)
*
      IMPLICIT NONE
      CHARACTER contents * (*)
      CHARACTER stuff(1024) * 64
      INTEGER links(1024, 2), floater
      COMMON /stuff/ stuff
      COMMON /links/ links, floater
*
      floater = floater + 1
      stuff(floater) = contents
      NEWSTUFF = floater
      RETURN
*
      END
*
      LOGICAL FUNCTION YORN()
*
*     Yes or No?
*
      IMPLICIT NONE
      CHARACTER c
      LOGICAL sequitiur
*
      sequitiur = .FALSE.
      DO WHILE (sequitiur .EQV. .FALSE.)
         READ(*, *) c
         IF (c .EQ. 'Y' .OR. c .EQ. 'y') THEN
            YORN = .TRUE.
            sequitiur = .TRUE.
         ELSE IF (c .EQ. 'N' .OR. c .EQ. 'n') THEN
	    YORN = .FALSE.
            sequitiur = .TRUE.
         ELSE
            WRITE(*, '(a$)') 'Yes or no: '
            END IF
      END DO
      RETURN
*
      END
*
      INTEGER FUNCTION STRLEN(string)
*
      IMPLICIT NONE
      CHARACTER string*(*)
      INTEGER i
*
      DO 10 i = LEN(string), 1, -1
	 IF(string(i:i) .NE. ' ') GOTO 15
10    CONTINUE
15    STRLEN = i
      RETURN
*
      END
*
      SUBROUTINE HANDLEV(c)
*
      CHARACTER c
*
      IF (c .EQ. 'a' .OR. c .EQ. 'e' .OR. c .EQ. 'i' .OR. c .EQ. 'o'
     C   .OR. c .EQ. 'u') THEN
	 WRITE(*, '(a$)') 'n'
      END IF
*
      END
*
