  PROGRAM

  INCLUDE('regex.inc')

  MAP
    POSIX::Test()
    POSIX::MatchTest()
    PCRE::Test()
  END


  CODE
!  POSIX::Test()
!  POSIX::MatchTest()
  
  PCRE::Test()
  
POSIX::Test                   PROCEDURE()
regex                           TRegExPOSIX
reti                            LONG
pattern                         STRING(128)
str                             STRING(128)
  CODE
  
  ! Compile regular expression
  pattern = '^a[[:alnum:]]'
  IF regex.regcomp(pattern, REG_EXTENDED+REG_NEWLINE) <> REG_NOERROR
    MESSAGE('Could not compile regex')
    RETURN
  END
  
  ! Execute regular expression
  str = 'abc'
  reti = regex.regexec(str)
  IF NOT reti
    MESSAGE('Match')
  ELSIF reti = REG_NOMATCH
    MESSAGE('No match')
  ELSE
    MESSAGE('Regex match failed: '& regex.regerror(reti))
  END
  
  ! Free compiled regular expression if you want to use the regex_t again
  regex.regfree()

!POSIX::MatchTest              PROCEDURE()
!regex                           TRegEx
!reti                            LONG
!nomatch                         LONG
!pattern                         STRING(128)
!str                             STRING(128)
!strLen                          LONG, AUTO
!strNdx                          LONG, AUTO
!m                               LIKE(regmatch_t), DIM(10)
!n_matches                       LONG, AUTO
!i                               LONG, AUTO
!start                           LONG, AUTO
!finish                          LONG, AUTO
!  CODE
!  
!!  regex.SetSyntax(REG_EXTENDED+REG_NEWLINE)
!  
!  ! Compile regular expression
!  pattern = '([[:digit:]]+)[^[:digit:]]+([[:digit:]]+)'
!  reti = regex.RegComp(pattern, REG_EXTENDED+REG_NEWLINE)
!  IF reti
!    MESSAGE('Could not compile regex')
!    RETURN
!  END
!  
!  str = 'This 1 is nice 2 so 33 for 4254'
!  strLen = LEN(CLIP(str))
!  strNdx = 1
!  n_matches = MAXIMUM(m, 1)
!  
!  LOOP
!    IF strNdx > strLen
!      BREAK
!    END
!    
!    ! Execute regular expression
!    MESSAGE('Serach in '& str[strNdx : strLen])
!    nomatch = regex.RegExec(str[strNdx : strLen], n_matches, ADDRESS(m))
!    IF nomatch
!      MESSAGE('No more matches.')
!      RETURN
!    END
!
!    LOOP i = 1 TO n_matches
!      IF m[i].rm_so = -1
!        BREAK
!      END
!      
!      start = m[i].rm_so + 1  !strNdx   !(p - to_match);
!      finish = m[i].rm_eo + 1 !strNdx  !(p - to_match);
!      
!      MESSAGE('(finish - start)='& (finish - start) &'; S='& str[start : finish] &'; start:finish='& start &':'& finish)
!      
!      strNdx += m[1].rm_eo - 1
!    END
!  END
!  
!  ! Free compiled regular expression if you want to use the regex_t again
!  regex.RegFree()
POSIX::MatchTest              PROCEDURE()
regex                           TRegExPOSIX
pattern                         STRING(128)
str                             STRING(128)
matchIndex                      LONG, AUTO
  CODE
  
!  regex.SetSyntax(REG_EXTENDED+REG_NEWLINE)
  
  ! Compile regular expression
  pattern = '([[:digit:]]+)[^[:digit:]]+([[:digit:]]+)'
  IF regex.regcomp(pattern, REG_EXTENDED+REG_NEWLINE) <> REG_NOERROR
    MESSAGE('Could not compile regex')
    RETURN
  END
  
  str = 'This 1 is nice 2 so 33 for 4254'
  IF regex.regexec(str) = REG_NOERROR
    LOOP matchIndex = 1 TO regex.matchcount()
      MESSAGE(regex.matchitem(matchIndex))
    END
  END
  
  ! Free compiled regular expression if you want to use the regex_t again
  regex.regfree()
  
PCRE::Test                    PROCEDURE()
regex                           TRegExPCRE
errstr                          STRING(80)
ret                             LONG, AUTO
  CODE
!  errstr = regex.CompilePattern('^a[[:alnum:]]')
  errstr = regex.re_compile_pattern('a*')
!  errstr = regex.CompilePattern('{{4|four}th')
  IF errstr
    MESSAGE('Could not compile regex: '& CLIP(errstr))
    RETURN
  END
  
!  ret = regex.Match('abc', 0)
!  ret = regex.re_match('aaaaab', 2)
  ret = regex.re_match('aaaaab')
  IF ret >= 0
    MESSAGE('Match! ret '& ret)
  ELSIF ret = -1
    MESSAGE('No match')
  ELSE
    MESSAGE('Match failed')
  END
    
!  ret = regex.re_search('abc')
!!  ret = regex.re_search('^Th?om{{as|my}?{{ }+')
!  IF ret >= 0
!    MESSAGE('Search returned '& ret)
!  ELSIF ret = -1
!    MESSAGE('No match')
!  ELSE
!    MESSAGE('Search failed')
!  END
