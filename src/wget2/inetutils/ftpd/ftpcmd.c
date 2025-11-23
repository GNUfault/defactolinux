/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 70 "ftpcmd.y"


#include <config.h>

#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/ftp.h>

#include <ctype.h>
#include <errno.h>
#include <pwd.h>
#include <setjmp.h>
#include <signal.h>
#ifdef HAVE_INTTYPES_H
# include <inttypes.h>	/* strtoimax */
#elif defined HAVE_STDINT_H
# include <stdint.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <sys/utsname.h>
/* Include glob.h last, because it may define "const" which breaks
   system headers on some platforms. */
#include <glob.h>

#include "extern.h"

#if !defined NBBY && defined CHAR_BIT
#define NBBY CHAR_BIT
#endif

off_t restart_point;

static char cbuf[512];           /* Command Buffer.  */
static char *fromname;
static int cmd_type;
static int cmd_form;
static int cmd_bytesz;

struct tab
{
  const char	*name;
  short	token;
  short	state;
  short	implemented;	/* 1 if command is implemented */
  const char	*help;
};

static struct tab cmdtab[];
static struct tab sitetab[];
static char *extlist[];
static char *copy         (char *);
static void help          (struct tab *, char *);
static struct tab *lookup (struct tab *, char *);
static void sizecmd       (char *);
static int yylex          (void);
static void yyerror       (const char *s);


#line 142 "ftpcmd.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    A = 258,                       /* A  */
    B = 259,                       /* B  */
    C = 260,                       /* C  */
    E = 261,                       /* E  */
    F = 262,                       /* F  */
    I = 263,                       /* I  */
    L = 264,                       /* L  */
    N = 265,                       /* N  */
    P = 266,                       /* P  */
    R = 267,                       /* R  */
    S = 268,                       /* S  */
    T = 269,                       /* T  */
    SP = 270,                      /* SP  */
    CRLF = 271,                    /* CRLF  */
    COMMA = 272,                   /* COMMA  */
    USER = 273,                    /* USER  */
    PASS = 274,                    /* PASS  */
    ACCT = 275,                    /* ACCT  */
    REIN = 276,                    /* REIN  */
    QUIT = 277,                    /* QUIT  */
    PORT = 278,                    /* PORT  */
    PASV = 279,                    /* PASV  */
    TYPE = 280,                    /* TYPE  */
    STRU = 281,                    /* STRU  */
    MODE = 282,                    /* MODE  */
    RETR = 283,                    /* RETR  */
    STOR = 284,                    /* STOR  */
    APPE = 285,                    /* APPE  */
    MLFL = 286,                    /* MLFL  */
    MAIL = 287,                    /* MAIL  */
    MSND = 288,                    /* MSND  */
    MSOM = 289,                    /* MSOM  */
    MSAM = 290,                    /* MSAM  */
    MRSQ = 291,                    /* MRSQ  */
    MRCP = 292,                    /* MRCP  */
    ALLO = 293,                    /* ALLO  */
    REST = 294,                    /* REST  */
    RNFR = 295,                    /* RNFR  */
    RNTO = 296,                    /* RNTO  */
    ABOR = 297,                    /* ABOR  */
    DELE = 298,                    /* DELE  */
    CWD = 299,                     /* CWD  */
    LIST = 300,                    /* LIST  */
    NLST = 301,                    /* NLST  */
    SITE = 302,                    /* SITE  */
    STAT = 303,                    /* STAT  */
    HELP = 304,                    /* HELP  */
    NOOP = 305,                    /* NOOP  */
    MKD = 306,                     /* MKD  */
    RMD = 307,                     /* RMD  */
    PWD = 308,                     /* PWD  */
    CDUP = 309,                    /* CDUP  */
    STOU = 310,                    /* STOU  */
    SMNT = 311,                    /* SMNT  */
    SYST = 312,                    /* SYST  */
    SIZE = 313,                    /* SIZE  */
    MDTM = 314,                    /* MDTM  */
    FEAT = 315,                    /* FEAT  */
    OPTS = 316,                    /* OPTS  */
    EPRT = 317,                    /* EPRT  */
    EPSV = 318,                    /* EPSV  */
    LPRT = 319,                    /* LPRT  */
    LPSV = 320,                    /* LPSV  */
    ADAT = 321,                    /* ADAT  */
    AUTH = 322,                    /* AUTH  */
    CCC = 323,                     /* CCC  */
    CONF = 324,                    /* CONF  */
    ENC = 325,                     /* ENC  */
    MIC = 326,                     /* MIC  */
    PBSZ = 327,                    /* PBSZ  */
    PROT = 328,                    /* PROT  */
    UMASK = 329,                   /* UMASK  */
    IDLE = 330,                    /* IDLE  */
    CHMOD = 331,                   /* CHMOD  */
    LEXERR = 332,                  /* LEXERR  */
    STRING = 333,                  /* STRING  */
    NUMBER = 334,                  /* NUMBER  */
    CHAR = 335                     /* CHAR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define A 258
#define B 259
#define C 260
#define E 261
#define F 262
#define I 263
#define L 264
#define N 265
#define P 266
#define R 267
#define S 268
#define T 269
#define SP 270
#define CRLF 271
#define COMMA 272
#define USER 273
#define PASS 274
#define ACCT 275
#define REIN 276
#define QUIT 277
#define PORT 278
#define PASV 279
#define TYPE 280
#define STRU 281
#define MODE 282
#define RETR 283
#define STOR 284
#define APPE 285
#define MLFL 286
#define MAIL 287
#define MSND 288
#define MSOM 289
#define MSAM 290
#define MRSQ 291
#define MRCP 292
#define ALLO 293
#define REST 294
#define RNFR 295
#define RNTO 296
#define ABOR 297
#define DELE 298
#define CWD 299
#define LIST 300
#define NLST 301
#define SITE 302
#define STAT 303
#define HELP 304
#define NOOP 305
#define MKD 306
#define RMD 307
#define PWD 308
#define CDUP 309
#define STOU 310
#define SMNT 311
#define SYST 312
#define SIZE 313
#define MDTM 314
#define FEAT 315
#define OPTS 316
#define EPRT 317
#define EPSV 318
#define LPRT 319
#define LPSV 320
#define ADAT 321
#define AUTH 322
#define CCC 323
#define CONF 324
#define ENC 325
#define MIC 326
#define PBSZ 327
#define PROT 328
#define UMASK 329
#define IDLE 330
#define CHMOD 331
#define LEXERR 332
#define STRING 333
#define NUMBER 334
#define CHAR 335

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 141 "ftpcmd.y"

	intmax_t i;
	char   *s;

#line 357 "ftpcmd.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);



/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_A = 3,                          /* A  */
  YYSYMBOL_B = 4,                          /* B  */
  YYSYMBOL_C = 5,                          /* C  */
  YYSYMBOL_E = 6,                          /* E  */
  YYSYMBOL_F = 7,                          /* F  */
  YYSYMBOL_I = 8,                          /* I  */
  YYSYMBOL_L = 9,                          /* L  */
  YYSYMBOL_N = 10,                         /* N  */
  YYSYMBOL_P = 11,                         /* P  */
  YYSYMBOL_R = 12,                         /* R  */
  YYSYMBOL_S = 13,                         /* S  */
  YYSYMBOL_T = 14,                         /* T  */
  YYSYMBOL_SP = 15,                        /* SP  */
  YYSYMBOL_CRLF = 16,                      /* CRLF  */
  YYSYMBOL_COMMA = 17,                     /* COMMA  */
  YYSYMBOL_USER = 18,                      /* USER  */
  YYSYMBOL_PASS = 19,                      /* PASS  */
  YYSYMBOL_ACCT = 20,                      /* ACCT  */
  YYSYMBOL_REIN = 21,                      /* REIN  */
  YYSYMBOL_QUIT = 22,                      /* QUIT  */
  YYSYMBOL_PORT = 23,                      /* PORT  */
  YYSYMBOL_PASV = 24,                      /* PASV  */
  YYSYMBOL_TYPE = 25,                      /* TYPE  */
  YYSYMBOL_STRU = 26,                      /* STRU  */
  YYSYMBOL_MODE = 27,                      /* MODE  */
  YYSYMBOL_RETR = 28,                      /* RETR  */
  YYSYMBOL_STOR = 29,                      /* STOR  */
  YYSYMBOL_APPE = 30,                      /* APPE  */
  YYSYMBOL_MLFL = 31,                      /* MLFL  */
  YYSYMBOL_MAIL = 32,                      /* MAIL  */
  YYSYMBOL_MSND = 33,                      /* MSND  */
  YYSYMBOL_MSOM = 34,                      /* MSOM  */
  YYSYMBOL_MSAM = 35,                      /* MSAM  */
  YYSYMBOL_MRSQ = 36,                      /* MRSQ  */
  YYSYMBOL_MRCP = 37,                      /* MRCP  */
  YYSYMBOL_ALLO = 38,                      /* ALLO  */
  YYSYMBOL_REST = 39,                      /* REST  */
  YYSYMBOL_RNFR = 40,                      /* RNFR  */
  YYSYMBOL_RNTO = 41,                      /* RNTO  */
  YYSYMBOL_ABOR = 42,                      /* ABOR  */
  YYSYMBOL_DELE = 43,                      /* DELE  */
  YYSYMBOL_CWD = 44,                       /* CWD  */
  YYSYMBOL_LIST = 45,                      /* LIST  */
  YYSYMBOL_NLST = 46,                      /* NLST  */
  YYSYMBOL_SITE = 47,                      /* SITE  */
  YYSYMBOL_STAT = 48,                      /* STAT  */
  YYSYMBOL_HELP = 49,                      /* HELP  */
  YYSYMBOL_NOOP = 50,                      /* NOOP  */
  YYSYMBOL_MKD = 51,                       /* MKD  */
  YYSYMBOL_RMD = 52,                       /* RMD  */
  YYSYMBOL_PWD = 53,                       /* PWD  */
  YYSYMBOL_CDUP = 54,                      /* CDUP  */
  YYSYMBOL_STOU = 55,                      /* STOU  */
  YYSYMBOL_SMNT = 56,                      /* SMNT  */
  YYSYMBOL_SYST = 57,                      /* SYST  */
  YYSYMBOL_SIZE = 58,                      /* SIZE  */
  YYSYMBOL_MDTM = 59,                      /* MDTM  */
  YYSYMBOL_FEAT = 60,                      /* FEAT  */
  YYSYMBOL_OPTS = 61,                      /* OPTS  */
  YYSYMBOL_EPRT = 62,                      /* EPRT  */
  YYSYMBOL_EPSV = 63,                      /* EPSV  */
  YYSYMBOL_LPRT = 64,                      /* LPRT  */
  YYSYMBOL_LPSV = 65,                      /* LPSV  */
  YYSYMBOL_ADAT = 66,                      /* ADAT  */
  YYSYMBOL_AUTH = 67,                      /* AUTH  */
  YYSYMBOL_CCC = 68,                       /* CCC  */
  YYSYMBOL_CONF = 69,                      /* CONF  */
  YYSYMBOL_ENC = 70,                       /* ENC  */
  YYSYMBOL_MIC = 71,                       /* MIC  */
  YYSYMBOL_PBSZ = 72,                      /* PBSZ  */
  YYSYMBOL_PROT = 73,                      /* PROT  */
  YYSYMBOL_UMASK = 74,                     /* UMASK  */
  YYSYMBOL_IDLE = 75,                      /* IDLE  */
  YYSYMBOL_CHMOD = 76,                     /* CHMOD  */
  YYSYMBOL_LEXERR = 77,                    /* LEXERR  */
  YYSYMBOL_STRING = 78,                    /* STRING  */
  YYSYMBOL_NUMBER = 79,                    /* NUMBER  */
  YYSYMBOL_CHAR = 80,                      /* CHAR  */
  YYSYMBOL_YYACCEPT = 81,                  /* $accept  */
  YYSYMBOL_cmd_list = 82,                  /* cmd_list  */
  YYSYMBOL_cmd = 83,                       /* cmd  */
  YYSYMBOL_rcmd = 84,                      /* rcmd  */
  YYSYMBOL_username = 85,                  /* username  */
  YYSYMBOL_password = 86,                  /* password  */
  YYSYMBOL_byte_size = 87,                 /* byte_size  */
  YYSYMBOL_net_proto = 88,                 /* net_proto  */
  YYSYMBOL_tcp_port = 89,                  /* tcp_port  */
  YYSYMBOL_net_addr = 90,                  /* net_addr  */
  YYSYMBOL_host_port = 91,                 /* host_port  */
  YYSYMBOL_long_host_port = 92,            /* long_host_port  */
  YYSYMBOL_form_code = 93,                 /* form_code  */
  YYSYMBOL_type_code = 94,                 /* type_code  */
  YYSYMBOL_struct_code = 95,               /* struct_code  */
  YYSYMBOL_mode_code = 96,                 /* mode_code  */
  YYSYMBOL_pathname = 97,                  /* pathname  */
  YYSYMBOL_pathstring = 98,                /* pathstring  */
  YYSYMBOL_octal_number = 99,              /* octal_number  */
  YYSYMBOL_check_login = 100               /* check_login  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   319

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  81
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  89
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  290

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   335


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   185,   185,   186,   192,   196,   201,   207,   246,   251,
     289,   301,   313,   317,   321,   327,   333,   339,   344,   350,
     355,   361,   367,   371,   377,   392,   396,   401,   407,   411,
     429,   433,   439,   445,   450,   455,   468,   480,   487,   495,
     499,   504,   515,   531,   545,   551,   569,   575,   619,   634,
     662,   813,   818,   833,   874,   880,   885,   891,   906,   918,
     923,   926,   930,   934,   947,   951,   955,  1004,  1066,  1135,
    1139,  1143,  1150,  1155,  1160,  1165,  1170,  1174,  1179,  1185,
    1193,  1197,  1201,  1208,  1212,  1216,  1223,  1264,  1268,  1297
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "A", "B", "C", "E",
  "F", "I", "L", "N", "P", "R", "S", "T", "SP", "CRLF", "COMMA", "USER",
  "PASS", "ACCT", "REIN", "QUIT", "PORT", "PASV", "TYPE", "STRU", "MODE",
  "RETR", "STOR", "APPE", "MLFL", "MAIL", "MSND", "MSOM", "MSAM", "MRSQ",
  "MRCP", "ALLO", "REST", "RNFR", "RNTO", "ABOR", "DELE", "CWD", "LIST",
  "NLST", "SITE", "STAT", "HELP", "NOOP", "MKD", "RMD", "PWD", "CDUP",
  "STOU", "SMNT", "SYST", "SIZE", "MDTM", "FEAT", "OPTS", "EPRT", "EPSV",
  "LPRT", "LPSV", "ADAT", "AUTH", "CCC", "CONF", "ENC", "MIC", "PBSZ",
  "PROT", "UMASK", "IDLE", "CHMOD", "LEXERR", "STRING", "NUMBER", "CHAR",
  "$accept", "cmd_list", "cmd", "rcmd", "username", "password",
  "byte_size", "net_proto", "tcp_port", "net_addr", "host_port",
  "long_host_port", "form_code", "type_code", "struct_code", "mode_code",
  "pathname", "pathstring", "octal_number", "check_login", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-106)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -106,    41,  -106,    -3,     4,     7,    14,  -106,  -106,    16,
      19,    24,  -106,  -106,  -106,    33,    43,  -106,  -106,    58,
    -106,  -106,  -106,  -106,    61,    99,    -1,   101,  -106,  -106,
    -106,  -106,  -106,   116,  -106,  -106,  -106,  -106,  -106,  -106,
    -106,  -106,  -106,  -106,  -106,    55,    56,  -106,   123,   124,
      69,     5,    -2,   126,   127,   128,    60,    65,   130,   131,
    -106,   132,    46,    93,    95,    48,  -106,   133,    71,  -106,
    -106,   135,   136,   137,   138,   140,  -106,   141,   142,    98,
     103,   143,   105,   144,   145,  -106,   146,  -106,   147,    73,
    -106,   149,   150,  -106,    -6,   151,  -106,  -106,  -106,   152,
    -106,  -106,  -106,   153,    82,    82,    82,   110,  -106,   154,
      82,    82,    82,    82,  -106,    82,  -106,    88,  -106,   112,
    -106,   155,  -106,    82,   156,    82,    82,  -106,  -106,    82,
      82,    82,    96,  -106,    97,  -106,   100,    94,  -106,   104,
    -106,  -106,  -106,   159,   161,   102,   102,    65,  -106,  -106,
    -106,  -106,  -106,   162,  -106,   163,   165,   170,  -106,  -106,
     168,   169,   171,   172,   173,   174,   108,  -106,   115,  -106,
     176,   177,   178,  -106,   179,   180,   181,   182,   183,   184,
     185,    94,  -106,   186,   187,   189,   114,  -106,  -106,  -106,
    -106,  -106,  -106,  -106,  -106,  -106,  -106,   188,  -106,  -106,
    -106,  -106,  -106,  -106,   190,   129,  -106,   134,   129,  -106,
    -106,  -106,  -106,  -106,  -106,  -106,  -106,   139,  -106,   148,
    -106,   192,   157,  -106,  -106,   191,   194,   196,   160,   195,
     158,   198,  -106,  -106,    82,  -106,   164,   166,   199,  -106,
     201,   167,   203,   175,  -106,  -106,   193,   197,   204,   202,
     205,   200,  -106,   206,   207,   208,   209,   210,  -106,   211,
     212,   213,   214,   215,   216,   217,   218,   222,   219,   223,
     220,   224,   221,   225,   226,   230,   227,   231,   228,   232,
     229,   233,   234,   235,   236,   238,   237,   239,   240,  -106
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,    89,    89,     0,
       0,     0,    89,    89,    89,     0,     0,    89,    89,     0,
      89,    89,    89,    89,     0,    89,     0,     0,    89,    89,
      89,    89,    89,     0,    89,    89,    89,    89,    89,    89,
      89,    89,     3,     4,    56,     0,    60,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,    22,     0,     0,    28,
      30,     0,     0,     0,     0,     0,    47,     0,     0,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,     0,
       8,    72,    74,    76,    77,     0,    80,    82,    81,     0,
      84,    85,    83,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,    26,     0,    19,     0,    17,     0,
      89,    89,    89,     0,     0,     0,     0,    33,    34,     0,
       0,     0,     0,    35,     0,    37,     0,     0,    51,     0,
      54,     5,     6,     0,     0,     0,     0,     0,    79,     9,
      10,    11,    87,     0,    86,     0,     0,     0,    12,    58,
       0,     0,     0,     0,     0,     0,     0,    39,     0,    44,
       0,     0,     0,    29,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     7,    71,    69,
      70,    73,    75,    78,    14,    15,    16,     0,    57,    24,
      23,    27,    20,    18,     0,     0,    41,     0,     0,    21,
      31,    32,    46,    48,    49,    36,    38,     0,    52,     0,
      53,     0,     0,    40,    88,     0,     0,     0,     0,     0,
       0,     0,    42,    45,     0,    65,     0,     0,     0,    13,
       0,     0,     0,     0,    43,    64,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -106,  -106,  -106,  -106,  -106,  -106,   -90,    34,  -106,  -106,
    -106,  -106,    77,  -106,  -106,  -106,  -105,  -106,    18,    15
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,    42,    43,    86,    88,   109,   183,   246,   236,
     144,   185,   191,    95,    99,   103,   153,   154,   225,    48
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     155,   156,   100,   101,   148,   160,   161,   162,   163,   147,
     164,   102,    96,    44,    68,    69,    97,    98,   172,    45,
     174,   175,    46,    49,   176,   177,   178,    53,    54,    55,
      47,    50,    58,    59,    51,    61,    62,    63,    64,    52,
      67,     2,     3,    71,    72,    73,    74,    75,    56,    77,
      78,    79,    80,    81,    82,    83,    84,   193,    57,     4,
       5,   113,   114,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    91,   108,    60,    92,    65,    93,    94,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,   119,    33,    34,
      35,    36,    37,    38,    39,    40,    41,   188,   115,   116,
     117,   118,   189,   132,   133,    66,   190,    70,   134,   135,
     137,   138,   120,   121,   122,   157,   158,   166,   167,   240,
     205,   206,    76,    85,    87,   168,   170,   171,    89,   107,
      90,   104,   105,   106,   108,   110,   111,   112,   123,   124,
     125,   126,   143,   127,   128,   129,   130,   131,   136,   139,
     152,   140,   141,   142,   145,   146,   165,   149,   150,   151,
     159,   169,   173,   182,   179,   180,   186,   187,   194,   195,
     181,   196,   197,   184,   198,   199,   204,   200,   201,   202,
     203,   207,   208,   221,   209,   210,   211,   212,   213,   214,
     215,   216,   218,   222,   219,   220,   223,   232,   224,   230,
     233,   234,   237,   226,   239,   217,   243,   244,   252,   228,
     247,   251,   253,   192,   256,   257,   227,   229,   260,     0,
     262,     0,   264,     0,   266,     0,   231,   238,   235,   268,
     270,   272,   274,     0,   241,   242,   245,   276,   278,   280,
     282,     0,   284,     0,   248,   286,   288,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   249,     0,     0,   250,     0,     0,   254,
       0,     0,     0,     0,     0,   255,     0,     0,   258,   259,
       0,   261,     0,   263,     0,   265,     0,   267,   269,   271,
     273,     0,     0,     0,     0,   275,   277,   279,   281,     0,
       0,     0,     0,   283,     0,   285,   287,     0,     0,   289
};

static const yytype_int16 yycheck[] =
{
     105,   106,     4,     5,    94,   110,   111,   112,   113,    15,
     115,    13,     7,    16,    15,    16,    11,    12,   123,    15,
     125,   126,    15,     8,   129,   130,   131,    12,    13,    14,
      16,    15,    17,    18,    15,    20,    21,    22,    23,    15,
      25,     0,     1,    28,    29,    30,    31,    32,    15,    34,
      35,    36,    37,    38,    39,    40,    41,   147,    15,    18,
      19,    15,    16,    22,    23,    24,    25,    26,    27,    28,
      29,    30,     3,    79,    16,     6,    15,     8,     9,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    49,    57,    58,
      59,    60,    61,    62,    63,    64,    65,     5,    15,    16,
      15,    16,    10,    15,    16,    16,    14,    16,    15,    16,
      15,    16,    74,    75,    76,    15,    16,    15,    16,   234,
      15,    16,    16,    78,    78,   120,   121,   122,    15,    79,
      16,    15,    15,    15,    79,    15,    15,    15,    15,    78,
      15,    15,    79,    16,    16,    15,    15,    15,    15,    15,
      78,    16,    16,    16,    15,    15,    78,    16,    16,    16,
      16,    16,    16,    79,    78,    78,    17,    16,    16,    16,
      80,    16,    12,    79,    16,    16,    78,    16,    16,    16,
      16,    15,    15,    79,    16,    16,    16,    16,    16,    16,
      16,    16,    16,    15,    17,    16,    16,    16,    79,    17,
      16,    15,    17,    79,    16,   181,    17,    16,    16,    80,
      17,    17,    17,   146,    17,    17,   208,    79,    17,    -1,
      17,    -1,    17,    -1,    17,    -1,    79,    79,    78,    17,
      17,    17,    17,    -1,    80,    79,    79,    17,    17,    17,
      17,    -1,    17,    -1,    79,    17,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    79,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    79,    79,
      -1,    79,    -1,    79,    -1,    79,    -1,    79,    79,    79,
      79,    -1,    -1,    -1,    -1,    79,    79,    79,    79,    -1,
      -1,    -1,    -1,    79,    -1,    79,    79,    -1,    -1,    79
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    82,     0,     1,    18,    19,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    83,    84,    16,    15,    15,    16,   100,   100,
      15,    15,    15,   100,   100,   100,    15,    15,   100,   100,
      16,   100,   100,   100,   100,    15,    16,   100,    15,    16,
      16,   100,   100,   100,   100,   100,    16,   100,   100,   100,
     100,   100,   100,   100,   100,    78,    85,    78,    86,    15,
      16,     3,     6,     8,     9,    94,     7,    11,    12,    95,
       4,     5,    13,    96,    15,    15,    15,    79,    79,    87,
      15,    15,    15,    15,    16,    15,    16,    15,    16,    49,
      74,    75,    76,    15,    78,    15,    15,    16,    16,    15,
      15,    15,    15,    16,    15,    16,    15,    15,    16,    15,
      16,    16,    16,    79,    91,    15,    15,    15,    87,    16,
      16,    16,    78,    97,    98,    97,    97,    15,    16,    16,
      97,    97,    97,    97,    97,    78,    15,    16,   100,    16,
     100,   100,    97,    16,    97,    97,    97,    97,    97,    78,
      78,    80,    79,    88,    79,    92,    17,    16,     5,    10,
      14,    93,    93,    87,    16,    16,    16,    12,    16,    16,
      16,    16,    16,    16,    78,    15,    16,    15,    15,    16,
      16,    16,    16,    16,    16,    16,    16,    88,    16,    17,
      16,    79,    15,    16,    79,    99,    79,    99,    80,    79,
      17,    79,    16,    16,    15,    78,    90,    17,    79,    16,
      97,    80,    79,    17,    16,    79,    89,    17,    79,    80,
      79,    17,    16,    17,    79,    79,    17,    17,    79,    79,
      17,    79,    17,    79,    17,    79,    17,    79,    17,    79,
      17,    79,    17,    79,    17,    79,    17,    79,    17,    79,
      17,    79,    17,    79,    17,    79,    17,    79,    17,    79
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    81,    82,    82,    82,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    84,    84,    85,
      86,    86,    87,    88,    89,    90,    91,    92,    92,    93,
      93,    93,    94,    94,    94,    94,    94,    94,    94,    94,
      95,    95,    95,    96,    96,    96,    97,    98,    99,   100
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     4,     4,     5,     3,     4,
       4,     4,     4,     8,     5,     5,     5,     3,     5,     3,
       5,     5,     2,     5,     5,     2,     3,     5,     2,     4,
       2,     5,     5,     3,     3,     3,     5,     3,     5,     4,
       6,     5,     7,     9,     4,     7,     5,     2,     5,     5,
      11,     3,     5,     5,     3,     2,     2,     5,     4,     1,
       0,     1,     1,     1,     1,     1,    11,    17,    41,     1,
       1,     1,     1,     3,     1,     3,     1,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 3: /* cmd_list: cmd_list cmd  */
#line 187 "ftpcmd.y"
                {
			free (fromname);
			fromname = (char *) 0;
			restart_point = (off_t) 0;
		}
#line 1617 "ftpcmd.c"
    break;

  case 5: /* cmd: USER SP username CRLF  */
#line 197 "ftpcmd.y"
                {
			user ((yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 1626 "ftpcmd.c"
    break;

  case 6: /* cmd: PASS SP password CRLF  */
#line 202 "ftpcmd.y"
                {
			pass ((yyvsp[-1].s));
			memset ((yyvsp[-1].s), 0, strlen ((yyvsp[-1].s)));
			free ((yyvsp[-1].s));
		}
#line 1636 "ftpcmd.c"
    break;

  case 7: /* cmd: PORT check_login SP host_port CRLF  */
#line 208 "ftpcmd.y"
                {
			if ((yyvsp[-3].i))
			  {
			    if ((yyvsp[-1].i)
				&& ((his_addr.ss_family == AF_INET
				     && memcmp (&((struct sockaddr_in *) &his_addr)->sin_addr,
						&((struct sockaddr_in *) &data_dest)->sin_addr,
						sizeof (struct in_addr))
					== 0
				     && ntohs (((struct sockaddr_in *) &data_dest)->sin_port)
					> IPPORT_RESERVED)
				    ||
				    (his_addr.ss_family == AF_INET6
				     && memcmp (&((struct sockaddr_in6 *) &his_addr)->sin6_addr,
						&((struct sockaddr_in6 *) &data_dest)->sin6_addr,
						sizeof (struct in6_addr))
					== 0
				     && ntohs (((struct sockaddr_in6 *) &data_dest)->sin6_port)
					> IPPORT_RESERVED)
				   )
			       )
			      {
				usedefault = 0;
				if (pdata >= 0)
				  {
				    close (pdata);
				    pdata = -1;
				  }
				reply (200, "PORT command successful.");
			      }
			    else
			      {
				usedefault = 1;
				memset (&data_dest, 0, sizeof (data_dest));
				reply (500, "Illegal PORT Command");
			      }
			  }
		}
#line 1679 "ftpcmd.c"
    break;

  case 8: /* cmd: PASV check_login CRLF  */
#line 247 "ftpcmd.y"
                {
			if ((yyvsp[-1].i))
			  passive (PASSIVE_PASV, AF_INET);
		}
#line 1688 "ftpcmd.c"
    break;

  case 9: /* cmd: TYPE SP type_code CRLF  */
#line 252 "ftpcmd.y"
                {
			switch (cmd_type)
			  {
			  case TYPE_A:
			    if (cmd_form == FORM_N)
			      {
				reply (200, "Type set to A.");
				type = cmd_type;
				form = cmd_form;
			      }
			    else
			      reply (504, "Form must be N.");
			    break;

			  case TYPE_E:
			    reply (504, "Type E not implemented.");
			    break;

			  case TYPE_I:
			    reply (200, "Type set to I.");
			    type = cmd_type;
			    break;

			  case TYPE_L:
#if defined NBBY && NBBY == 8
			    if (cmd_bytesz == 8)
			      {
				reply (200, "Type set to L (byte size 8).");
				type = cmd_type;
			      }
			    else
			      reply (504, "Byte size must be 8.");
#else /* NBBY == 8 */
			  UNIMPLEMENTED for NBBY != 8
#endif /* NBBY == 8 */
			  }
		}
#line 1730 "ftpcmd.c"
    break;

  case 10: /* cmd: STRU SP struct_code CRLF  */
#line 290 "ftpcmd.y"
                {
			switch ((yyvsp[-1].i))
			  {
			  case STRU_F:
			    reply (200, "STRU F ok.");
			    break;

			  default:
			    reply (504, "Unimplemented STRU type.");
			  }
		}
#line 1746 "ftpcmd.c"
    break;

  case 11: /* cmd: MODE SP mode_code CRLF  */
#line 302 "ftpcmd.y"
                {
			switch ((yyvsp[-1].i))
			  {
			  case MODE_S:
			    reply (200, "MODE S ok.");
			    break;

			  default:
			    reply (502, "Unimplemented MODE type.");
			  }
		}
#line 1762 "ftpcmd.c"
    break;

  case 12: /* cmd: ALLO SP NUMBER CRLF  */
#line 314 "ftpcmd.y"
                {
			reply (202, "ALLO command ignored.");
		}
#line 1770 "ftpcmd.c"
    break;

  case 13: /* cmd: ALLO SP NUMBER SP R SP NUMBER CRLF  */
#line 318 "ftpcmd.y"
                {
			reply (202, "ALLO command ignored.");
		}
#line 1778 "ftpcmd.c"
    break;

  case 14: /* cmd: RETR check_login SP pathname CRLF  */
#line 322 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  retrieve ((char *) 0, (yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 1788 "ftpcmd.c"
    break;

  case 15: /* cmd: STOR check_login SP pathname CRLF  */
#line 328 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  store ((yyvsp[-1].s), "w", 0);
			free ((yyvsp[-1].s));
		}
#line 1798 "ftpcmd.c"
    break;

  case 16: /* cmd: APPE check_login SP pathname CRLF  */
#line 334 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  store ((yyvsp[-1].s), "a", 0);
			free ((yyvsp[-1].s));
		}
#line 1808 "ftpcmd.c"
    break;

  case 17: /* cmd: NLST check_login CRLF  */
#line 340 "ftpcmd.y"
                {
			if ((yyvsp[-1].i))
			  send_file_list (".");
		}
#line 1817 "ftpcmd.c"
    break;

  case 18: /* cmd: NLST check_login SP STRING CRLF  */
#line 345 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  send_file_list ((yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 1827 "ftpcmd.c"
    break;

  case 19: /* cmd: LIST check_login CRLF  */
#line 351 "ftpcmd.y"
                {
			if ((yyvsp[-1].i))
			  retrieve ("/bin/ls -lgA", "");
		}
#line 1836 "ftpcmd.c"
    break;

  case 20: /* cmd: LIST check_login SP pathname CRLF  */
#line 356 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  retrieve ("/bin/ls -lgA %s", (yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 1846 "ftpcmd.c"
    break;

  case 21: /* cmd: STAT check_login SP pathname CRLF  */
#line 362 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  statfilecmd ((yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 1856 "ftpcmd.c"
    break;

  case 22: /* cmd: STAT CRLF  */
#line 368 "ftpcmd.y"
                {
			statcmd ();
		}
#line 1864 "ftpcmd.c"
    break;

  case 23: /* cmd: DELE check_login SP pathname CRLF  */
#line 372 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  delete ((yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 1874 "ftpcmd.c"
    break;

  case 24: /* cmd: RNTO check_login SP pathname CRLF  */
#line 378 "ftpcmd.y"
                {
			if ((yyvsp[-3].i))
			  {
			    if (fromname)
			      {
				renamecmd (fromname, (yyvsp[-1].s));
				free (fromname);
				fromname = (char *) 0;
			      }
			    else
			      reply (503, "Bad sequence of commands.");
			  }
			free ((yyvsp[-1].s));
		}
#line 1893 "ftpcmd.c"
    break;

  case 25: /* cmd: ABOR CRLF  */
#line 393 "ftpcmd.y"
                {
			reply (225, "ABOR command successful.");
		}
#line 1901 "ftpcmd.c"
    break;

  case 26: /* cmd: CWD check_login CRLF  */
#line 397 "ftpcmd.y"
                {
			if ((yyvsp[-1].i))
			  cwd (cred.homedir);
		}
#line 1910 "ftpcmd.c"
    break;

  case 27: /* cmd: CWD check_login SP pathname CRLF  */
#line 402 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  cwd ((yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 1920 "ftpcmd.c"
    break;

  case 28: /* cmd: HELP CRLF  */
#line 408 "ftpcmd.y"
                {
			help (cmdtab, (char *) 0);
		}
#line 1928 "ftpcmd.c"
    break;

  case 29: /* cmd: HELP SP STRING CRLF  */
#line 412 "ftpcmd.y"
                {
			char *cp = (yyvsp[-1].s);

			if (strncasecmp (cp, "SITE", 4) == 0)
			  {
			    cp = (yyvsp[-1].s) + 4;
			    if (*cp == ' ')
			      cp++;
			    if (*cp)
			      help (sitetab, cp);
			    else
			      help (sitetab, (char *) 0);
			  }
			else
			  help (cmdtab, (yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 1950 "ftpcmd.c"
    break;

  case 30: /* cmd: NOOP CRLF  */
#line 430 "ftpcmd.y"
                {
			reply (200, "NOOP command successful.");
		}
#line 1958 "ftpcmd.c"
    break;

  case 31: /* cmd: MKD check_login SP pathname CRLF  */
#line 434 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  makedir ((yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 1968 "ftpcmd.c"
    break;

  case 32: /* cmd: RMD check_login SP pathname CRLF  */
#line 440 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  removedir ((yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 1978 "ftpcmd.c"
    break;

  case 33: /* cmd: PWD check_login CRLF  */
#line 446 "ftpcmd.y"
                {
			if ((yyvsp[-1].i))
			  pwd ();
		}
#line 1987 "ftpcmd.c"
    break;

  case 34: /* cmd: CDUP check_login CRLF  */
#line 451 "ftpcmd.y"
                {
			if ((yyvsp[-1].i))
			  cwd ("..");
		}
#line 1996 "ftpcmd.c"
    break;

  case 35: /* cmd: FEAT check_login CRLF  */
#line 456 "ftpcmd.y"
                {
			if ((yyvsp[-1].i))
			  {
			    char **name;

			    lreply (211, "Supported extensions:");
			    for (name = extlist; *name; name++)
			      printf (" %s\r\n", *name);
			    reply (211, "End");
			  }
		}
#line 2012 "ftpcmd.c"
    break;

  case 36: /* cmd: FEAT check_login SP STRING CRLF  */
#line 469 "ftpcmd.y"
                {
			if ((yyvsp[-3].i))
			  {
			    reply (501, "Not accepting arguments.");
			    free ((yyvsp[-1].s));
			  }
		}
#line 2024 "ftpcmd.c"
    break;

  case 37: /* cmd: OPTS check_login CRLF  */
#line 481 "ftpcmd.y"
                {
			if ((yyvsp[-1].i))
			  {
			    reply (501, "Must have an argument.");
			  }
		}
#line 2035 "ftpcmd.c"
    break;

  case 38: /* cmd: OPTS check_login SP STRING CRLF  */
#line 488 "ftpcmd.y"
                {
			if ((yyvsp[-3].i))
			  {
			    reply (501, "No options are available.");
			    free ((yyvsp[-1].s));
			  }
		}
#line 2047 "ftpcmd.c"
    break;

  case 39: /* cmd: SITE SP HELP CRLF  */
#line 496 "ftpcmd.y"
                {
			help (sitetab, (char *) 0);
		}
#line 2055 "ftpcmd.c"
    break;

  case 40: /* cmd: SITE SP HELP SP STRING CRLF  */
#line 500 "ftpcmd.y"
                {
			help (sitetab, (yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 2064 "ftpcmd.c"
    break;

  case 41: /* cmd: SITE SP UMASK check_login CRLF  */
#line 505 "ftpcmd.y"
                {
			int oldmask;

			if ((yyvsp[-1].i))
			  {
			    oldmask = umask (0);
			    umask (oldmask);
			    reply (200, "Current UMASK is %03o", oldmask);
			  }
		}
#line 2079 "ftpcmd.c"
    break;

  case 42: /* cmd: SITE SP UMASK check_login SP octal_number CRLF  */
#line 516 "ftpcmd.y"
                {
			int oldmask;

			if ((yyvsp[-3].i))
			  {
			    if (((yyvsp[-1].i) == -1) || ((yyvsp[-1].i) > 0777))
			      reply (501, "Bad UMASK value");
			    else
			      {
				oldmask = umask ((yyvsp[-1].i));
				reply (200, "UMASK set to %03o (was %03o)",
				      (yyvsp[-1].i), oldmask);
			      }
			  }
		}
#line 2099 "ftpcmd.c"
    break;

  case 43: /* cmd: SITE SP CHMOD check_login SP octal_number SP pathname CRLF  */
#line 532 "ftpcmd.y"
                {
			if ((yyvsp[-5].i) && ((yyvsp[-1].s) != NULL))
			  {
			    if ((yyvsp[-3].i) > 0777)
			      reply (501,
				     "CHMOD: Mode value must be between 0 and 0777");
			    else if (chmod ((yyvsp[-1].s), (yyvsp[-3].i)) < 0)
			      perror_reply (550, (yyvsp[-1].s));
			    else
			      reply (200, "CHMOD command successful.");
			  }
			free ((yyvsp[-1].s));
		}
#line 2117 "ftpcmd.c"
    break;

  case 44: /* cmd: SITE SP IDLE CRLF  */
#line 546 "ftpcmd.y"
                {
			reply (200,
			       "Current IDLE time limit is %d seconds; max %d",
			       timeout, maxtimeout);
		}
#line 2127 "ftpcmd.c"
    break;

  case 45: /* cmd: SITE SP IDLE check_login SP NUMBER CRLF  */
#line 552 "ftpcmd.y"
                {
			if ((yyvsp[-3].i))
			  {
			    if ((yyvsp[-1].i) < 30 || (yyvsp[-1].i) > maxtimeout)
			      reply (501,
				     "Maximum IDLE time must be between 30 and %d seconds",
				     maxtimeout);
			    else
			      {
				timeout = (yyvsp[-1].i);
				alarm ((unsigned) timeout);
				reply (200,
				       "Maximum IDLE time set to %d seconds",
				       timeout);
			      }
			  }
		}
#line 2149 "ftpcmd.c"
    break;

  case 46: /* cmd: STOU check_login SP pathname CRLF  */
#line 570 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  store ((yyvsp[-1].s), "w", 1);
			free ((yyvsp[-1].s));
		}
#line 2159 "ftpcmd.c"
    break;

  case 47: /* cmd: SYST CRLF  */
#line 576 "ftpcmd.y"
                {
		        const char *sys_type; /* Official rfc-defined os type.  */
			char *version = 0; /* A more specific type. */

#ifdef HAVE_UNAME
			struct utsname u;

			if (uname (&u) >= 0)
			  {
			    version = malloc (strlen (u.sysname) + 1
					      + strlen (u.release) + 1);
			    if (version)
			      sprintf (version, "%s %s", u.sysname, u.release);
			  }
#else /* !HAVE_UNAME */
# ifdef BSD
			version = "BSD";
# endif /* BSD */
#endif /* !HAVE_UNAME */

#if defined unix || defined __unix || defined __unix__
			sys_type = "UNIX";
#else
			sys_type = "UNKNOWN";
#endif

			if (!no_version && version)
			  reply (215, "%s Type: L%d Version: %s",
				 sys_type, NBBY, version);
			else
			  reply (215, "%s Type: L%d", sys_type, NBBY);

#ifdef HAVE_UNAME
			free (version);
#endif
		}
#line 2200 "ftpcmd.c"
    break;

  case 48: /* cmd: SIZE check_login SP pathname CRLF  */
#line 620 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  sizecmd ((yyvsp[-1].s));
			free ((yyvsp[-1].s));
		}
#line 2210 "ftpcmd.c"
    break;

  case 49: /* cmd: MDTM check_login SP pathname CRLF  */
#line 635 "ftpcmd.y"
                {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
			  {
			    struct stat stbuf;

			    if (stat ((yyvsp[-1].s), &stbuf) < 0)
			      reply (550, "%s: %s", (yyvsp[-1].s), strerror (errno));
			    else if (!S_ISREG (stbuf.st_mode))
			      reply (550, "%s: not a plain file.", (yyvsp[-1].s));
			    else
			      {
				struct tm *t;

				t = gmtime (&stbuf.st_mtime);
				reply (213,
				       "%04d%02d%02d%02d%02d%02d",
				       1900 + t->tm_year, t->tm_mon+1,
				       t->tm_mday, t->tm_hour,
				       t->tm_min, t->tm_sec);
			      }
			  }
			free ((yyvsp[-1].s));
		}
#line 2238 "ftpcmd.c"
    break;

  case 50: /* cmd: EPRT check_login SP CHAR net_proto CHAR net_addr CHAR tcp_port CHAR CRLF  */
#line 663 "ftpcmd.y"
                {
			usedefault = 0;
			if (pdata >= 0)
			  {
			    close (pdata);
			    pdata = -1;
			  }
			/* A first sanity check.  */
			if ((yyvsp[-9].i)				/* valid login */
			    && ((yyvsp[-6].i) > 0)			/* valid protocols */
			    && ((yyvsp[-7].i) > 32 && (yyvsp[-7].i) < 127)	/* legal first delimiter */
							/* identical delimiters */
			    && ((yyvsp[-7].i) == (yyvsp[-5].i) && (yyvsp[-7].i) == (yyvsp[-3].i) && (yyvsp[-7].i) == (yyvsp[-1].i)))
			  {
			    /* We only accept connections using
			     * the same address family as is
			     * currently in use, unless we
			     * detect IPv4-mapped-to-IPv6.
			     */
			    if (his_addr.ss_family == (yyvsp[-6].i)
				|| ((yyvsp[-6].i) == AF_INET6
				    && his_addr.ss_family == AF_INET)
				|| ((yyvsp[-6].i) == AF_INET
				    && his_addr.ss_family == AF_INET6))
			      {
				int err;
				char p[8];
				struct addrinfo hints, *res;

				memset (&hints, 0, sizeof (hints));
				snprintf (p, sizeof (p), "%jd", (yyvsp[-2].i) & 0xffff);
				hints.ai_family = (yyvsp[-6].i);
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;

				err = getaddrinfo ((yyvsp[-4].s), p, &hints, &res);
				if (err)
				  reply (500, "Illegal EPRT Command");
				else if (/* sanity check */
					 (his_addr.ss_family == AF_INET
					  && memcmp (&((struct sockaddr_in *) &his_addr)->sin_addr,
						     &((struct sockaddr_in *) res->ai_addr)->sin_addr,
						     sizeof (struct in_addr))
					     == 0
					  && ntohs (((struct sockaddr_in *) res->ai_addr)->sin_port)
					     > IPPORT_RESERVED
					 )
					 ||
					 (his_addr.ss_family == AF_INET6
					  && memcmp (&((struct sockaddr_in6 *) &his_addr)->sin6_addr,
						     &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr,
						     sizeof (struct in6_addr))
					     == 0
					  && ntohs (((struct sockaddr_in6 *) res->ai_addr)->sin6_port)
					     > IPPORT_RESERVED
					 )
					 ||
					 (his_addr.ss_family == AF_INET
					  && res->ai_family == AF_INET6
					  && IN6_IS_ADDR_V4MAPPED (&((struct sockaddr_in6 *) res->ai_addr)->sin6_addr)
					  && memcmp (&((struct sockaddr_in *) &his_addr)->sin_addr,
						     &((struct in_addr *) &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr)[3],
						     sizeof (struct in_addr))
					     == 0
					  && ntohs (((struct sockaddr_in6 *) res->ai_addr)->sin6_port)
					     > IPPORT_RESERVED
					 )
					 ||
					 (his_addr.ss_family == AF_INET6
					  && res->ai_family == AF_INET
					  && IN6_IS_ADDR_V4MAPPED (&((struct sockaddr_in6 *) &his_addr)->sin6_addr)
					  && memcmp (&((struct in_addr *) &((struct sockaddr_in6 *) &his_addr)->sin6_addr)[3],
						     &((struct sockaddr_in *) res->ai_addr)->sin_addr,
						     sizeof (struct in_addr))
					     == 0
					  && ntohs (((struct sockaddr_in *) res->ai_addr)->sin_port)
					     > IPPORT_RESERVED
					 )
					)
				  {
				    /* In the case of IPv4 mapped as IPv6,
				     * the addresses were proven to coincide,
				     * only the extraction remains.
				     * Since non-mapped is the standard,
				     * test that situation first.
				     */
				    if (his_addr.ss_family == res->ai_family)
				      {
					memcpy (&data_dest, res->ai_addr,
						res->ai_addrlen);
					data_dest_len = res->ai_addrlen;
				      }
				    else if (his_addr.ss_family == AF_INET
					     && res->ai_family == AF_INET6)
				      {
					/* `his_addr' contains the reduced
					 * IPv4 address.
					 */
					memcpy (&data_dest, &his_addr,
						sizeof (struct sockaddr_in));
					data_dest_len =
					  sizeof (struct sockaddr_in);
					((struct sockaddr_in *) &data_dest)->sin_port =
					  ((struct sockaddr_in6 *) res->ai_addr)->sin6_port;
				      }
				    else
				      {
					/* `res->ai_addr' contains the reduced
					 * IPv4 address, but the connection
					 * stands on `his_addr', which is
					 * an IPv4-to-IPv6-mapped address.
					 */
					memcpy (&data_dest, &his_addr,
						sizeof (struct sockaddr_in6));
					data_dest_len =
					  sizeof (struct sockaddr_in6);
					((struct sockaddr_in6 *) &data_dest)->sin6_port =
					  ((struct sockaddr_in *) res->ai_addr)->sin_port;
				      }

				    freeaddrinfo (res);
				    reply (200, "EPRT command successful.");
				  }
				else
				  {
				    /* failed identity check */
				    if (res)
				      freeaddrinfo (res);
				    reply (500, "Illegal EPRT Command");
				  }
			      }
			    else
			      /* Not fit for established connection.  */
			      reply (522,
				     "Network protocol not supported, use (%d)",
				     ((yyvsp[-6].i) == 1) ? 2 : 1);
			  }
			else if ((yyvsp[-9].i) && ((yyvsp[-6].i) <= 0))
			    reply (522,
				   "Network protocol not supported, use (1,2)");
			else if ((yyvsp[-9].i))
			  /* Incorrect delimiters detected,
			   * the other conditions are met.
			   */
			  reply (500, "Illegal EPRT Command");
		}
#line 2389 "ftpcmd.c"
    break;

  case 51: /* cmd: EPSV check_login CRLF  */
#line 814 "ftpcmd.y"
                {
			if ((yyvsp[-1].i))
			  passive (PASSIVE_EPSV, AF_UNSPEC);
		}
#line 2398 "ftpcmd.c"
    break;

  case 52: /* cmd: EPSV check_login SP net_proto CRLF  */
#line 819 "ftpcmd.y"
                {
			if ((yyvsp[-3].i))
			  {
			    if ((yyvsp[-1].i) > 0)
			      passive (PASSIVE_EPSV, (yyvsp[-1].i));
			    else
			      reply (522,
				     "Network protocol not supported, use (1,2)");
			  }
		}
#line 2413 "ftpcmd.c"
    break;

  case 53: /* cmd: LPRT check_login SP long_host_port CRLF  */
#line 834 "ftpcmd.y"
                {
			if ((yyvsp[-3].i))
			  {
			    if ((yyvsp[-1].i) &&
				((his_addr.ss_family == AF_INET
				  && memcmp (&((struct sockaddr_in *) &his_addr)->sin_addr,
					     &((struct sockaddr_in *) &data_dest)->sin_addr,
					     sizeof (struct in_addr)) == 0
				  && ntohs (((struct sockaddr_in *) &data_dest)->sin_port)
					> IPPORT_RESERVED)
				 ||
				 (his_addr.ss_family == AF_INET6
				  && memcmp (&((struct sockaddr_in6 *) &his_addr)->sin6_addr,
					     &((struct sockaddr_in6 *) &data_dest)->sin6_addr,
					     sizeof (struct in6_addr)) == 0
				  && ntohs (((struct sockaddr_in6 *) &data_dest)->sin6_port)
					> IPPORT_RESERVED)
				)
			       )
			      {
				usedefault = 0;
				if (pdata >= 0)
				  {
				    close (pdata);
				    pdata = -1;
				  }
				  reply (200, "LPRT command successful.");
			      }
			    else
			      {
				usedefault = 1;
				memset (&data_dest, 0, sizeof (data_dest));
				reply (500, "Illegal LPRT Command");
			      }
			  } /* check_login */
		}
#line 2454 "ftpcmd.c"
    break;

  case 54: /* cmd: LPSV check_login CRLF  */
#line 875 "ftpcmd.y"
                {
			if ((yyvsp[-1].i))
			  passive (PASSIVE_LPSV, 0 /* not used */);
		}
#line 2463 "ftpcmd.c"
    break;

  case 55: /* cmd: QUIT CRLF  */
#line 881 "ftpcmd.y"
                {
			reply (221, "Goodbye.");
			dologout (0);
		}
#line 2472 "ftpcmd.c"
    break;

  case 56: /* cmd: error CRLF  */
#line 886 "ftpcmd.y"
                {
			yyerrok;
		}
#line 2480 "ftpcmd.c"
    break;

  case 57: /* rcmd: RNFR check_login SP pathname CRLF  */
#line 892 "ftpcmd.y"
                {
			restart_point = (off_t) 0;
			if ((yyvsp[-3].i) && (yyvsp[-1].s))
			  {
			    free (fromname);
			    fromname = renamefrom ((yyvsp[-1].s));
			  }
			if (fromname == (char *) 0 && (yyvsp[-1].s))
			  free ((yyvsp[-1].s));
		}
#line 2495 "ftpcmd.c"
    break;

  case 58: /* rcmd: REST SP byte_size CRLF  */
#line 907 "ftpcmd.y"
                {
		        free (fromname);
			fromname = (char *) 0;
			restart_point = (yyvsp[-1].i);
			reply (350, "Restarting at %jd. %s",
			       (intmax_t) restart_point,
			       "Send STORE or RETRIEVE to initiate transfer.");
		}
#line 2508 "ftpcmd.c"
    break;

  case 60: /* password: %empty  */
#line 923 "ftpcmd.y"
                {
			(yyval.s) = (char *) calloc (1, sizeof (char));
		}
#line 2516 "ftpcmd.c"
    break;

  case 63: /* net_proto: NUMBER  */
#line 935 "ftpcmd.y"
                {
			/* Rewrite as valid address family.  */
			if ((yyvsp[0].i) == 1)
			  (yyval.i) = AF_INET;
			else if ((yyvsp[0].i) == 2)
			  (yyval.i) = AF_INET6;
			else
			  (yyval.i) = -1;	/* Invalid protocol.  */
		}
#line 2530 "ftpcmd.c"
    break;

  case 66: /* host_port: NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER  */
#line 957 "ftpcmd.y"
                {
			int err;
			char a[INET6_ADDRSTRLEN], p[8];
			struct addrinfo hints, *res;

			snprintf (a, sizeof (a), "%jd.%jd.%jd.%jd",
				  (yyvsp[-10].i) & 0xff, (yyvsp[-8].i) & 0xff,
				  (yyvsp[-6].i) & 0xff, (yyvsp[-4].i) & 0xff);
			snprintf (p, sizeof (p), "%jd",
				  (((yyvsp[-2].i) & 0xff) << 8) + ((yyvsp[0].i) & 0xff));
			memset (&hints, 0, sizeof (hints));
			hints.ai_family = his_addr.ss_family;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;

			if (his_addr.ss_family == AF_INET6)
			  {
			    /* IPv4 mapped to IPv6.  */
			    hints.ai_family = AF_INET6;
#ifdef AI_V4MAPPED
			    hints.ai_flags |= AI_V4MAPPED;
#endif
			    snprintf (a, sizeof (a),
				      "::ffff:%jd.%jd.%jd.%jd",
				      (yyvsp[-10].i) & 0xff, (yyvsp[-8].i) & 0xff,
				      (yyvsp[-6].i) & 0xff, (yyvsp[-4].i) & 0xff);
			  }

			err = getaddrinfo (a, p, &hints, &res);
			if (err)
			  {
			    reply (550, "Address failure: %s,%s", a, p);
			    memset (&data_dest, 0, sizeof (data_dest));
			    data_dest_len = 0;
			    (yyval.i) = 0;
			  }
			else
			  {
			    memcpy (&data_dest, res->ai_addr, res->ai_addrlen);
			    data_dest_len = res->ai_addrlen;
			    freeaddrinfo (res);
			    (yyval.i) = 1;
			  }
		}
#line 2579 "ftpcmd.c"
    break;

  case 67: /* long_host_port: NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER  */
#line 1007 "ftpcmd.y"
                {
			int err;
			char a[INET6_ADDRSTRLEN], p[8];
			struct addrinfo hints, *res;

			/* Well formed input for IPv4?  */
			if ((yyvsp[-16].i) != 4 || (yyvsp[-14].i) != 4 || (yyvsp[-4].i) != 2
			    || (yyvsp[-12].i) < 0 || (yyvsp[-12].i) > 255 || (yyvsp[-10].i) < 0 || (yyvsp[-10].i) > 255
			    || (yyvsp[-8].i) < 0 || (yyvsp[-8].i) > 255 || (yyvsp[-6].i) < 0 || (yyvsp[-6].i) > 255
			    || (yyvsp[-2].i) < 0 || (yyvsp[-2].i) > 255
			    || (yyvsp[0].i) < 0 || (yyvsp[0].i) > 255)
			  {
			    reply (500, "Invalid address.");
			    memset (&data_dest, 0, sizeof (data_dest));
			    data_dest_len = 0;
			    (yyval.i) = 0;
			  }
			else
			  {
			    snprintf (a, sizeof (a), "%jd.%jd.%jd.%jd",
				      (yyvsp[-12].i), (yyvsp[-10].i), (yyvsp[-8].i), (yyvsp[-6].i));
			    snprintf (p, sizeof (p), "%jd", ((yyvsp[-2].i) << 8) + (yyvsp[0].i));

			    memset (&hints, 0, sizeof (hints));
			    hints.ai_family = his_addr.ss_family;
			    hints.ai_socktype = SOCK_STREAM;
			    hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;

			    if (his_addr.ss_family == AF_INET6)
			      {
				/* IPv4 mapped to IPv6.  */
				hints.ai_family = AF_INET6;
#ifdef AI_V4MAPPED
				hints.ai_flags |= AI_V4MAPPED;
#endif
				snprintf (a, sizeof (a),
					  "::ffff:%jd.%jd.%jd.%jd",
					  (yyvsp[-12].i), (yyvsp[-10].i), (yyvsp[-8].i), (yyvsp[-6].i));
			      }

			    err = getaddrinfo (a, p, &hints, &res);
			    if (err)
			      {
				reply (550, "LPRT address failure: %s,%s",
				       a, p);
				memset (&data_dest, 0, sizeof (data_dest));
				data_dest_len = 0;
				(yyval.i) = 0;
			      }
			    else
			      {
				memcpy (&data_dest, res->ai_addr,
					res->ai_addrlen);
				data_dest_len = res->ai_addrlen;
				freeaddrinfo (res);
				(yyval.i) = 1;
			      }
			  }
		}
#line 2643 "ftpcmd.c"
    break;

  case 68: /* long_host_port: NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER  */
#line 1072 "ftpcmd.y"
                {
			int err;
			char a[INET6_ADDRSTRLEN], p[8];
			struct addrinfo hints, *res;

			/* Well formed input for IPv6?  */
			if ((yyvsp[-40].i) != 6 || (yyvsp[-38].i) != 16 || (yyvsp[-4].i) != 2
			    || (yyvsp[-36].i) < 0 || (yyvsp[-36].i) > 255 || (yyvsp[-34].i) < 0 || (yyvsp[-34].i) > 255
			    || (yyvsp[-32].i) < 0 || (yyvsp[-32].i) > 255 || (yyvsp[-30].i) < 0 || (yyvsp[-30].i) > 255
			    || (yyvsp[-28].i) < 0 || (yyvsp[-28].i) > 255 || (yyvsp[-26].i) < 0 || (yyvsp[-26].i) > 255
			    || (yyvsp[-24].i) < 0 || (yyvsp[-24].i) > 255 || (yyvsp[-22].i) < 0 || (yyvsp[-22].i) > 255
			    || (yyvsp[-20].i) < 0 || (yyvsp[-20].i) > 255 || (yyvsp[-18].i) < 0 || (yyvsp[-18].i) > 255
			    || (yyvsp[-16].i) < 0 || (yyvsp[-16].i) > 255 || (yyvsp[-14].i) < 0 || (yyvsp[-14].i) > 255
			    || (yyvsp[-12].i) < 0 || (yyvsp[-12].i) > 255 || (yyvsp[-10].i) < 0 || (yyvsp[-10].i) > 255
			    || (yyvsp[-8].i) < 0 || (yyvsp[-8].i) > 255 || (yyvsp[-6].i) < 0 || (yyvsp[-6].i) > 255
			    || (yyvsp[-2].i) < 0 || (yyvsp[-2].i) > 255 || (yyvsp[0].i) < 0 || (yyvsp[0].i) > 255)
			  {
			    reply (500, "Invalid address.");
			    memset (&data_dest, 0, sizeof (data_dest));
			    data_dest_len = 0;
			    (yyval.i) = 0;
			  }
			else
			  {
			    snprintf (a, sizeof (a),
				     "%02jx%02jx:%02jx%02jx:"
				     "%02jx%02jx:%02jx%02jx:"
				     "%02jx%02jx:%02jx%02jx:"
				     "%02jx%02jx:%02jx%02jx",
				      (yyvsp[-36].i), (yyvsp[-34].i), (yyvsp[-32].i), (yyvsp[-30].i),
				      (yyvsp[-28].i), (yyvsp[-26].i), (yyvsp[-24].i), (yyvsp[-22].i),
				      (yyvsp[-20].i), (yyvsp[-18].i), (yyvsp[-16].i), (yyvsp[-14].i),
				      (yyvsp[-12].i), (yyvsp[-10].i), (yyvsp[-8].i), (yyvsp[-6].i));
			    snprintf (p, sizeof (p), "%jd",
				      ((yyvsp[-2].i) << 8) + (yyvsp[0].i));

			    memset (&hints, 0, sizeof (hints));
			    hints.ai_family = his_addr.ss_family;
			    hints.ai_socktype = SOCK_STREAM;
			    hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;

			    err = getaddrinfo (a, p, &hints, &res);
			    if (err)
			      {
				reply (550, "LPRT address failure: %s,%s",
				       a, p);
				memset (&data_dest, 0, sizeof (data_dest));
				data_dest_len = 0;
				(yyval.i) = 0;
			      }
			    else
			      {
				memcpy (&data_dest, res->ai_addr,
					res->ai_addrlen);
				data_dest_len = res->ai_addrlen;
				freeaddrinfo (res);
				(yyval.i) = 1;
			      }
			  }
		}
#line 2708 "ftpcmd.c"
    break;

  case 69: /* form_code: N  */
#line 1136 "ftpcmd.y"
                {
			(yyval.i) = FORM_N;
		}
#line 2716 "ftpcmd.c"
    break;

  case 70: /* form_code: T  */
#line 1140 "ftpcmd.y"
                {
			(yyval.i) = FORM_T;
		}
#line 2724 "ftpcmd.c"
    break;

  case 71: /* form_code: C  */
#line 1144 "ftpcmd.y"
                {
			(yyval.i) = FORM_C;
		}
#line 2732 "ftpcmd.c"
    break;

  case 72: /* type_code: A  */
#line 1151 "ftpcmd.y"
                {
			cmd_type = TYPE_A;
			cmd_form = FORM_N;
		}
#line 2741 "ftpcmd.c"
    break;

  case 73: /* type_code: A SP form_code  */
#line 1156 "ftpcmd.y"
                {
			cmd_type = TYPE_A;
			cmd_form = (yyvsp[0].i);
		}
#line 2750 "ftpcmd.c"
    break;

  case 74: /* type_code: E  */
#line 1161 "ftpcmd.y"
                {
			cmd_type = TYPE_E;
			cmd_form = FORM_N;
		}
#line 2759 "ftpcmd.c"
    break;

  case 75: /* type_code: E SP form_code  */
#line 1166 "ftpcmd.y"
                {
			cmd_type = TYPE_E;
			cmd_form = (yyvsp[0].i);
		}
#line 2768 "ftpcmd.c"
    break;

  case 76: /* type_code: I  */
#line 1171 "ftpcmd.y"
                {
			cmd_type = TYPE_I;
		}
#line 2776 "ftpcmd.c"
    break;

  case 77: /* type_code: L  */
#line 1175 "ftpcmd.y"
                {
			cmd_type = TYPE_L;
			cmd_bytesz = NBBY;
		}
#line 2785 "ftpcmd.c"
    break;

  case 78: /* type_code: L SP byte_size  */
#line 1180 "ftpcmd.y"
                {
			cmd_type = TYPE_L;
			cmd_bytesz = (yyvsp[0].i);
		}
#line 2794 "ftpcmd.c"
    break;

  case 79: /* type_code: L byte_size  */
#line 1186 "ftpcmd.y"
                {
			cmd_type = TYPE_L;
			cmd_bytesz = (yyvsp[0].i);
		}
#line 2803 "ftpcmd.c"
    break;

  case 80: /* struct_code: F  */
#line 1194 "ftpcmd.y"
                {
			(yyval.i) = STRU_F;
		}
#line 2811 "ftpcmd.c"
    break;

  case 81: /* struct_code: R  */
#line 1198 "ftpcmd.y"
                {
			(yyval.i) = STRU_R;
		}
#line 2819 "ftpcmd.c"
    break;

  case 82: /* struct_code: P  */
#line 1202 "ftpcmd.y"
                {
			(yyval.i) = STRU_P;
		}
#line 2827 "ftpcmd.c"
    break;

  case 83: /* mode_code: S  */
#line 1209 "ftpcmd.y"
                {
			(yyval.i) = MODE_S;
		}
#line 2835 "ftpcmd.c"
    break;

  case 84: /* mode_code: B  */
#line 1213 "ftpcmd.y"
                {
			(yyval.i) = MODE_B;
		}
#line 2843 "ftpcmd.c"
    break;

  case 85: /* mode_code: C  */
#line 1217 "ftpcmd.y"
                {
			(yyval.i) = MODE_C;
		}
#line 2851 "ftpcmd.c"
    break;

  case 86: /* pathname: pathstring  */
#line 1224 "ftpcmd.y"
                {
			/*
			 * Problem: this production is used for all pathname
			 * processing, but only gives a 550 error reply.
			 * This is a valid reply in some cases but not in others.
			 */
			if (cred.logged_in && (yyvsp[0].s) && *(yyvsp[0].s) == '~')
			  {
			    glob_t gl;
			    int flags = GLOB_NOCHECK;

#ifdef GLOB_BRACE
			    flags |= GLOB_BRACE;
#endif
#ifdef GLOB_QUOTE
			    flags |= GLOB_QUOTE;
#endif
#ifdef GLOB_TILDE
			    flags |= GLOB_TILDE;
#endif

			    memset (&gl, 0, sizeof (gl));
			    if (glob ((yyvsp[0].s), flags, NULL, &gl)
				|| gl.gl_pathc == 0)
			      {
				reply (550, "not found");
				(yyval.s) = NULL;
			      }
			    else
			      (yyval.s) = strdup (gl.gl_pathv[0]);

			    globfree (&gl);
			    free ((yyvsp[0].s));
			  }
			else
			  (yyval.s) = (yyvsp[0].s);
		}
#line 2893 "ftpcmd.c"
    break;

  case 88: /* octal_number: NUMBER  */
#line 1269 "ftpcmd.y"
                {
			int ret, dec, multby, digit;

			/*
			 * Convert a number that was read as decimal number
			 * to what it would be if it had been read as octal.
			 */
			dec = (yyvsp[0].i);
			multby = 1;
			ret = 0;
			while (dec)
			  {
			    digit = dec % 10;
			    if (digit > 7)
			      {
				ret = -1;
				break;
			      }
			    ret += digit * multby;
			    multby *= 8;
			    dec /= 10;
			  }
			(yyval.i) = ret;
		}
#line 2922 "ftpcmd.c"
    break;

  case 89: /* check_login: %empty  */
#line 1297 "ftpcmd.y"
                {
			if (cred.logged_in)
			  (yyval.i) = 1;
			else
			  {
			    reply (530, "Please login with USER and PASS.");
			    (yyval.i) = 0;
			  }
		}
#line 2936 "ftpcmd.c"
    break;


#line 2940 "ftpcmd.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1308 "ftpcmd.y"


#define	CMD	0	/* beginning of command */
#define	ARGS	1	/* expect miscellaneous arguments */
#define	STR1	2	/* expect SP followed by STRING */
#define	STR2	3	/* expect STRING (must be STR2 + 1)*/
#define	OSTR	4	/* optional SP then STRING */
#define	ZSTR1	5	/* SP then optional STRING */
#define	ZSTR2	6	/* optional STRING after SP (must be ZSTR1 + 1) */
#define	SITECMD	7	/* SITE command */
#define	NSTR	8	/* Number followed by a string */
#define	DLIST	9	/* SP and delimited list for EPRT/EPSV */

static struct tab cmdtab[] = {
  /* In the order defined by RFC 959.  See also RFC 1123.  */
  /* Access control commands.  */
  { "USER", USER, STR1, 1,	"<sp> username" },
  { "PASS", PASS, ZSTR1, 1,	"<sp> password" },
  { "ACCT", ACCT, STR1, 0,	"(specify account)" },
  { "CWD",  CWD,  OSTR, 1,	"[ <sp> directory-name ]" },
  { "CDUP", CDUP, ARGS, 1,	"(change to parent directory)" },
  { "SMNT", SMNT, ARGS, 0,	"(structure mount)" },
  { "REIN", REIN, ARGS, 0,	"(reinitialize server state)" },
  { "QUIT", QUIT, ARGS, 1,	"(terminate service)", },
  /* Transfer parameter commands.  */
  { "PORT", PORT, ARGS, 1,	"<sp> b0, b1, b2, b3, b4" },
  { "PASV", PASV, ARGS, 1,	"(set server in passive mode)" },
  { "TYPE", TYPE, ARGS, 1,	"<sp> [ A | E | I | L ]" },
  { "STRU", STRU, ARGS, 1,	"(specify file structure)" },
  { "MODE", MODE, ARGS, 1,	"(specify transfer mode)" },
  /* FTP service commands.  */
  { "RETR", RETR, STR1, 1,	"<sp> file-name" },
  { "STOR", STOR, STR1, 1,	"<sp> file-name" },
  { "STOU", STOU, STR1, 1,	"<sp> file-name" },
  { "APPE", APPE, STR1, 1,	"<sp> file-name" },
  { "ALLO", ALLO, ARGS, 1,	"allocate storage (vacuously)" },
  { "REST", REST, ARGS, 1,	"<sp> offset (restart command)" },
  { "RNFR", RNFR, STR1, 1,	"<sp> file-name" },
  { "RNTO", RNTO, STR1, 1,	"<sp> file-name" },
  { "ABOR", ABOR, ARGS, 1,	"(abort operation)" },
  { "DELE", DELE, STR1, 1,	"<sp> file-name" },
  { "RMD",  RMD,  STR1, 1,	"<sp> path-name" },
  { "MKD",  MKD,  STR1, 1,	"<sp> path-name" },
  { "PWD",  PWD,  ARGS, 1,	"(return current directory)" },
  { "LIST", LIST, OSTR, 1,	"[ <sp> path-name ]" },
  { "NLST", NLST, OSTR, 1,	"[ <sp> path-name ]" },
  { "SITE", SITE, SITECMD, 1,	"site-cmd [ <sp> arguments ]" },
  { "SYST", SYST, ARGS, 1,	"(get type of operating system)" },
  { "STAT", STAT, OSTR, 1,	"[ <sp> path-name ]" },
  { "HELP", HELP, OSTR, 1,	"[ <sp> <string> ]" },
  { "NOOP", NOOP, ARGS, 1,	"" },
  /* Experimental commands, as mentioned in RFC 1123.  Now obsolete.  */
  { "XMKD", MKD,  STR1, 1,	"<sp> path-name" },
  { "XRMD", RMD,  STR1, 1,	"<sp> path-name" },
  { "XPWD", PWD,  ARGS, 1,	"(return current directory)" },
  { "XCUP", CDUP, ARGS, 1,	"(change to parent directory)" },
  { "XCWD", CWD,  OSTR, 1,	"[ <sp> directory-name ]" },
  /* Commands in RFC 2389.  */
  { "FEAT", FEAT, OSTR, 1,	"(display command extensions)" },
  /* XXX: Replace OSTR once some functionality exists.  */
  { "OPTS", OPTS, OSTR, 1,	"<sp> cmd-name [ <sp> options ]" },
  /* Commands in RFC 3659.  */
  { "SIZE", SIZE, OSTR, 1,	"<sp> path-name" },
  { "MDTM", MDTM, OSTR, 1,	"<sp> path-name" },
  /* Unimplemented, but reserved in RFC ???.  */
  { "MLFL", MLFL, OSTR, 0,	"(mail file)" },
  { "MAIL", MAIL, OSTR, 0,	"(mail to user)" },
  { "MSND", MSND, OSTR, 0,	"(mail send to terminal)" },
  { "MSOM", MSOM, OSTR, 0,	"(mail send to terminal or mailbox)" },
  { "MSAM", MSAM, OSTR, 0,	"(mail send to terminal and mailbox)" },
  { "MRSQ", MRSQ, OSTR, 0,	"(mail recipient scheme question)" },
  { "MRCP", MRCP, STR1, 0,	"(mail recipient)" },
  /* Extended addressing in RFC 2428.  */
  { "EPRT", EPRT, DLIST, 1,	"<sp> <d> proto <d> addr <d> port <d>" },
  { "EPSV", EPSV, ARGS, 1,	"[ <sp> af ]" },
  /* Long addressing in RFC 1639.  Obsoleted in RFC 5797.  */
  { "LPRT", LPRT, ARGS, 1,	"<sp> af,hal,h0..hn,2,p0,p1" },
  { "LPSV", LPSV, ARGS, 1,	"(set server in long passive mode)" },
  /* Security extensions in RFC 2228.  */
  { "ADAT", ADAT, OSTR, 0,	"<sp> security-data" },
  { "AUTH", AUTH, OSTR, 0,	"<sp> mechanism" },
  { "CCC", CCC, ARGS, 0,	"(clear command channel)" },
  { "CONF", CONF, OSTR, 0,	"<sp> confidential-msg" },
  { "ENC", ENC, OSTR, 0,	"<sp> private-message" },
  { "MIC", MIC, OSTR, 0,	"<sp> safe-message" },
  { "PBSZ", PBSZ, OSTR, 0,	"<sp> buf-size" },
  { "PROT", PROT, OSTR, 0,	"<sp> char" },
  /* End of list.  */
  { NULL,   0,    0,    0,	NULL }
};

static struct tab sitetab[] = {
  { "CHMOD", CHMOD, NSTR, 1,	"<sp> mode <sp> file-name" },
  { "HELP", HELP, OSTR, 1,	"[ <sp> <string> ]" },
  { "IDLE", IDLE, ARGS, 1,	"[ <sp> maximum-idle-time ]" },
  { "UMASK", UMASK, ARGS, 1,	"[ <sp> umask ]" },
  { NULL,   0,    0,    0,	NULL }
};

/* Extensions beyond RFC 959 and RFC 2389.  Ordered as implemented.  */
static char *extlist[] = {
  "MDTM", "SIZE", "REST STREAM",
  "EPRT", "EPSV", "LPRT", "LPSV",
  NULL };

static struct tab *
lookup (struct tab *p, char *cmd)
{
  for (; p->name != NULL; p++)
    if (strcmp (cmd, p->name) == 0)
      return (p);
  return (0);
}

#include <arpa/telnet.h>

/*
 * getline - a hacked up version of fgets to ignore TELNET escape codes.
 */
char *
telnet_fgets (char *s, int n, FILE *iop)
{
  int c;
  register char *cs;

  cs = s;
/* tmpline may contain saved command from urgent mode interruption */
  for (c = 0; tmpline[c] != '\0' && --n > 0; ++c)
    {
      *cs++ = tmpline[c];
      if (tmpline[c] == '\n')
	{
	  *cs++ = '\0';
	  if (debug)
	    syslog (LOG_DEBUG, "command: %s", s);
	  tmpline[0] = '\0';
	  return (s);
	}

      if (c == 0)
	tmpline[0] = '\0';
    }

  while ((c = getc (iop)) != EOF)
    {
      c &= 0377;
      if (c == IAC)
	{
	  c = getc (iop);
	  if (c != EOF)
	    {
	      c &= 0377;
	      switch (c)
		{
		case WILL:
		case WONT:
		  c = getc (iop);
		  printf ("%c%c%c", IAC, DONT, 0377 & c);
		  fflush (stdout);
		  continue;

		case DO:
		case DONT:
		  c = getc (iop);
		  printf ("%c%c%c", IAC, WONT, 0377 & c);
		  fflush (stdout);
		  continue;

		case IAC:
		  break;

		default:
		  continue;	/* ignore command */
		}
	    }
	}

      *cs++ = c;
      if (--n <= 0 || c == '\n')
	break;
    }

  if (c == EOF && cs == s)
    return (NULL);

  *cs++ = '\0';

  if (debug)
    {
      if (!cred.guest && strncasecmp ("pass ", s, 5) == 0)
	{
	  /* Don't syslog passwords.  */
	  syslog (LOG_DEBUG, "command: %.5s ???", s);
	}
      else
	{
	  register char *cp;
	  register int len;

	  /* Don't syslog trailing CR-LF.  */
	  len = strlen (s);
	  cp = s + len - 1;

	  while (cp >= s && (*cp == '\n' || *cp == '\r'))
	    {
	      --cp;
	      --len;
	    }

	  syslog (LOG_DEBUG, "command: %.*s", len, s);
	}
    }
  return (s);
}

void
toolong (int signo)
{
  (void) signo;
  reply (421, "Timeout (%d seconds): closing control connection.",
	 timeout);
  if (logging)
    syslog (LOG_INFO, "User %s timed out after %d seconds",
	    (cred.name ? cred.name : "unknown"), timeout);
  dologout (1);
}

static int
yylex (void)
{
  static int cpos, state;
  char *cp, *cp2;
  struct tab *p;
  int n;
  char c;

  for (;;)
    {
      switch (state)
	{
	case CMD:
	  signal (SIGALRM, toolong);
	  alarm ((unsigned) timeout);
	  if (telnet_fgets (cbuf, sizeof (cbuf)-1, stdin) == NULL)
	    {
	      reply (221, "You could at least say goodbye.");
	      dologout (0);
	    }
	  alarm (0);

#ifdef HAVE_SETPROCTITLE
	  if (strncasecmp (cbuf, "PASS", 4) != 0)
	    setproctitle ("%s: %s", proctitle, cbuf);
#endif /* HAVE_SETPROCTITLE */

	  cp = strchr (cbuf, '\r');
	  if (cp)
	    {
	      *cp++ = '\n';
	      *cp = '\0';
	    }

	  cp = strpbrk (cbuf, " \n");
	  if (cp)
	    cpos = cp - cbuf;

	  if (cpos == 0)
	    cpos = 4;

	  c = cbuf[cpos];
	  cbuf[cpos] = '\0';
	  upper (cbuf);
	  p = lookup (cmdtab, cbuf);
	  cbuf[cpos] = c;

	  if (p != 0)
	    {
	      if (p->implemented == 0)
		{
		  nack (p->name);
		  longjmp (errcatch, 0);
		  /* NOTREACHED */
		}
	      state = p->state;
	      yylval.s = (char*) p->name;
	      return (p->token);
	    }
	  break;	/* Command not known.  */

	case SITECMD:
	  if (cbuf[cpos] == ' ')
	    {
	      cpos++;
	      return (SP);
	    }
	  cp = &cbuf[cpos];

	  cp2 = strpbrk (cp, " \n");
	  if (cp2)
	    cpos = cp2 - cbuf;

	  c = cbuf[cpos];
	  cbuf[cpos] = '\0';
	  upper (cp);
	  p = lookup (sitetab, cp);
	  cbuf[cpos] = c;

	  if (p != 0)
	    {
	      if (p->implemented == 0)
		{
		  state = CMD;
		  nack (p->name);
		  longjmp (errcatch, 0);
		  /* NOTREACHED */
		}

	      state = p->state;
	      yylval.s = (char*) p->name;
	      return (p->token);
	    }
	  state = CMD;
	  break;	/* Command not known.  */

	case OSTR:
	  if (cbuf[cpos] == '\n')
	    {
	      state = CMD;
	      return (CRLF);
	    }

	case STR1:
	case ZSTR1:
	dostr1:
	  if (cbuf[cpos] == ' ')
	    {
	      cpos++;
	      if (state == OSTR)
		state = STR2;
	      else
		++state;

	      return (SP);
	    }
	  /* Intentional continuation.  */

	case ZSTR2:
	  if (cbuf[cpos] == '\n')
	    {
	      state = CMD;
	      return (CRLF);
	    }

	case STR2:
	  cp = &cbuf[cpos];
	  n = strlen (cp);
	  cpos += n - 1;
	  /*
	   * Make sure the string is nonempty and newline terminated.
	   */
	  if (n > 1 && cbuf[cpos] == '\n')
	    {
	      cbuf[cpos] = '\0';
	      yylval.s = copy (cp);
	      cbuf[cpos] = '\n';
	      state = ARGS;
	      return (STRING);
	    }
	  break;	/* Empty string, missing NL.  */

	case NSTR:
	  if (cbuf[cpos] == ' ')
	    {
	      cpos++;
	      return (SP);
	    }
	  if (isdigit (cbuf[cpos]))
	    {
	      cp = &cbuf[cpos];
	      while (isdigit (cbuf[++cpos]))
		;

	      c = cbuf[cpos];
	      cbuf[cpos] = '\0';
	      yylval.i = atoi (cp);
	      cbuf[cpos] = c;
	      state = STR1;
	      return (NUMBER);
	    }
	  state = STR1;
	  goto dostr1;

	case DLIST:
	  /* Either numerical strings or
	   * address strings for IPv4 and IPv6.
	   * The consist of hexadecimal chars,
	   * colon and periods.  A period can
	   * not begin a valid address.  */
	  if (isxdigit (cbuf[cpos]) || cbuf[cpos] == ':')
	    {
	      int is_num = 1;	/* Only to turn off.  */

	      cp = &cbuf[cpos];
	      while (isxdigit (cbuf[cpos])
		     || cbuf[cpos] == ':'
		     || cbuf[cpos] == '.')
		{
		  if (!isdigit (cbuf[cpos]))
		    is_num = 0;
		  cpos++;
		}

	      c = cbuf[cpos];
	      cbuf[cpos] = '\0';
	      if (is_num)
		{
		  yylval.i = atoi (cp);
		  cbuf[cpos] = c;
		  return (NUMBER);
		}
	      else
		{
		  yylval.s = copy (cp);
		  cbuf[cpos] = c;
		  return (STRING);
		}
	    }

	  c = cbuf[cpos++];
	  switch (c)
	    {
	    case ' ':
	      return (SP);

	    case '\n':
	      state = CMD;
	      return (CRLF);

	    default:
	      yylval.i = c;
	      return (CHAR);
	    }
	  break;	/* Not reachable.  */

	case ARGS:
	  if (isdigit (cbuf[cpos]))
	    {
	      cp = &cbuf[cpos];
	      while (isdigit (cbuf[++cpos]))
		;

	      c = cbuf[cpos];
	      cbuf[cpos] = '\0';
	      yylval.i = strtoimax (cp, NULL, 10);	/* off_t */
	      cbuf[cpos] = c;
	      return (NUMBER);
	    }

	  switch (cbuf[cpos++])
	    {
	    case '\n':
	      state = CMD;
	      return (CRLF);

	    case ' ':
	      return (SP);

	    case ',':
	      return (COMMA);

	    case 'A':
	    case 'a':
	      return (A);

	    case 'B':
	    case 'b':
	      return (B);

	    case 'C':
	    case 'c':
	      return (C);

	    case 'E':
	    case 'e':
	      return (E);

	    case 'F':
	    case 'f':
	      return (F);

	    case 'I':
	    case 'i':
	      return (I);

	    case 'L':
	    case 'l':
	      return (L);

	    case 'N':
	    case 'n':
	      return (N);

	    case 'P':
	    case 'p':
	      return (P);

	    case 'R':
	    case 'r':
	      return (R);

	    case 'S':
	    case 's':
	      return (S);

	    case 'T':
	    case 't':
	      return (T);
	    }
	  break;	/* No number, not in [\n ,aAbBcCeEfFiIlLnNpPrRsSttT] */

	default:
	  fatal ("Unknown state in scanner.");
	}

      /*
       * Analysis: Cases when this point is reached.
       *
       *  CMD:      command not known
       *  SITECMD:  site command not known (state changed to CMD)
       *
       *  OSTR, STR1, ZSTR1, STR2, ZSTR2, NSTR:
       *            empty string or string without NL
       *
       *  ARGS:     not a number, not a special character
       */

      /*
       * Issue a new error message only if the parser has not
       * yet reported a complaint.  Without this precaution
       * two messages would be directed to the client, thus
       * upsetting all following exchange.
       */
      if (!yynerrs)
	yyerror ("command not recognized");

      state = CMD;
      longjmp (errcatch, 0);
    } /* for (;;) */
}

void
upper (char *s)
{
  while (*s != '\0')
    {
      if (islower (*s))
	*s = toupper (*s);
      s++;
    }
}

static char *
copy (char *s)
{
  char *p;

  p = malloc (strlen (s) + 1);
  if (p == NULL)
    fatal ("Ran out of memory.");

  strcpy (p, s);
  return (p);
}

static void
help (struct tab *ctab, char *s)
{
  struct tab *c;
  int width, NCMDS;
  const char *help_type;

  if (ctab == sitetab)
    help_type = "SITE ";
  else
    help_type = "";

  width = 0, NCMDS = 0;
  for (c = ctab; c->name != NULL; c++)
    {
      int len = strlen (c->name);

      if (len > width)
	width = len;

      NCMDS++;
    }

  width = (width + 8) &~ 7;

  if (s == 0)
    {
      int i, j, w;
      int columns, lines;

      lreply (214, "The following %scommands are recognized %s.",
	      help_type, "(* =>'s unimplemented)");

      columns = 76 / width;
      if (columns == 0)
	columns = 1;

      lines = (NCMDS + columns - 1) / columns;

      for (i = 0; i < lines; i++)
	{
	  printf ("   ");
	  for (j = 0; j < columns; j++)
	    {
	      c = ctab + j * lines + i;
	      printf ("%s%c", c->name, c->implemented ? ' ' : '*');

	      if (c + lines >= &ctab[NCMDS])
		break;

	      w = strlen (c->name) + 1;
	      while (w < width)
		{
		  putchar (' ');
		  w++;
		}
	    }
	  printf ("\r\n");
	}
      fflush (stdout);
      reply (214, "Direct comments to ftp-bugs@%s.", hostname);
      return;
    }

  upper (s);

  c = lookup (ctab, s);
  if (c == (struct tab *) 0)
    {
      reply (502, "Unknown command %s.", s);
      return;
    }

  if (c->implemented)
    reply (214, "Syntax: %s%s %s", help_type, c->name, c->help);
  else
    reply (214, "%s%-*s\t%s; unimplemented.", help_type,
	   width, c->name, c->help);
}

static void
sizecmd (char *filename)
{
  switch (type)
    {
    case TYPE_L:
    case TYPE_I:
      {
	struct stat stbuf;

	if (stat (filename, &stbuf) < 0 || !S_ISREG (stbuf.st_mode))
	  reply (550, "%s: not a plain file.", filename);
	else
	  reply (213, "%ju", (uintmax_t) stbuf.st_size);
	break;
      }

    case TYPE_A:
      {
	FILE *fin;
	int c;
	off_t count;
	struct stat stbuf;

	fin = fopen (filename, "r");
	if (fin == NULL)
	  {
	    perror_reply (550, filename);
	    return;
	  }

	if (fstat (fileno (fin), &stbuf) < 0 || !S_ISREG (stbuf.st_mode))
	  {
	    reply (550, "%s: not a plain file.", filename);
	    fclose (fin);
	    return;
	  }

	count = 0;
	while ((c = getc (fin)) != EOF)
	  {
	    if (c == '\n')	/* will get expanded to \r\n */
	      count++;
	    count++;
	  }
	fclose (fin);

	reply (213, "%jd", (intmax_t) count);
	break;
      }

    default:
      reply (504, "SIZE not implemented for Type %c.", "?AEIL"[type]);
    }
}

static void
yyerror (const char *s)
{
  char *cp;

  cp = strchr (cbuf, '\n');
  if (cp != NULL)
    *cp = '\0';

  reply (500, "'%s': %s", cbuf, (s ? s : "command not understood."));
}
