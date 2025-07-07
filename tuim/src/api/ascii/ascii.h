#ifndef ASCII_H
#define ASCII_H

/* Update this when targeting a system where strings are not ASCII encoded. */
#define string(s) ((char*)(s))
#define ascii(s)  ((uint8_t*)(s))

#endif /* ASCII_H */
