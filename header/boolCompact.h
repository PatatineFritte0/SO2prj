#ifndef BOOLCOMPAT_H
#define BOOLCOMPAT_H

#ifndef __bool_true_false_are_defined
typedef enum { false = 0, true = 1 } bool;
#define __bool_true_false_are_defined 1
#endif

#endif /* BOOLCOMPAT_H */
