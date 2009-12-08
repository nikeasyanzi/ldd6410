/* $XFree86: xc/programs/Xserver/hw/xfree86/common/xf86Bus.h,v 1.24 2004/02/13 23:58:36 dawes Exp $ */

/*
 * Copyright (c) 1997-2003 by The XFree86 Project, Inc.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 *   1.  Redistributions of source code must retain the above copyright
 *       notice, this list of conditions, and the following disclaimer.
 *
 *   2.  Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer
 *       in the documentation and/or other materials provided with the
 *       distribution, and in the same place and form as other copyright,
 *       license and disclaimer information.
 *
 *   3.  The end-user documentation included with the redistribution,
 *       if any, must include the following acknowledgment: "This product
 *       includes software developed by The XFree86 Project, Inc
 *       (http://www.xfree86.org/) and its contributors", in the same
 *       place and form as other third-party acknowledgments.  Alternately,
 *       this acknowledgment may appear in the software itself, in the
 *       same form and location as other such third-party acknowledgments.
 *
 *   4.  Except as contained in this notice, the name of The XFree86
 *       Project, Inc shall not be used in advertising or otherwise to
 *       promote the sale, use or other dealings in this Software without
 *       prior written authorization from The XFree86 Project, Inc.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE XFREE86 PROJECT, INC OR ITS CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This file contains definitions of the bus-related data structures/types.
 * Everything contained here is private to xf86Bus.c.  In particular the
 * video drivers must not include this file.
 */

#ifndef _XF86_BUS_H
#define _XF86_BUS_H

#include "xf86pciBus.h"
#ifdef __sparc__
#include "xf86sbusBus.h"
#endif

typedef struct racInfo {
    xf86AccessPtr mem_new;
    xf86AccessPtr io_new;
    xf86AccessPtr io_mem_new;
    xf86SetAccessFuncPtr old;
} AccessFuncRec, *AccessFuncPtr;


typedef struct {
    DriverPtr                   driver;
    int                         chipset;
    int                         entityProp;
    EntityProc                  entityInit;
    EntityProc                  entityEnter;
    EntityProc                  entityLeave;
    pointer                     private;
    resPtr                      resources;
    Bool                        active;
    Bool                        inUse;
    BusRec                      bus;
    EntityAccessPtr             access;
    AccessFuncPtr               rac;
    pointer                     busAcc;
    int                         lastScrnFlag;
    DevUnion *                  entityPrivates;
    int                         numInstances;
    GDevPtr *                   devices;   
    IOADDRESS                   domainIO;
} EntityRec, *EntityPtr;

/* asynchronous event handling */
#ifdef async
typedef struct _AsyncQRec {
    void (*func)(pointer);
    pointer arg;
    struct _AsyncQRec *next;
} AsyncQRec, *AsyncQPtr;
#endif

#define NO_SEPARATE_IO_FROM_MEM 0x0001
#define NO_SEPARATE_MEM_FROM_IO 0x0002
#define NEED_VGA_ROUTED 0x0004
#define NEED_VGA_ROUTED_SETUP 0x0008
#define NEED_MEM 0x0010
#define NEED_IO  0x0020
#define NEED_MEM_SHARED 0x0040
#define NEED_IO_SHARED 0x0080
#define ACCEL_IS_SHARABLE 0x0100
#define IS_SHARED_ACCEL 0x0200
#define SA_PRIM_INIT_DONE 0x0400
#define NEED_VGA_MEM 0x1000
#define NEED_VGA_IO  0x2000

#define NEED_SHARED (NEED_MEM_SHARED | NEED_IO_SHARED)

#define busType bus.type
#define pciBusId bus.id.pci
#define isaBusId bus.id.isa
#define sbusBusId bus.id.sbus

struct x_BusAccRec;
typedef void (*BusAccProcPtr)(struct x_BusAccRec *ptr);

typedef struct x_BusAccRec {
    BusAccProcPtr set_f;
    BusAccProcPtr enable_f;
    BusAccProcPtr disable_f;
    BusAccProcPtr save_f;
    BusAccProcPtr restore_f;
    struct x_BusAccRec *current; /* pointer to bridge open on this bus */
    struct x_BusAccRec *primary; /* pointer to the bus connecting to this */
    struct x_BusAccRec *next;    /* this links the different buses together */
    BusType type;
    BusType busdep_type;
    /* Bus-specific fields */
    union {
	struct {
	    int bus;
	    int primary_bus;
	    PCITAG acc;
	    pciBridgesSave save;
	} pci;
    } busdep;
} BusAccRec, *BusAccPtr;

/* state change notification callback */
typedef struct _stateChange {
    xf86StateChangeNotificationCallbackFunc func;
    pointer arg;
    struct _stateChange *next;
} StateChangeNotificationRec, *StateChangeNotificationPtr;


extern EntityPtr *xf86Entities;
extern int xf86NumEntities;
extern xf86AccessRec AccessNULL;
extern BusRec primaryBus;
extern resPtr Acc;
extern resPtr osRes;
extern resPtr ResRange;
extern BusAccPtr xf86BusAccInfo;

int xf86AllocateEntity(void);
BusType StringToBusType(const char* busID, const char **retID);
memType ChkConflict(resRange *rgp, resPtr res, xf86State state);
Bool xf86IsSubsetOf(resRange range, resPtr list);
Bool xf86IsListSubsetOf(resPtr list, resPtr BaseList);
resPtr xf86ExtractTypeFromList(resPtr list, unsigned long type);
resPtr findIntersect(resRange Range, resPtr list);
resPtr xf86FindIntersect(resRange Range, resPtr list);
void RemoveOverlaps(resPtr target, resPtr list, Bool pow2Alignment,
		    Bool useEstimated);
void xf86ConvertListToHost(int entityIndex, resPtr list);

#endif /* _XF86_BUS_H */
