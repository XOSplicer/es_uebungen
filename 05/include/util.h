/**
 * @author Felix Stegmaier
 */

#ifndef UTIL_H
#define UTIL_H

#define NO_COPY_INSTANCE(cls) \
cls(const cls&);\
cls& operator =(const cls&);

#define CEIL_DIV(x, y) (x/y + (x % y != 0))

#ifndef STDOUT
#define STDOUT 1
#endif

#endif /* UTIL_H */
