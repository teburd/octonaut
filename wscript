#!/usr/bin/env python

VERSION='0.0.1'
LIBNAME='octonaut'

top = '.'

import waflib.Configure
waflib.Configure.autoconfig = True

from waflib import Logs

def options(opt):
    opt.load('compiler_c')
    opt.load('waf_unit_test')
    opt.load('gnu_dirs')

def configure(conf):
    conf.load('compiler_c')
    conf.load('waf_unit_test')
    conf.env.append_value('CFLAGS', '-Wall -pedantic -std=gnu99 -g'.split())
    conf.check_cc(lib='ev', uselib_store='ev', mandatory=True)
    conf.check_cc(lib='check', uselib_store='check', mandatory=False)

def test(ctx):
    ctx.exec_command('./build/tests/octonaut_tests') 

def valgrindtest(ctx):
    ctx.exec_command('CK_FORK=no valgrind --leak-check=full -v ./build/tests/octonaut_tests') 

def gdbtest(ctx):
    ctx.exec_command('CK_FORK=no gdb ./build/tests/octonaut_tests') 


def build(bld):
    bld.recurse('octonaut')
    bld.recurse('tests')
    bld.options.all_tests = True
    bld.add_post_fun(test)


