#!/usr/bin/python2

import os
import sys
import re

label, code = {}, []

def remove_comment(s):
    return s.split('#', 1)[0]

def trim(s):
    from string import whitespace as ws

    while len(s) > 0 and s[0] in ws:
        s = s[1:]

    while len(s) > 0 and s[-1] in ws:
        s = s[:-1]

    return s

def parse_arguments(ins):
    global label, code

    def arg_iter(s):
        while len(s) != 0:
            if re.match(r"^'(.)'", s):
                yield str(ord(re.match(r"^'(.)'", s).groups()[0]))
                s = s[3:]
            elif re.match(r'^0x([a-f0-9]+)', s):
                yield str(int(re.match(r'^0x([a-f0-9]+)', s).groups()[0], 16) & 0xff)
                if ',' in s:
                    s = trim(s.split(',', 1)[1])
                else:
                    s = ''
            elif re.match(r'^(r[0-9]+)', s):
                yield re.match(r'^(r[0-9]+)', s).groups()[0]
                if ',' in s:
                    s = trim(s.split(',', 1)[1])
                else:
                    s = ''
            elif re.match(r'^([0-9]+)', s):
                yield re.match(r'^([0-9]+)', s).groups()[0]
                if ',' in s:
                    s = trim(s.split(',', 1)[1])
                else:
                    s = ''
            elif re.match(r'^([a-zA-Z_][a-zA-Z_0-9]*)', s):
                l = re.match(r'^([a-zA-Z_][a-zA-Z_0-9]*)', s).groups()[0]
                assert l in label, 'No such label: [{}]'.format(l)

                yield str(label[l])
                if ',' in s:
                    s = trim(s.split(',', 1)[1])
                else:
                    s = ''
            else:
                assert False, 'No such pattern: [{}]'.format(s)

    res = ins.split(' ', 1)
    if len(res) == 1:
        return ins, []

    ins_head, args = res
    return ins_head, list(arg_iter(args))

assert len(sys.argv) > 1, 'Give me more arguments'
assert os.path.exists(sys.argv[1]), 'No such file'
assert sys.argv[1].endswith('.mini'), 'Invalid extension'
assert os.path.basename(sys.argv[1]).startswith('_'), 'Bad naming convention'

with open(sys.argv[1], 'rb') as f:
    data = f.read().split('\n')
    data = [trim(remove_comment(i)) for i in data]
    data = [i for i in data if i != '']


for ins in data:
    if ins.endswith(':'):
        l = trim(ins.split(':', 1)[0])
        label[l] = len(code)
    else:
        code.append(ins)

code = map(parse_arguments, code)

dirname = os.path.dirname(sys.argv[1])
basename = os.path.basename(sys.argv[1])[1:-5]

fn = os.path.join(dirname, basename)
with open('{}.mini'.format(fn), 'wb') as f:
    f.write('\n'.join(trim('{} {}'.format(h, ', '.join(a))) for h, a in code))

for a, b, c in os.walk('../'):
    if a.endswith('compiler'):
        break
os.system('ocaml str.cma ./{}/compiler.ml {}.mini {}.out'.format(a, fn, fn))
