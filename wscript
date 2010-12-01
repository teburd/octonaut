#!/usr/bin/env python

VERSION='0.0.1'
LIBNAME='octonaut'

import waflib.Configure
waflib.Configure.autoconfig = True

from waflib import Logs

def options(opt):
    opt.load('compiler_c')
    opt.load('waf_unit_test')
    opt.load('gnu_dirs')

def configure(conf):
    conf.setenv('debug')
    conf.load('compiler_c')
    conf.check_cc(lib='ev', uselib_store='ev', mandatory=True)
    conf.check_cc(lib='check', uselib_store='check', mandatory=False)
    conf.env.append_value('CFLAGS', '-Wall -pedantic -std=gnu99 -g'.split())

    conf.setenv('release')
    conf.load('compiler_c')
    conf.check_cc(lib='ev', uselib_store='ev', mandatory=True)
    conf.check_cc(lib='check', uselib_store='check', mandatory=False)
    conf.env.append_value('CFLAGS', '-Wall -pedantic -std=gnu99 -ffast-math -O3'.split())

    conf.setenv('coverage')
    conf.load('compiler_c')
    conf.check_cc(lib='ev', uselib_store='ev', mandatory=True)
    conf.check_cc(lib='check', uselib_store='check', mandatory=False)
    conf.check_cc(lib='gcov', uselib_store='gcov', mandatory=True)
    conf.env.append_value('CFLAGS', '-Wall -pedantic -std=gnu99 -fprofile-arcs -ftest-coverage'.split())

def test(ctx):
    ctx.exec_command('./build/tests/octonaut_tests') 

def valgrindtest(ctx):
    ctx.exec_command('CK_FORK=no valgrind --leak-check=full -v ./build/tests/octonaut_tests') 

def gdbtest(ctx):
    ctx.exec_command('CK_FORK=no gdb ./build/tests/octonaut_tests') 

def build(bld):
    if not bld.variant: 
        bld.fatal('call "waf debug" or "waf release", and try "waf --help"')
    bld.recurse('octonaut')
    bld.recurse('tests')
    bld.options.all_tests = True
    bld.add_post_fun(test)

from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext

for x in 'debug release coverage'.split():
    for y in (BuildContext, CleanContext, InstallContext, UninstallContext):
        name = y.__name__.replace('Context','').lower()
        class tmp(y): 
            cmd = name + '_' + x
            variant = x
