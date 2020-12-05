/*
** $Id: liolib.c $
** Standard I/O (and system) library
** See Copyright Notice in lua.h
*/


#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


#define IO_PREFIX    "_IO_"
#define IOPREF_LEN    (sizeof(IO_PREFIX)/sizeof(char) - 1)
#define IO_INPUT    (IO_PREFIX "input")
#define IO_OUTPUT    (IO_PREFIX "output")


typedef luaL_Stream LStream;


#define isclosed(p)    ((p)->closef == NULL)


static FILE *getiofile(lua_State *L, const char *findex) {
    LStream *p;
    lua_getfield(L, LUA_REGISTRYINDEX, findex);
    p = (LStream *) lua_touserdata(L, -1);
    if (isclosed(p))
        luaL_error(L, "default %s file is closed", findex + IOPREF_LEN);
    return p->f;
}




/*
** {======================================================
** READ
** =======================================================
*/


/* maximum length of a numeral */
#if !defined (L_MAXLENNUM)
#define L_MAXLENNUM     200
#endif


/* auxiliary structure used by 'read_number' */
typedef struct {
    FILE *f;  /* file being read */
    int c;  /* current character (look ahead) */
    int n;  /* number of elements in buffer 'buff' */
    char buff[L_MAXLENNUM + 1];  /* +1 for ending '\0' */
} RN;


/*
** Add current char to buffer (if not out of space) and read next one
*/
static int nextc(RN *rn) {
    if (rn->n >= L_MAXLENNUM) {  /* buffer overflow? */
        rn->buff[0] = '\0';  /* invalidate result */
        return 0;  /* fail */
    } else {
        rn->buff[rn->n++] = rn->c;  /* save current char */
        rn->c = l_getc(rn->f);  /* read next one */
        return 1;
    }
}


/*
** Accept current char if it is in 'set' (of size 2)
*/
static int test2(RN *rn, const char *set) {
    if (rn->c == set[0] || rn->c == set[1])
        return nextc(rn);
    else return 0;
}


/*
** Read a sequence of (hex)digits
*/
static int readdigits(RN *rn, int hex) {
    int count = 0;
    while ((hex ? isxdigit(rn->c) : isdigit(rn->c)) && nextc(rn))
        count++;
    return count;
}


/*
** Read a number: first reads a valid prefix of a numeral into a buffer.
** Then it calls 'lua_stringtonumber' to check whether the format is
** correct and to convert it to a Lua number.
*/
static int read_number(lua_State *L, FILE *f) {
    RN rn;
    int count = 0;
    int hex = 0;
    char decp[2];
    rn.f = f;
    rn.n = 0;
    decp[0] = lua_getlocaledecpoint();  /* get decimal point from locale */
    decp[1] = '.';  /* always accept a dot */
    l_lockfile(rn.f);
    do { rn.c = l_getc(rn.f); } while (isspace(rn.c));  /* skip spaces */
    test2(&rn, "-+");  /* optional sign */
    if (test2(&rn, "00")) {
        if (test2(&rn, "xX")) hex = 1;  /* numeral is hexadecimal */
        else count = 1;  /* count initial '0' as a valid digit */
    }
    count += readdigits(&rn, hex);  /* integral part */
    if (test2(&rn, decp))  /* decimal point? */
        count += readdigits(&rn, hex);  /* fractional part */
    if (count > 0 && test2(&rn, (hex ? "pP" : "eE"))) {  /* exponent mark? */
        test2(&rn, "-+");  /* exponent sign */
        readdigits(&rn, 0);  /* exponent digits */
    }
    ungetc(rn.c, rn.f);  /* unread look-ahead char */
    l_unlockfile(rn.f);
    rn.buff[rn.n] = '\0';  /* finish string */
    if (lua_stringtonumber(L, rn.buff))  /* is this a valid number? */
        return 1;  /* ok */
    else {  /* invalid format */
        lua_pushnil(L);  /* "result" to be removed */
        return 0;  /* read fails */
    }
}


static int test_eof(lua_State *L, FILE *f) {
    int c = getc(f);
    ungetc(c, f);  /* no-op when c == EOF */
    lua_pushliteral(L, "");
    return (c != EOF);
}


static int read_line(lua_State *L, FILE *f, int chop) {
    luaL_Buffer b;
    int c;
    luaL_buffinit(L, &b);
    do {  /* may need to read several chunks to get whole line */
        char *buff = luaL_prepbuffer(&b);  /* preallocate buffer space */
        int i = 0;
        while (i < LUAL_BUFFERSIZE && (c = l_getc(f)) != EOF && c != '\n')
            buff[i++] = c;  /* read up to end of line or buffer limit */
        luaL_addsize(&b, i);
    } while (c != EOF && c != '\n');  /* repeat until end of line */
    if (!chop && c == '\n')  /* want a newline and have one? */
        luaL_addchar(&b, c);  /* add ending newline to result */
    luaL_pushresult(&b);  /* close buffer */
    /* return ok if read something (either a newline or something else) */
    return (c == '\n' || lua_rawlen(L, -1) > 0);
}


static void read_all(lua_State *L, FILE *f) {
    size_t nr;
    luaL_Buffer b;
    luaL_buffinit(L, &b);
    do {  /* read file in chunks of LUAL_BUFFERSIZE bytes */
        char *p = luaL_prepbuffer(&b);
        nr = fread(p, sizeof(char), LUAL_BUFFERSIZE, f);
        luaL_addsize(&b, nr);
    } while (nr == LUAL_BUFFERSIZE);
    luaL_pushresult(&b);  /* close buffer */
}


static int read_chars(lua_State *L, FILE *f, size_t n) {
    size_t nr;  /* number of chars actually read */
    char *p;
    luaL_Buffer b;
    luaL_buffinit(L, &b);
    p = luaL_prepbuffsize(&b, n);  /* prepare buffer to read whole block */
    nr = fread(p, sizeof(char), n, f);  /* try to read 'n' chars */
    luaL_addsize(&b, nr);
    luaL_pushresult(&b);  /* close buffer */
    return (nr > 0);  /* true iff read something */
}


static int g_read(lua_State *L, FILE *f, int first) {
    int nargs = lua_gettop(L) - 1;
    int n, success;
    clearerr(f);
    if (nargs == 0) {  /* no arguments? */
        success = read_line(L, f, 1);
        n = first + 1;  /* to return 1 result */
    } else {
        /* ensure stack space for all results and for auxlib's buffer */
        luaL_checkstack(L, nargs + LUA_MINSTACK, "too many arguments");
        success = 1;
        for (n = first; nargs-- && success; n++) {
            if (lua_type(L, n) == LUA_TNUMBER) {
                size_t l = (size_t) luaL_checkinteger(L, n);
                success = (l == 0) ? test_eof(L, f) : read_chars(L, f, l);
            } else {
                const char *p = luaL_checkstring(L, n);
                if (*p == '*') p++;  /* skip optional '*' (for compatibility) */
                switch (*p) {
                    case 'n':  /* number */
                        success = read_number(L, f);
                        break;
                    case 'l':  /* line */
                        success = read_line(L, f, 1);
                        break;
                    case 'L':  /* line with end-of-line */
                        success = read_line(L, f, 0);
                        break;
                    case 'a':  /* file */
                        read_all(L, f);  /* read entire file */
                        success = 1; /* always success */
                        break;
                    default:
                        return luaL_argerror(L, n, "invalid format");
                }
            }
        }
    }
    if (ferror(f))
        return luaL_fileresult(L, 0, NULL);
    if (!success) {
        lua_pop(L, 1);  /* remove last result */
        luaL_pushfail(L);  /* push nil instead */
    }
    return n - first;
}


static int io_read(lua_State *L) {
    return g_read(L, getiofile(L, IO_INPUT), 1);
}


/* }====================================================== */


static int g_write(lua_State *L, FILE *f, int arg) {
    int nargs = lua_gettop(L) - arg;
    int status = 1;
    for (; nargs--; arg++) {
        if (lua_type(L, arg) == LUA_TNUMBER) {
            /* optimization: could be done exactly as for strings */
            int len = lua_isinteger(L, arg)
                      ? fprintf(f, LUA_INTEGER_FMT,
                                (LUAI_UACINT) lua_tointeger(L, arg))
                      : fprintf(f, LUA_NUMBER_FMT,
                                (LUAI_UACNUMBER) lua_tonumber(L, arg));
            status = status && (len > 0);
        } else {
            size_t l;
            const char *s = luaL_checklstring(L, arg, &l);
            status = status && (fwrite(s, sizeof(char), l, f) == l);
        }
    }
    if (status) return 1;  /* file handle already on stack top */
    else return luaL_fileresult(L, status, NULL);
}


static int io_write(lua_State *L) {
    return g_write(L, getiofile(L, IO_OUTPUT), 1);
}


static int io_flush(lua_State *L) {
    return luaL_fileresult(L, fflush(getiofile(L, IO_OUTPUT)) == 0, NULL);
}


/*
** functions for 'io' library
*/
static const luaL_Reg iolib[] = {
        {"flush", io_flush},
        {"read",  io_read},
        {"write", io_write},
        {NULL, NULL}
};


LUAMOD_API int lua_io_lib(lua_State *L) {
    luaL_newlib(L, iolib);  /* new module */
    return 1;
}

