#!/usr/bin/env python

VERSION='0.0.1'
LIBNAME='octonaut'

top = '.'

import waflib.Configure
waflib.Configure.autoconfig = True

def options(opt):
    opt.load('compiler_c')
    opt.load('gnu_dirs')

def configure(conf):
    conf.load('compiler_c')
    conf.check_cc(lib='ev', uselib_store='ev', mandatory=True)
    conf.check_cc(lib='check', uselib_store='check', mandatory=False)

def build(bld):
    bld.recurse('octonaut')
    bld.recurse('tests')
