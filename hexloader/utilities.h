//
//  utilities.h
//  hexloader
//
//  Created by Chris Matthews on 2016-07-17.
//  Copyright © 2016 Unrecoverable.org. All rights reserved.
//

#ifndef utilities_h
#define utilities_h

#define GENERIC_MIN(x, y) ((x) < (y) ? (x) : (y))
#define GENERIC_MAX(x, y) ((x) > (y) ? (x) : (y))

#define ENSURE_int(i)   _Generic((i), int:   (i))
#define ENSURE_float(f) _Generic((f), float: (f))
#define ENSURE_long(l) _Generic((l), ssize_t: (l))


#define MIN(type, x, y) \
(type)GENERIC_MIN(ENSURE_##type(x), ENSURE_##type(y))

#define MAX(type, x, y) \
(type)GENERIC_MAX(ENSURE_##type(x), ENSURE_##type(y))

#endif /* utilities_h */
