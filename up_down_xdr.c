/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "up_down.h"

bool_t
xdr_chunk (XDR *xdrs, chunk *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->sendBuff, 1025,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->dirname, 1000,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->flag))
		 return FALSE;
	return TRUE;
}