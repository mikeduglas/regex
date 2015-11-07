  INCLUDE('regextypes.inc')


!POSIX Regular Expressions
TRegExPOSIX                   CLASS, TYPE, MODULE('regex.clw'), LINK('regex.clw')
regex                           LIKE(regex_t), PROTECTED
matches                         &TMatchItems, PROTECTED

Construct                       PROCEDURE()
Destruct                        PROCEDURE(), VIRTUAL

!private/protected methods
regexec                         PROCEDURE(STRING str, size_t nmatch, LONG pmatch, LONG eflags), LONG, PRIVATE

!public methods
!!!<summary>
!!!If `regcomp' successfully compiles the regular expression, it returns zero
!!!If `regcomp' can't compile the regular expression, it returns one of
!!!the error codes (see reg_errcode_t itemize)
!!!</summary>
regcomp                         PROCEDURE(STRING pattern, LONG cflags = 0), LONG  !cflags: REG_EXTENDED|REG_ICASE|REG_NOSUB|REG_NEWLINE
regexec                         PROCEDURE(STRING str, LONG eflags = 0), LONG  !returns zero if the compiled pattern matches STRING and `REG_NOMATCH' if it doesn't ;  eflags: REG_NOTBOL|REG_NOTEOL
regerror                        PROCEDURE(LONG perrcode), STRING
regfree                         PROCEDURE()

matchcount                      PROCEDURE(), LONG
matchitem                       PROCEDURE(LONG pIndex), STRING
matchpos                        PROCEDURE(LONG pIndex, *LONG pStart, *LONG pFinish)
                              END

!Perl Compatible Regular Expressions
TRegExPCRE                    CLASS, TYPE, MODULE('regex.clw'), LINK('regex.clw')
regex                           LIKE(re_pattern_buffer), PROTECTED
matches                         &TMatchItems, PROTECTED

Construct                       PROCEDURE()
Destruct                        PROCEDURE(), VIRTUAL

re_set_syntax                   PROCEDURE(reg_syntax_t syntax), reg_syntax_t, PROC

!!!<summary>
!!!Compile the regular expression PATTERN,
!!!and syntax given by the global `re_syntax_options'.
!!!Return '' if successful, and an error string if not.
!!!</summary>
re_compile_pattern              PROCEDURE(STRING pattern), STRING, PROC

!!!<summary>
!!!Search in the string for the compiled pattern.
!!!Start searching at position START, for RANGE
!!!characters. Return the starting position of the match, -1 for no
!!!match, or -2 for an internal error.  Also return register
!!!information in REGS (if REGS and BUFFER->no_sub are nonzero).
!!!</summary>
re_search                       PROCEDURE(STRING str, LONG start = 0, LONG range = 0), LONG

!!!<summary>
!!!Returns how many characters in STRING matched, starting at position START.
!!!</summary>
!!!<returns>
!!!number of characters matches (may be 0, but if so there's still a match),
!!!-1 No match
!!!Anything else (though docs say -2 is internal library error)
!!!</returns>
re_match                        PROCEDURE(STRING str, LONG start = 0), LONG

regfree                         PROCEDURE()

matchcount                      PROCEDURE(), LONG
matchitem                       PROCEDURE(LONG pIndex), STRING
matchpos                        PROCEDURE(LONG pIndex, *LONG pStart, *LONG pFinish)
                              END
