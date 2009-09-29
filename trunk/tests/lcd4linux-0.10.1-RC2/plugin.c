/* $Id: plugin.c 728 2007-01-14 11:14:38Z michael $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/branches/0.10.1/plugin.c $
 *
 * plugin handler for the Evaluator
 *
 * Copyright (C) 2003 Michael Reinelt <reinelt@eunet.at>
 * Copyright (C) 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 *
 * This file is part of LCD4Linux.
 *
 * LCD4Linux is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * LCD4Linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* 
 * exported functions:
 *
 * int plugin_init (void)
 *  initializes the expression evaluator
 *  adds some handy constants and functions
 *
 */


#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "plugin.h"


/* Prototypes */
int plugin_init_cfg(void);
void plugin_exit_cfg(void);
int plugin_init_math(void);
void plugin_exit_math(void);
int plugin_init_string(void);
void plugin_exit_string(void);
int plugin_init_test(void);
void plugin_exit_test(void);
int plugin_init_time(void);
void plugin_exit_time(void);

int plugin_init_apm(void);
void plugin_exit_apm(void);
int plugin_init_cpuinfo(void);
void plugin_exit_cpuinfo(void);
int plugin_init_diskstats(void);
void plugin_exit_diskstats(void);
int plugin_init_dvb(void);
void plugin_exit_dvb(void);
int plugin_init_exec(void);
void plugin_exit_exec(void);
int plugin_init_file(void);
void plugin_exit_file(void);
int plugin_init_i2c_sensors(void);
void plugin_exit_i2c_sensors(void);
int plugin_init_imon(void);
void plugin_exit_imon(void);
int plugin_init_iconv(void);
void plugin_exit_iconv(void);
int plugin_init_isdn(void);
void plugin_exit_isdn(void);
int plugin_init_kvv(void);
void plugin_exit_kvv(void);
int plugin_init_loadavg(void);
void plugin_exit_loadavg(void);
int plugin_init_meminfo(void);
void plugin_exit_meminfo(void);
int plugin_init_mpd(void);
void plugin_exit_mpd(void);
int plugin_init_mysql(void);
void plugin_exit_mysql(void);
int plugin_init_netdev(void);
void plugin_exit_netdev(void);
int plugin_init_pop3(void);
void plugin_exit_pop3(void);
int plugin_init_ppp(void);
void plugin_exit_ppp(void);
int plugin_init_proc_stat(void);
void plugin_exit_proc_stat(void);
int plugin_init_python(void);
void plugin_exit_python(void);
int plugin_init_sample(void);
void plugin_exit_sample(void);
int plugin_init_seti(void);
void plugin_exit_seti(void);
int plugin_init_statfs(void);
void plugin_exit_statfs(void);
int plugin_init_uname(void);
void plugin_exit_uname(void);
int plugin_init_uptime(void);
void plugin_exit_uptime(void);
int plugin_init_wireless(void);
void plugin_exit_wireless(void);
int plugin_init_xmms(void);
void plugin_exit_xmms(void);


int plugin_init(void)
{
    plugin_init_cfg();
    plugin_init_math();
    plugin_init_string();
    plugin_init_test();
    plugin_init_time();

#ifdef PLUGIN_APM
    plugin_init_apm();
#endif
#ifdef PLUGIN_CPUINFO
    plugin_init_cpuinfo();
#endif
#ifdef PLUGIN_DISKSTATS
    plugin_init_diskstats();
#endif
#ifdef PLUGIN_DVB
    plugin_init_dvb();
#endif
#ifdef PLUGIN_EXEC
    plugin_init_exec();
#endif
#ifdef PLUGIN_FILE
    plugin_init_file();
#endif
#ifdef PLUGIN_I2C_SENSORS
    plugin_init_i2c_sensors();
#endif
#ifdef PLUGIN_ICONV
    plugin_init_iconv();
#endif
#ifdef PLUGIN_IMON
    plugin_init_imon();
#endif
#ifdef PLUGIN_ISDN
    plugin_init_isdn();
#endif
#ifdef PLUGIN_KVV
    plugin_init_kvv();
#endif
#ifdef PLUGIN_LOADAVG
    plugin_init_loadavg();
#endif
#ifdef PLUGIN_MEMINFO
    plugin_init_meminfo();
#endif
#ifdef PLUGIN_MPD
    plugin_init_mpd();
#endif

#ifdef PLUGIN_MYSQL
    plugin_init_mysql();
#endif
#ifdef PLUGIN_NETDEV
    plugin_init_netdev();
#endif
#ifdef PLUGIN_POP3
    plugin_init_pop3();
#endif
#ifdef PLUGIN_PPP
    plugin_init_ppp();
#endif
#ifdef PLUGIN_PROC_STAT
    plugin_init_proc_stat();
#endif
#ifdef PLUGIN_PYTHON
    plugin_init_python();
#endif
#ifdef PLUGIN_SAMPLE
    plugin_init_sample();
#endif
#ifdef PLUGIN_SETI
    plugin_init_seti();
#endif
#ifdef PLUGIN_STATFS
    plugin_init_statfs();
#endif
#ifdef PLUGIN_UNAME
    plugin_init_uname();
#endif
#ifdef PLUGIN_UPTIME
    plugin_init_uptime();
#endif
#ifdef PLUGIN_WIRELESS
    plugin_init_wireless();
#endif
#ifdef PLUGIN_XMMS
    plugin_init_xmms();
#endif

    return 0;
}


void plugin_exit(void)
{
#ifdef PLUGIN_APM
    plugin_exit_apm();
#endif
#ifdef PLUGIN_CPUINFO
    plugin_exit_cpuinfo();
#endif
#ifdef PLUGIN_DISKSTATS
    plugin_exit_diskstats();
#endif
#ifdef PLUGIN_DVB
    plugin_exit_dvb();
#endif
#ifdef PLUGIN_EXEC
    plugin_exit_exec();
#endif
#ifdef PLUGIN_FILE
    plugin_exit_file();
#endif
#ifdef PLUGIN_I2C_SENSORS
    plugin_exit_i2c_sensors();
#endif
#ifdef PLUGIN_ICONV
    plugin_exit_iconv();
#endif
#ifdef PLUGIN_IMON
    plugin_exit_imon();
#endif
#ifdef PLUGIN_ISDN
    plugin_exit_isdn();
#endif
#ifdef PLUGIN_KVV
    plugin_exit_kvv();
#endif
#ifdef PLUGIN_LOADAVG
    plugin_exit_loadavg();
#endif
#ifdef PLUGIN_MEMINFO
    plugin_exit_meminfo();
#endif
#ifdef PLUGIN_MPD
    plugin_exit_mpd();
#endif
#ifdef PLUGIN_MYSQL
    plugin_exit_mysql();
#endif
#ifdef PLUGIN_NETDEV
    plugin_exit_netdev();
#endif
#ifdef PLUGIN_POP3
    plugin_exit_pop3();
#endif
#ifdef PLUGIN_PPP
    plugin_exit_ppp();
#endif
#ifdef PLUGIN_PROC_STAT
    plugin_exit_proc_stat();
#endif
#ifdef PLUGIN_PYTHON
    plugin_exit_python();
#endif
#ifdef PLUGIN_SAMPLE
    plugin_exit_sample();
#endif
#ifdef PLUGIN_SETI
    plugin_exit_seti();
#endif
#ifdef PLUGIN_STATFS
    plugin_exit_statfs();
#endif
#ifdef PLUGIN_UNAME
    plugin_exit_uname();
#endif
#ifdef PLUGIN_UPTIME
    plugin_exit_uptime();
#endif
#ifdef PLUGIN_WIRELESS
    plugin_exit_wireless();
#endif
#ifdef PLUGIN_XMMS
    plugin_exit_xmms();
#endif

    plugin_exit_cfg();
    plugin_exit_math();
    plugin_exit_string();
    plugin_exit_test();
    plugin_exit_time();

    DeleteFunctions();
    DeleteVariables();
}
