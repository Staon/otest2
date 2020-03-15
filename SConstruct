# location of the llvm-config
AddOption(
    '--with-llvm',
    dest='llvmcfg',
    type='string',
    nargs=1,
    action='store',
    metavar='DIR',
    help='The llvm-config binary file.')
llvmcfg = GetOption('llvmcfg')
if not llvmcfg :
    llvmcfg = 'llvm-config'

env = Environment()

# -- auto configuration
cfg = Configure(env)

# check the llvm-config utility
if not cfg.CheckProg(llvmcfg):
    print 'Unable to find the llvm-config utility'  
    Exit(1)
    
env = cfg.Finish()

# fixed environment variables
env.Append(CPPPATH = ['#include', '#lib'])
env.Append(CXXFLAGS = '-std=c++14')
env.Append(CCFLAGS = '-g')

# OTest2 preprocessor builder
def otest2_preprocessor(source, target, env, for_signature):
    cmd = ['otest2/otest2', '-o', target[0]]
    cmd.append('-I/usr/local/lib/clang/7.1.0/include')
    cmd.append('-I/usr/lib/gcc/x86_64-linux-gnu/6/include')
    # -- include paths
    for path in env.Dictionary()['CPPPATH']:
        cmd.append('-I' + str(env.Dir(path)))
    # -- domain
    if 'OTEST2DOMAIN' in env:
        cmd.append('-d' + env['OTEST2DOMAIN'])
    cmd.append(source[0])
    return [cmd]
def otest2_tg_scanner(node, env, path):
    return [env.File('otest2/otest2')]
tgscanner = env.Scanner(otest2_tg_scanner)
from SCons.Scanner.C import CScanner
otest2 = Builder(
    generator = otest2_preprocessor,
    suffix = '.ot2.cpp',
    src_suffix = '.ot2',
    single_source = True,
    target_scanner = tgscanner,
    source_scanner = CScanner())
env.Append(BUILDERS = {'OTest2' : otest2})

# Compilation of the test suite
def otest2_compile_tests(env, builder, target, sources):
    new_sources = []
    for src in sources:
        if str(src).endswith('.ot2'):
            new_sources.append(env.OTest2(src))
        else:
            new_sources.append(src)
    return getattr(env, builder)(target, new_sources)
env.AddMethod(otest2_compile_tests, "OTest2Suite")

SConscript([
    'lib/SConstruct',
    'otest2/SConstruct',
    'test/SConstruct',
], ['env', 'llvmcfg'])
