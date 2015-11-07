! /* The following bits are used to determine the regexp syntax we
! recognize. The set/not-set meanings are chosen so that Emacs syntax
! remains the value 0.  The bits are given in alphabetical order, and
! the definitions shifted by one from the previous bit; thus, when we
! add or remove a bit, only one other definition need change.  */
reg_syntax_t                  EQUATE(ULONG)

! /* If this bit is not set, then \ inside a bracket expression is literal.
! If set, then such a \ quotes the following character.  */
RE_BACKSLASH_ESCAPE_IN_LISTS  EQUATE(00000000000000000000000000000001b) 

! /* If this bit is not set, then + and ? are operators, and \+ and \? are
! literals.
! If set, then \+ and \? are operators and + and ? are literals.  */
RE_BK_PLUS_QM                 EQUATE(00000000000000000000000000000010b) !(RE_BACKSLASH_ESCAPE_IN_LISTS << 1) 

! /* If this bit is set, then character classes are supported.  They are:
! [:alpha:], [:upper:], [:lower:],  [:digit:], [:alnum:], [:xdigit:],
! [:space:], [:print:], [:punct:], [:graph:], and [:cntrl:].
! If not set, then character classes are not supported.  */
RE_CHAR_CLASSES               EQUATE(00000000000000000000000000000100b) !(RE_BK_PLUS_QM << 1) 

! /* If this bit is set, then ^ and $ are always anchors (outside bracket
! expressions, of course).
! If this bit is not set, then it depends:
!  ^  is an anchor if it is at the beginning of a regular
! expression  or after an open-group or an alternation operator;
!  $  is an anchor if it is at the end of a regular expression, or
! before  a close-group or an alternation operator.
! This bit could be (re)combined with RE_CONTEXT_INDEP_OPS, because
! POSIX draft 11.2 says that * etc. in leading positions is undefined.
! We already implemented a previous draft which made those constructs
! invalid, though, so we haven't changed the code back.  */
RE_CONTEXT_INDEP_ANCHORS      EQUATE(00000000000000000000000000001000b) !(RE_CHAR_CLASSES << 1)
 
! /* If this bit is set, then special characters are always special
! regardless of where they are in the pattern.
! If this bit is not set, then special characters are special only in
! some contexts; otherwise they are ordinary.  Specifically,
! * + ? and intervals are only special when not after the beginning,
! open-group, or alternation operator.  */
RE_CONTEXT_INDEP_OPS          EQUATE(00000000000000000000000000010000b) !(RE_CONTEXT_INDEP_ANCHORS << 1) 

! /* If this bit is set, then *, +, ?, and { cannot be first in an re or
! immediately after an alternation or begin-group operator.  */
RE_CONTEXT_INVALID_OPS        EQUATE(00000000000000000000000000100000b) !(RE_CONTEXT_INDEP_OPS << 1) 

! /* If this bit is set, then . matches newline.
! If not set, then it doesn't.  */
RE_DOT_NEWLINE                EQUATE(00000000000000000000000001000000b) !(RE_CONTEXT_INVALID_OPS << 1) 

! /* If this bit is set, then . doesn't match NUL.
! If not set, then it does.  */
RE_DOT_NOT_NULL               EQUATE(00000000000000000000000010000000b) !(RE_DOT_NEWLINE << 1) 

! /* If this bit is set, nonmatching lists [^...] do not match newline.
! If not set, they do.  */
RE_HAT_LISTS_NOT_NEWLINE      EQUATE(00000000000000000000000100000000b) !(RE_DOT_NOT_NULL << 1)

! /* If this bit is set, either \{...\} or {...} defines an
! interval, depending on RE_NO_BK_BRACES.
! If not set, \{, \}, {, and } are literals.  */
RE_INTERVALS                  EQUATE(00000000000000000000001000000000b) !(RE_HAT_LISTS_NOT_NEWLINE << 1) 

! /* If this bit is set, +, ? and | aren't recognized as operators.
! If not set, they are.  */
RE_LIMITED_OPS                EQUATE(00000000000000000000010000000000b) !(RE_INTERVALS << 1) 

! /* If this bit is set, newline is an alternation operator.
! If not set, newline is literal.  */
RE_NEWLINE_ALT                EQUATE(00000000000000000000100000000000b) !(RE_LIMITED_OPS << 1) 

! /* If this bit is set, then `{...}' defines an interval, and \{ and \}
! are literals.
! If not set, then `\{...\}' defines an interval.  */
RE_NO_BK_BRACES               EQUATE(00000000000000000001000000000000b) !(RE_NEWLINE_ALT << 1) 

! /* If this bit is set, (...) defines a group, and \( and \) are literals.
! If not set, \(...\) defines a group, and ( and ) are literals.  */
RE_NO_BK_PARENS               EQUATE(00000000000000000010000000000000b) !(RE_NO_BK_BRACES << 1) 

! /* If this bit is set, then \<digit> matches <digit>.
! If not set, then \<digit> is a back-reference.  */
RE_NO_BK_REFS                 EQUATE(00000000000000000100000000000000b) !(RE_NO_BK_PARENS << 1)
 
! /* If this bit is set, then | is an alternation operator, and \| is literal.
! If not set, then \| is an alternation operator, and | is literal.  */
RE_NO_BK_VBAR                 EQUATE(00000000000000001000000000000000b) !(RE_NO_BK_REFS << 1) 

! /* If this bit is set, then an ending range point collating higher
! than the starting range point, as in [z-a], is invalid.
! If not set, then when ending range point collates higher than the
! starting range point, the range is ignored.  */
RE_NO_EMPTY_RANGES            EQUATE(00000000000000010000000000000000b) !(RE_NO_BK_VBAR << 1) 

! /* If this bit is set, then an unmatched ) is ordinary.
! If not set, then an unmatched ) is invalid.  */
RE_UNMATCHED_RIGHT_PAREN_ORD  EQUATE(00000000000000100000000000000000b) !(RE_NO_EMPTY_RANGES << 1) 

! /* If this bit is set, succeed as soon as we match the whole pattern,
! without further backtracking.  */
RE_NO_POSIX_BACKTRACKING      EQUATE(00000000000001000000000000000000b) !(RE_UNMATCHED_RIGHT_PAREN_ORD << 1) 

! /* If this bit is set, do not process the GNU regex operators.
! If not set, then the GNU regex operators are recognized. */
RE_NO_GNU_OPS                 EQUATE(00000000000010000000000000000000b)!(RE_NO_POSIX_BACKTRACKING << 1) 

! /* If this bit is set, turn on internal regex debugging.
! If not set, and debugging was on, turn it off.
! This only works if regex.c is compiled -DDEBUG.
! We define this bit always, so that all that's needed to turn on
! debugging is to recompile regex.c; the calling code can always have
! this bit set, and it won't affect anything in the normal case. */
RE_DEBUG                      EQUATE(00000000000100000000000000000000b) !(RE_NO_GNU_OPS << 1) 

! /* If this bit is set, a syntactically invalid interval is treated as
! a string of ordinary characters.  For example, the ERE 'a{1' is
! treated as 'a\{1'.  */
RE_INVALID_INTERVAL_ORD       EQUATE(00000000001000000000000000000000b) !(RE_DEBUG << 1) 

! /* If this bit is set, then ignore case when matching.
! If not set, then case is significant.  */
RE_ICASE                      EQUATE(00000000010000000000000000000000b) !(RE_INVALID_INTERVAL_ORD << 1) 

! /* This bit is used internally like RE_CONTEXT_INDEP_ANCHORS but only
! for ^, because it is difficult to scan the regex backwards to find
! whether ^ should be special.  */
RE_CARET_ANCHORS_HERE         EQUATE(00000000100000000000000000000000b) !(RE_ICASE << 1) 

! /* If this bit is set, then \{ cannot be first in an bre or
! immediately after an alternation or begin-group operator.  */
RE_CONTEXT_INVALID_DUP        EQUATE(00000001000000000000000000000000b) !(RE_CARET_ANCHORS_HERE << 1) 

! /* If this bit is set, then no_sub will be set to 1 during
! re_compile_pattern.  */
RE_NO_SUB                     EQUATE(00000010000000000000000000000000b) !(RE_CONTEXT_INVALID_DUP << 1) 

    
! Define combinations of the above bits for the standard possibilities.
RE_SYNTAX_EMACS               EQUATE(0)
RE_SYNTAX_AWK                 EQUATE(RE_BACKSLASH_ESCAPE_IN_LISTS + RE_DOT_NOT_NULL |
                                + RE_NO_BK_PARENS + RE_NO_BK_REFS                   |
                                + RE_NO_BK_VBAR + RE_NO_EMPTY_RANGES                |
                                + RE_DOT_NEWLINE + RE_CONTEXT_INDEP_ANCHORS         |
                                + RE_UNMATCHED_RIGHT_PAREN_ORD + RE_NO_GNU_OPS)
RE_SYNTAX_GREP                EQUATE(RE_BK_PLUS_QM + RE_CHAR_CLASSES                |
                                + RE_HAT_LISTS_NOT_NEWLINE + RE_INTERVALS           |
                                + RE_NEWLINE_ALT)
RE_SYNTAX_EGREP               EQUATE(RE_CHAR_CLASSES + RE_CONTEXT_INDEP_ANCHORS     |
                                + RE_CONTEXT_INDEP_OPS + RE_HAT_LISTS_NOT_NEWLINE   |
                                + RE_NEWLINE_ALT + RE_NO_BK_PARENS + RE_NO_BK_VBAR)
RE_SYNTAX_POSIX_EGREP         EQUATE(RE_SYNTAX_EGREP + RE_INTERVALS                 |
                                + RE_NO_BK_BRACES + RE_INVALID_INTERVAL_ORD)
! /* Syntax bits common to both basic and extended POSIX regex syntax.  */
_RE_SYNTAX_POSIX_COMMON       EQUATE(RE_CHAR_CLASSES + RE_DOT_NEWLINE               |
                                + RE_DOT_NOT_NULL + RE_INTERVALS                    |
                                + RE_NO_EMPTY_RANGES)
RE_SYNTAX_POSIX_BASIC         EQUATE(_RE_SYNTAX_POSIX_COMMON + RE_BK_PLUS_QM        |
                                + RE_CONTEXT_INVALID_DUP)
! /* Differs from ..._POSIX_BASIC only in that RE_BK_PLUS_QM becomes
! RE_LIMITED_OPS, i.e., \? \+ \| are not recognized.  Actually, this
! isn't minimal, since other operators, such as \`, aren't disabled.  */ 
RE_SYNTAX_POSIX_MINIMAL_BASIC EQUATE(_RE_SYNTAX_POSIX_COMMON + RE_LIMITED_OPS)
RE_SYNTAX_POSIX_EXTENDED      EQUATE(_RE_SYNTAX_POSIX_COMMON + RE_CONTEXT_INDEP_ANCHORS |
                                + RE_CONTEXT_INDEP_OPS + RE_NO_BK_BRACES                |
                                + RE_NO_BK_PARENS + RE_NO_BK_VBAR                       |
                                + RE_CONTEXT_INVALID_OPS + RE_UNMATCHED_RIGHT_PAREN_ORD) 
! /* Differs from ..._POSIX_EXTENDED in that RE_CONTEXT_INDEP_OPS is
! removed and RE_NO_BK_REFS is added.  */ 
RE_SYNTAX_POSIX_MINIMAL_EXTENDED  EQUATE(_RE_SYNTAX_POSIX_COMMON + RE_CONTEXT_INDEP_ANCHORS |
                                    + RE_CONTEXT_INVALID_OPS + RE_NO_BK_BRACES              |
                                    + RE_NO_BK_PARENS + RE_NO_BK_REFS                       |
                                    + RE_NO_BK_VBAR	+ RE_UNMATCHED_RIGHT_PAREN_ORD) 
RE_SYNTAX_GNU_AWK             EQUATE(BAND(RE_SYNTAX_POSIX_EXTENDED + RE_BACKSLASH_ESCAPE_IN_LISTS + RE_DEBUG, |
                                (0FFFFFFFFh - (RE_DOT_NOT_NULL + RE_INTERVALS + RE_CONTEXT_INDEP_OPS + RE_CONTEXT_INVALID_OPS))))
RE_SYNTAX_POSIX_AWK           EQUATE(RE_SYNTAX_POSIX_EXTENDED + RE_BACKSLASH_ESCAPE_IN_LISTS + RE_INTERVALS + RE_NO_GNU_OPS)


! /* POSIX `cflags' bits (i.e., information for `regcomp').  */
!
! /* If this bit is set, then use extended regular expression syntax.
! If not set, then use basic regular expression syntax.  */
REG_EXTENDED                  EQUATE(00001b)

! /* If this bit is set, then ignore case when matching.
! If not set, then case is significant.  */
REG_ICASE                     EQUATE(00010b)  !(REG_EXTENDED << 1)

! /* If this bit is set, then anchors do not match at newline
!   characters in the string.
! If not set, then anchors do match at newlines.  */
REG_NEWLINE                   EQUATE(00100b)  !(REG_ICASE << 1)

! /* If this bit is set, then report only success or fail in regexec.
! If not set, then returns differ between not matching and errors.  */
REG_NOSUB                     EQUATE(01000b)  !(REG_NEWLINE << 1)
 
         
! /* POSIX `eflags' bits (i.e., information for regexec).  */

! /* If this bit is set, then the beginning-of-line operator doesn't match
!   the beginning of the string (presumably because it's not the
!   beginning of a line).
! If not set, then the beginning-of-line operator does match the
!   beginning of the string.  */
REG_NOTBOL                    EQUATE(00001b)

! /* Like REG_NOTBOL, except for the end-of-line.  */
REG_NOTEOL                    EQUATE(00010b)!(1 << 1)

! /* Use PMATCH[0] to delimit the start and end of the search in the
! buffer.  */
REG_STARTEND                  EQUATE(00100b)!(1 << 2)


!#define RE_TRANSLATE_TYPE unsigned char *
RE_TRANSLATE_TYPE             EQUATE(LONG)

size_t                        EQUATE(UNSIGNED)

REGS_UNALLOCATED              EQUATE(0)
REGS_REALLOCATE               EQUATE(1)
REGS_FIXED                    EQUATE(2)

! /* This data structure represents a compiled pattern.  Before calling
! the pattern compiler, the fields `buffer', `allocated', `fastmap',
! `translate', and `no_sub' can be set.  After the pattern has been
! compiled, the `re_nsub' field is available.  All other fields are
! private to the regex routines.  */ 
re_pattern_buffer             GROUP, TYPE

! /* Space that holds the compiled pattern.  It is declared as
! `unsigned char *' because its elements are sometimes used as
! array indexes.  */ 
!unsigned  char *buffer;
buffer                          LONG

! /* Number of bytes to which `buffer' points.  */
allocated                       UNSIGNED

! /* Number of bytes actually used in `buffer'.  */
used                            UNSIGNED

! /* Syntax setting with which the pattern was compiled.  */
syntax                          reg_syntax_t

! /* Pointer to a fastmap, if any, otherwise zero.  re_search uses the
! fastmap, if there is one, to skip over impossible starting points
! for matches.  */
!char *fastmap;
fastmap                         LONG

! /* Either a translate table to apply to all characters before
! comparing them, or zero for no translation.  The translation is
! applied to a pattern when it is compiled and to a string when it
! is matched.  */
translate                       RE_TRANSLATE_TYPE

! /* Number of subexpressions found by the compiler.  */
re_nsub                         size_t

! /* Zero if this pattern cannot match the empty string, one else.
! Well, in truth it's used only in `re_search_2', to see whether or
! not we should use the fastmap, so we don't set this absolutely
! perfectly; see `re_compile_fastmap' (the `duplicate' case).  */
can_be_null                     UNSIGNED(1)

! /* If REGS_UNALLOCATED, allocate space in the `regs' structure
! for `max (RE_NREGS, re_nsub + 1)' groups.
! If REGS_REALLOCATE, reallocate space if necessary.
! If REGS_FIXED, use what's there.  */
! #define REGS_UNALLOCATED 0
! #define REGS_REALLOCATE 1
! #define REGS_FIXED 2
regs_allocated                  UNSIGNED(2)

! /* Set to zero when `regex_compile' compiles a pattern; set to one
! by `re_compile_fastmap' if it updates the fastmap.  */
fastmap_accurate                UNSIGNED(1)

! /* If set, `re_match_2' does not return information about
! subexpressions.  */
no_sub                          UNSIGNED(1)

! /* If set, a beginning-of-line anchor doesn't match at the beginning
! of the string.  */
not_bol                         UNSIGNED(1)

! /* Similarly for an end-of-line anchor.  */
not_eol                         UNSIGNED(1)

! /* If true, an anchor at a newline matches.  */
newline_anchor                  UNSIGNED(1)
    
                              END

regex_t                       LIKE(re_pattern_buffer)

! This is the structure we store register match data in.  See
! regex.texinfo for a full description of what registers match.
re_registers                  GROUP, TYPE
num_regs                        UNSIGNED
pstart                          LONG  !regoff_t* -- array
pend                            LONG  !regoff_t* -- array
                              END

! /* Type for byte offsets within the string.  POSIX mandates this.  */
!typedef int regoff_t; 
regoff_t                      EQUATE(LONG)

! /* POSIX specification for registers.  Aside from the different names than
! `re_registers', POSIX uses an array of structures, instead of a
! structure of arrays.  */
!typedef struct
!{
!  regoff_t  rm_so;  /* Byte offset from string's start to substring's start.  */
!  regoff_t  rm_eo;  /* Byte offset from string's start to substring's end.  */
!} regmatch_t; 
regmatch_t                    GROUP, TYPE
rm_so                           regoff_t
rm_eo                           regoff_t
                              END

reg_errcode_t                 ITEMIZE
REG_NOERROR                     EQUATE(0) !Success
REG_NOMATCH                     EQUATE    !Didn't find a match (for regexec)

!/* POSIX regcomp return error codes.  (In the order listed in the standard.)  */
REG_BADPAT                      EQUATE    !Invalid pattern
REG_ECOLLATE                    EQUATE    !Inalid collating element
REG_ECTYPE                      EQUATE    !Invalid character class name
REG_EESCAPE                     EQUATE    !Trailing backslash
REG_ESUBREG                     EQUATE    !Invalid back reference
REG_EBRACK                      EQUATE    !Unmatched left bracket
REG_EPAREN                      EQUATE    !Parenthesis imbalance
REG_EBRACE                      EQUATE    !Unmatched \{
REG_BADBR                       EQUATE    !Invalid contents of \{\}
REG_ERANGE                      EQUATE    !Invalid range end
REG_ESPACE                      EQUATE    !Ran out of memory
REG_BADRPT                      EQUATE    !No preceding re for repetition op

!/* Error codes we've added.  */
REG_EEND                        EQUATE    !Premature end.
REG_ESIZE                       EQUATE    !Compiled pattern bigger than 2^16 bytes
REG_ERPAREN                     EQUATE    !Unmatched ) or \); not returned from regcomp
                              END
