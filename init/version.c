/*
 *  linux/init/version.c
 *
 *  Copyright (C) 1992  Theodore Ts'o
 *
 *  May be freely distributed as part of Linux.
 */

#include <linux/compile.h>
#include <linux/module.h>
#include <linux/uts.h>
#include <linux/utsname.h>
#include <linux/utsrelease.h>
#include <linux/version.h>

#ifndef CONFIG_KALLSYMS
#define version(a) Version_ ## a
#define version_string(a) version(a)

extern int version_string(LINUX_VERSION_CODE);
int version_string(LINUX_VERSION_CODE);
#endif

struct uts_namespace init_uts_ns = {
	.kref = {
		.refcount	= ATOMIC_INIT(2),
	},
	.name = {
		.sysname	= UTS_SYSNAME,
		.nodename	= UTS_NODENAME,
		.release	= UTS_RELEASE,
		.version	= UTS_VERSION,
		.machine	= UTS_MACHINE,
		.domainname	= UTS_DOMAINNAME,
	},
};
EXPORT_SYMBOL_GPL(init_uts_ns);

//YWDRIVER_MODI lwj add begin
#ifdef YW_KERNEL_GIT_VER
#define YW_KERNEL_VER "-YW-STM24 0.0.02 "YW_KERNEL_GIT_VER
#else
#define YW_KERNEL_VER "-YW-STM24 0.0.02"
#endif
//YWDRIVER_MODI lwj add end
/* FIXED STRINGS! Don't touch! */
#if 0
const char linux_banner[] =
	"Linux version " UTS_RELEASE " (" LINUX_COMPILE_BY "@"
	LINUX_COMPILE_HOST ") (" LINUX_COMPILER ") " UTS_VERSION "\n";
#else
const char linux_banner[] =
	"Linux version " UTS_RELEASE " "YW_KERNEL_VER" (" LINUX_COMPILE_BY "@"
	LINUX_COMPILE_HOST ") (" LINUX_COMPILER ") " UTS_VERSION "\n";

#endif
/*****     2012-09-10     *****/
//YWDRIVER_MODI modify by lf for add kernel ver start
#if 1
const char linux_proc_banner[] =
	"%s version %s " YW_KERNEL_VER
	" (" LINUX_COMPILE_BY "@" LINUX_COMPILE_HOST ")"
	" (" LINUX_COMPILER ") %s\n";
#else
const char linux_proc_banner[] =
	"%s version %s"
	" (" LINUX_COMPILE_BY "@" LINUX_COMPILE_HOST ")"
	" (" LINUX_COMPILER ") %s\n";
#endif
//YWDRIVER_MODI modify by lf end
