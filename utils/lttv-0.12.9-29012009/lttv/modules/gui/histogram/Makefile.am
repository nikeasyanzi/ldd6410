# This file is part of the Linux Trace Toolkit viewer
# Copyright (C) 2003-2004 Mathieu Desnoyers
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License Version 2 as
# published by the Free Software Foundation;
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, 
# MA 02111-1307, USA.



#
# Makefile for LTT New generation user interface : plugins.
#
# Created by Mathieu Desnoyers on May 6, 2003
#

AM_CFLAGS = $(GLIB_CFLAGS) 
AM_CFLAGS += $(GTK_CFLAGS)
AM_CFLAGS += -fvisibility=hidden
LIBS += $(GLIB_LIBS)
LIBS += $(THREAD_LIBS)
LIBS += $(GTK_LIBS) -L${top_builddir}/lttv/modules/gui/lttvwindow/lttvwindow -llttvwindow

libdir = ${lttvplugindir}

lib_LTLIBRARIES = libguihistogram.la
libguihistogram_la_LDFLAGS = -module -avoid-version
libguihistogram_la_SOURCES = histomodule.c histoeventhooks.c histocfv.c \
			histobuttonwidget.c histodrawing.c histodrawitem.c

noinst_HEADERS = histoeventhooks.h histocfv.h \
		histobuttonwidget.h histodrawing.h histodrawitem.h

EXTRA_DIST = \
		hHistogramInsert.xpm stock_zoom_in_24.xpm stock_zoom_out_24.xpm \
		stock_zoom_fit_24.xpm
