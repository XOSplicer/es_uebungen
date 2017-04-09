/**
 * @author Felix Stegmaier
 */

#ifndef UTIL_H
#define UTIL_H

#define NO_COPY_INSTANCE(cls) \
cls(const cls&);\
cls& operator =(const cls&);

#endif /* UTIL_H */
