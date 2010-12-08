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
    conf.load('compiler_c')
    conf.check_cc(lib='ev', uselib_store='ev', mandatory=True)
    conf.check_cc(lib='check', uselib_store='check', mandatory=False)
    conf.env.append_value('CFLAGS', '-Wall -pedantic -std=gnu99'.split())
    
    base_env = conf.env.derive()

    conf.setenv('debug', base_env)
    conf.env.append_value('CFLAGS', '-g3'.split())

    conf.setenv('release', base_env)
    conf.env.append_value('CFLAGS', '-Wall -fno-strict-aliasing -ffast-math -O3'.split())

def test(ctx):
    if not ctx.variant_dir:
        ctx.exec_command('./build/tests/octonaut_tests')
    else:
        ctx.exec_command(ctx.variant_dir + '/tests/octonaut_tests') 

def kcovtest(ctx):
    ctx.exec_command('rm -rf coverage')
    ctx.exec_command('mkdir -p coverage')
    ctx.exec_command('CK_FORK=no kcov coverage ./build/debug/tests/octonaut_tests')

def valgrindtest(ctx):
    ctx.exec_command('CK_FORK=no valgrind --leak-check=full -v ./build/debug/tests/octonaut_tests') 

def gdbtest(ctx):
    ctx.exec_command('CK_FORK=no gdb ./build/debug/tests/octonaut_tests') 

def build(bld):
    bld.recurse('octonaut')
    bld.recurse('octohttp')
    bld.recurse('tests')
    bld.recurse('examples')
    bld.options.all_tests = True
    bld.add_post_fun(test)

from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext

for x in 'debug release'.split():
    for y in (BuildContext, CleanContext, InstallContext, UninstallContext):
        name = y.__name__.replace('Context','').lower()
        class tmp(y): 
            cmd = name + '_' + x
            variant = x
