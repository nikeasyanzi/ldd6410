#
#	Makefile for the AppWeb C samples 
#
#	Copyright (c) Embedthis Software LLC, 2003-2009. All Rights Reserved.
#

include 		.makedep

PRE_DIRS		+= simpleServer 

ifeq ($(BLD_FEATURE_HTTP_CLIENT),1)
	PRE_DIRS 	+= simpleClient 
endif

ifeq ($(BLD_FEATURE_STATIC),0)
	PRE_DIRS 	+= handler
endif

ifeq ($(BLD_FEATURE_EJS),1)
	PRE_DIRS 	+= simpleEjs
endif

ifeq ($(BLD_FEATURE_EGI),1)
	PRE_DIRS 	+= simpleEgi
endif
