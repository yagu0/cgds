#define lu_assert_msg(expr, ...) \
  if (!(expr)) { \
    fprintf(stdout, "Failure in file %s at line %i\n", __FILE__, __LINE__); \
    fprintf(stdout, ## __VA_ARGS__); \
    return; \
  }

#define lu_assert(expr) \
  lu_assert_msg(expr, "");

/* OP may be any comparison operator. */

#define _lu_assert_int(X, OP, Y) do { \
  int _lu_x = (X); \
  int _lu_y = (Y); \
  lu_assert_msg(_lu_x OP _lu_y, \
                "Assertion '"#X#OP#Y"' failed: "#X"==%i, "#Y"==%i\n", \
                _lu_x, _lu_y); \
} while (0)
#define lu_assert_int_eq(X, Y) _lu_assert_int(X, ==, Y)
#define lu_assert_int_ne(X, Y) _lu_assert_int(X, !=, Y)
#define lu_assert_int_lt(X, Y) _lu_assert_int(X, <, Y)
#define lu_assert_int_le(X, Y) _lu_assert_int(X, <=, Y)
#define lu_assert_int_gt(X, Y) _lu_assert_int(X, >, Y)
#define lu_assert_int_ge(X, Y) _lu_assert_int(X, >=, Y)

#define _lu_assert_dbl(X, OP, Y) do { \
  double _lu_x = (X); \
  double _lu_y = (Y); \
  lu_assert_msg(_lu_x OP _lu_y, \
                "Assertion '"#X#OP#Y"' failed: "#X"==%g, "#Y"==%g", \
                _lu_x, _lu_y); \
} while (0)
#define lu_assert_dbl_eq(X, Y) _lu_assert_dbl(X, ==, Y)
#define lu_assert_dbl_ne(X, Y) _lu_assert_dbl(X, !=, Y)
#define lu_assert_dbl_lt(X, Y) _lu_assert_dbl(X, <, Y)
#define lu_assert_dbl_le(X, Y) _lu_assert_dbl(X, <=, Y)
#define lu_assert_dbl_gt(X, Y) _lu_assert_dbl(X, >, Y)
#define lu_assert_dbl_ge(X, Y) _lu_assert_dbl(X, >=, Y)
