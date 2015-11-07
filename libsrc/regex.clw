  MEMBER

  INCLUDE('regex.inc')
  
TMatchItem                    GROUP, TYPE
Value                           &STRING
Start                           LONG
Finish                          LONG
                              END

TMatchItems                   QUEUE(TMatchItem), TYPE
                              END

  MAP
    MODULE('regex2.dll')

      ! Sets the current default syntax to SYNTAX, and return the old syntax.
      ! You can also simply assign to the `re_syntax_options' variable.
      re_set_syntax(reg_syntax_t syntax), reg_syntax_t, PROC, C, RAW, NAME('re_set_syntax')

      ! Compile the regular expression PATTERN, with length LENGTH
      ! and syntax given by the global `re_syntax_options', into the buffer
      ! BUFFER. Return NULL if successful, and an error string if not.
      re_compile_pattern(*STRING pattern, size_t length, *re_pattern_buffer buffer), *CSTRING, C, RAW, NAME('re_compile_pattern')

      ! Compile a fastmap for the compiled pattern in BUFFER; used to
      ! accelerate searches.  Return 0 if successful and -2 if was an
      ! internal error.
      re_compile_fastmap(*re_pattern_buffer buffer), LONG, C, RAW, NAME('re_compile_fastmap')

      ! Search in the string STRING (with length LENGTH) for the pattern
      ! compiled into BUFFER.  Start searching at position START, for RANGE
      ! characters. Return the starting position of the match, -1 for no
      ! match, or -2 for an internal error.  Also return register
      ! information in REGS (if REGS and BUFFER->no_sub are nonzero).
      re_search(*re_pattern_buffer buffer, *STRING string, LONG length, LONG start, LONG range, *re_registers regs), LONG, C, RAW, NAME('re_search')

      ! Like `re_search', but search in the concatenation of STRING1 and
      ! STRING2. Also, stop searching at index START + STOP. 
      re_search_2(*re_pattern_buffer buffer, *STRING string1, LONG length1, *STRING string2, LONG length2, LONG start, LONG range, *re_registers regs, LONG stop), LONG, C, RAW, NAME('re_search_2')

      ! Like `re_search', but return how many characters in STRING the regexp
      ! in BUFFER matched, starting at position START.
      re_match(*re_pattern_buffer buffer, *STRING string, LONG length, LONG start, *re_registers regs), LONG, C, RAW, NAME('re_match')

      ! Relates to `re_match' as `re_search_2' relates to `re_search'.
      re_match_2(*re_pattern_buffer buffer, *STRING string1, LONG length1, *STRING string2, LONG length2, LONG start, *re_registers regs, LONG stop), LONG, C, RAW, NAME('re_match_2')

      ! Set REGS to hold NUM_REGS registers, storing them in STARTS and
      ! ENDS. Subsequent matches using BUFFER and REGS will use this memory
      ! for recording register information.  STARTS and ENDS must be
      ! allocated with malloc, and must each be at least `NUM_REGS * sizeof
      ! (regoff_t)' bytes long.
      ! If NUM_REGS == 0, then subsequent matches should allocate their own
      ! register  data.
      ! Unless this function is called, the first search or match using
      ! PATTERN_BUFFER  will allocate its own register data, without
      ! freeing the old data.
      re_set_registers(*re_pattern_buffer buffer, *re_registers regs, UNSIGNED num_regs, LONG starts, LONG ends), C, RAW, NAME('re_set_registers')

      ! 4.2 bsd compatibility.
      re_comp(*CSTRING), *CSTRING, C, RAW, NAME('re_comp')
      re_exec(*CSTRING), LONG, C, RAW, NAME('re_exec')


      !POSIX compatibility
      regcomp(*regex_t preg, *CSTRING ppattern, LONG cflags), LONG, C, RAW, NAME('regcomp')
      regexec(*regex_t preg, *CSTRING pstr, size_t nmatch, LONG pmatch, LONG eflags), LONG, C, RAW, NAME('regexec')
      regerror(LONG perrcode, *regex_t preg, LONG perrbuf, size_t perrbuf_size), size_t, PROC, C, RAW, NAME('regerror')
      regfree(*regex_t preg), C, RAW, NAME('regfree')
    END

    GetMatchItem(TMatchItems matches, LONG pIndex), STRING, PRIVATE
    GetMatchPos(TMatchItems matches, LONG pIndex, *LONG pStart, *LONG pFinish), PRIVATE
    FreeMatches(*TMatchItems matches)
  END

!!!region private module procedures
GetMatchItem                  PROCEDURE(TMatchItems matches, LONG pIndex)
  CODE
  GET(matches, pIndex)
  ASSERT(ERRORCODE() = 0)
  IF NOT ERRORCODE()
    RETURN matches.Value
  END
  
  RETURN ''
  
GetMatchPos                   PROCEDURE(TMatchItems matches, LONG pIndex, *LONG pStart, *LONG pFinish)
  CODE
  GET(matches, pIndex)
  ASSERT(ERRORCODE() = 0)
  IF NOT ERRORCODE()
    pStart = matches.Start
    pFinish = matches.Finish
  ELSE
    pStart = 0
    pFinish = 0
  END

FreeMatches                   PROCEDURE(*TMatchItems matches)
qNdx                            LONG, AUTO
  CODE
  LOOP qNdx = 1 TO RECORDS(matches)
    GET(matches, qNdx)
    DISPOSE(matches.Value)
    matches.Value &= NULL
  END
  FREE(matches)
  
!!!endregion
  
!!!!region TRegEx
!TRegExBase.RegComp            PROCEDURE(STRING pattern, LONG cflags = 0)
!  CODE
!  ASSERT(FALSE, 'Applicable in derived class only.')
!  RETURN FALSE
!
!TRegExBase.RegError           PROCEDURE()
!  CODE
!  ASSERT(FALSE, 'Applicable in derived class only.')
!  RETURN ''
!  
!TRegExBase.RegFree            PROCEDURE()
!  CODE
!  regfree(SELF.regex)
!
!TRegExBase.MatchGroup         PROCEDURE(LONG pGrpNdx)
!  CODE
!  GET(SELF.groups, pGrpNdx)
!  IF NOT ERRORCODE()
!    RETURN SELF.groups.Grp
!  END
!  
!  RETURN ''
!
!TRegExBase.MatchBoundaries    PROCEDURE(LONG pGrpNdx, *LONG pStart, *LONG pFinish)
!  CODE
!  GET(SELF.groups, pGrpNdx)
!  IF NOT ERRORCODE()
!    pStart = SELF.groups.Start
!    pFinish = SELF.groups.Finish
!    RETURN
!  END
!  
!  pStart = 0
!  pFinish = 0
!
!!!!endregion
  
!!!region TRegExPOSIX
TRegExPOSIX.Construct         PROCEDURE()
  CODE
  SELF.matches &= NEW TMatchItems

TRegExPOSIX.Destruct          PROCEDURE()
  CODE
  regfree(SELF.regex)
  FreeMatches(SELF.matches)
  DISPOSE(SELF.matches)

TRegExPOSIX.regcomp           PROCEDURE(STRING pattern, LONG cflags = 0)
szpattern                       CSTRING(LEN(pattern) + 1)
  CODE
  szpattern = CLIP(pattern)
  RETURN regcomp(SELF.regex, szpattern, cflags)
  
TRegExPOSIX.regexec           PROCEDURE(STRING str, size_t nmatch, LONG pmatch, LONG eflags)
szstr                           CSTRING(LEN(str) + 1)
  CODE
  szstr = CLIP(str)
  RETURN regexec(SELF.regex, szstr, nmatch, pmatch, eflags)

TRegExPOSIX.regexec           PROCEDURE(STRING str, LONG eflags = 0)
len                             LONG, AUTO
ndx                             LONG, AUTO
m                               LIKE(regmatch_t), DIM(10)
n_matches                       LONG(10)  ! number of m[] elements
i                               LONG, AUTO
start                           LONG, AUTO
finish                          LONG, AUTO
nomatch                         LONG, AUTO
  CODE
  FreeMatches(SELF.matches)

  nomatch = SELF.regexec(str, n_matches, ADDRESS(m), eflags)
  IF nomatch
    RETURN nomatch
  END

  len = LEN(CLIP(str))
  ndx = 1

  LOOP
    LOOP i = 1 TO n_matches
      IF m[i].rm_so = -1
        BREAK
      END
      
      start = m[i].rm_so + 1
      finish = m[i].rm_eo + 1
      
      SELF.matches.Value &= NEW STRING(finish - start + 1)
      SELF.matches.Value = str[start : finish]
      SELF.matches.Start = start
      SELF.matches.Finish = finish
      ADD(SELF.matches)
      
      ndx += m[1].rm_eo - 1
    END
    
    IF ndx > len
      BREAK
    END
    
    nomatch = SELF.regexec(str[ndx : len], n_matches, ADDRESS(m), eflags)
    IF nomatch
      BREAK
    END

  END
  
  RETURN 0
                                
TRegExPOSIX.regerror          PROCEDURE(LONG errcode)
msgbuf                          STRING(80)
  CODE
  regerror(errcode, SELF.regex, ADDRESS(msgbuf), SIZE(msgbuf))
  RETURN CLIP(msgbuf)
    
TRegExPOSIX.regfree           PROCEDURE()
  CODE
  regfree(SELF.regex)

TRegExPOSIX.matchcount        PROCEDURE()
  CODE
  RETURN RECORDS(SELF.matches)
  
TRegExPOSIX.matchitem         PROCEDURE(LONG pIndex)
  CODE
  RETURN GetMatchItem(SELF.matches, pIndex)

TRegExPOSIX.matchpos          PROCEDURE(LONG pIndex, *LONG pStart, *LONG pFinish)
  CODE
  GetMatchPos(SELF.matches, pIndex, pStart, pFinish)

!!!endregion
  
!!!region TRegExPCRE
TRegExPCRE.Construct          PROCEDURE()
  CODE
  ! If non-zero, applies a translation function to characters before 
  ! attempting match (http://www.delorie.com/gnu/docs/regex/regex_51.html)  
  SELF.regex.translate = 0
  ! If non-zero, optimization technique.  Don't know details.
  ! See http://www.delorie.com/gnu/docs/regex/regex_45.html
  SELF.regex.fastmap = 0
  ! Next two must be set to 0 to request library allocate memory
  SELF.regex.buffer = 0
  SELF.regex.allocated = 0
  ! This is a global (!) used to set the regex type (note POSIX APIs don't use global for this)
  re_set_syntax(RE_SYNTAX_EGREP)
  
  SELF.matches &= NEW TMatchItems

TRegExPCRE.Destruct           PROCEDURE()
  CODE
  regfree(SELF.regex)
  FreeMatches(SELF.matches)
  DISPOSE(SELF.matches)
  
TRegExPCRE.re_set_syntax      PROCEDURE(reg_syntax_t syntax)
  CODE
  RETURN re_set_syntax(syntax)

TRegExPCRE.re_compile_pattern PROCEDURE(STRING pattern)
  CODE
  RETURN re_compile_pattern(pattern, LEN(pattern), SELF.regex)
  
TRegExPCRE.re_search          PROCEDURE(STRING str, LONG start = 0, LONG range = 0)
regs                            &re_registers
  CODE
  regs &= NULL
  RETURN re_search(SELF.regex, str, LEN(str), start, range, regs)

TRegExPCRE.re_match           PROCEDURE(STRING str, LONG start = 0)
regs                            &re_registers
  CODE
  regs &= NULL
  RETURN re_match(SELF.regex, str, LEN(str), start, regs)
    
TRegExPCRE.regfree            PROCEDURE()
  CODE
  regfree(SELF.regex)

TRegExPCRE.matchcount         PROCEDURE()
  CODE
  RETURN RECORDS(SELF.matches)
  
TRegExPCRE.matchitem          PROCEDURE(LONG pIndex)
  CODE
  RETURN GetMatchItem(SELF.matches, pIndex)

TRegExPCRE.matchpos           PROCEDURE(LONG pIndex, *LONG pStart, *LONG pFinish)
  CODE
  GetMatchPos(SELF.matches, pIndex, pStart, pFinish)

!endregion
  
