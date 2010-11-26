#!/usr/bin/env python

VERSION='0.0.1'
LIBNAME='octonaut'

top = '.'

import waflib.Configure
waflib.Configure.autoconfig = True

from waflib import Logs

def test_summary(bld):
    test_results = getattr(bld, 'utest_results', [])
    for test_result in test_results:
        Logs.pprint('CYAN', test_result[2])

def options(opt):
    opt.load('compiler_c')
    opt.load('waf_unit_test')
    opt.load('gnu_dirs')

def configure(conf):
    conf.load('compiler_c')
    conf.load('waf_unit_test')
    conf.env.append_value('CFLAGS', '-Wall -pedantic -std=c99 -g'.split())
    conf.check_cc(lib='ev', uselib_store='ev', mandatory=True)
    conf.check_cc(lib='check', uselib_store='check', mandatory=False)

def build(bld):
    bld.recurse('octonaut')
    bld.recurse('tests')
    bld.add_post_fun(test_summary)
    bld.options.all_tests = True


