#include "linkage.h"

ENTRY(strcopy)			/* Entry to "strcopy" function */
				/* R0 = address of destination string */
				/* R1 = address of source string */
copyloop:
	ldrb	r2,[r1],#1	/* Load byte into R2 and update R1 */
	strb	r2,[r0],#1	/* Store the byte and update R0 */
	cmp	r2,#0		/* Check for NUL terminator */
	bne	copyloop	/* Repeat loop if not */

	mov	r0,#0x20
	mov	r1,#0x10
	bl	c_func
	bl	c_func
	mov	pc,lr		/* Return to the caller */
ENDPROC(strcopy)

