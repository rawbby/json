import re

auto_decouple = False

step_repl = 'switch(is.get()){'
step_end_repl = 'default:FAIL();}'
steps_repl = 'for(;;){switch(is.get()){'
steps_end_repl = 'default:FAIL();}}'

step = '%step%'
step_end = '%step_end%'
steps = '%steps%'
steps_end = '%steps_end%'

functions = {}
uid_counter = 0


def register_name(c, name):
    global functions
    if auto_decouple:
        c = decouple(c)
    functions[finalise(c)] = name
    return name


def register(c):
    global uid_counter
    global functions
    if auto_decouple:
        c = decouple(c)
    c = finalise(c)
    if c in functions:
        return functions[c]
    uid_counter += 1
    return register_name(c, f"f{uid_counter}")


decouple_re = re.compile(steps + r"[^%]*" + steps_end, re.DOTALL)

step_re = re.compile(step, re.DOTALL)
step_end_re = re.compile(step_end, re.DOTALL)
steps_re = re.compile(steps, re.DOTALL)
steps_end_re = re.compile(steps_end, re.DOTALL)


def decouple_repl(match):
    if match.group(0) == match.string:
        return match.string
    return f"return {register(match.group(0))}(is);"


def decouple(c):
    n = decouple_re.sub(decouple_repl, c)
    return n if n == c else decouple(n)


def finalise(c):
    c = step_re.sub(step_repl, c)
    c = step_end_re.sub(step_end_repl, c)
    c = steps_re.sub(steps_repl, c)
    return steps_end_re.sub(steps_end_repl, c)


empty = {}

case_one_nine = "case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':"
case_digit = "case'0':" + case_one_nine
case_ws = "case' ':case'\\r':case'\\n':case'\\t':"
case_hex = case_digit + "case'a':case'b':case'c':case'd':case'e':case'f':case'A':case'B':case'C':case'D':case'E':case'F':"


def post_(buffer: [str], *args):
    if args:
        args[0](buffer, *args[1:])


def eof_(buffer: [str], *args):
    assert not args
    buffer.append("case EOF:return 0;")


def ws_(buffer: [str], *args):
    buffer.append(case_ws)
    buffer.append("continue;")
    post_(buffer, *args)


def digits_(buffer: [str], *args):
    buffer.append(case_digit)
    buffer.append(steps)
    buffer.append(case_digit)
    buffer.append("continue;")
    post_(buffer, *args)
    buffer.append(steps_end)


def exponent_(buffer: [str], *args):
    buffer.append("case'e':case'E':" + steps)
    buffer.append("case'+':case'-':" + steps)
    post_(buffer, digits_, *args)
    buffer.append(steps_end)
    post_(buffer, digits_, *args)
    buffer.append(steps_end)
    post_(buffer, *args)


def fraction_(buffer: [str], *args):
    buffer.append("case'.':" + steps)
    post_(buffer, digits_, *args)
    buffer.append(steps_end)
    post_(buffer, *args)


def integer_(buffer: [str], *args):
    buffer.append("case'0':" + steps)
    post_(buffer, *args)
    buffer.append(steps_end)
    buffer.append(case_one_nine)
    buffer.append(steps)
    buffer.append(case_digit)
    buffer.append("continue;")
    post_(buffer, *args)
    buffer.append(steps_end)
    buffer.append("case'-':" + steps)
    buffer.append("case'0':" + steps)
    post_(buffer, *args)
    buffer.append(steps_end)
    buffer.append(case_one_nine)
    buffer.append(steps)
    buffer.append(case_digit)
    buffer.append("continue;")
    post_(buffer, *args)
    buffer.append(steps_end)
    buffer.append(steps_end)


def number_(buffer: [str], *args):
    post_(buffer, integer_, fraction_, exponent_, *args)


def true_(buffer: [str], *args):
    buffer.append("case't':" + steps)
    buffer.append("case'r':" + steps)
    buffer.append("case'u':" + steps)
    buffer.append("case'e':" + steps)
    post_(buffer, *args)
    buffer.append(steps_end)
    buffer.append(steps_end)
    buffer.append(steps_end)
    buffer.append(steps_end)


def false_(buffer: [str], *args):
    buffer.append("case'f':" + steps)
    buffer.append("case'a':" + steps)
    buffer.append("case'l':" + steps)
    buffer.append("case's':" + steps)
    buffer.append("case'e':" + steps)
    post_(buffer, *args)
    buffer.append(steps_end)
    buffer.append(steps_end)
    buffer.append(steps_end)
    buffer.append(steps_end)
    buffer.append(steps_end)


def null_(buffer: [str], *args):
    buffer.append("case'n':" + steps)
    buffer.append("case'u':" + steps)
    buffer.append("case'l':" + steps)
    buffer.append("case'l':" + steps)
    post_(buffer, *args)
    buffer.append(steps_end)
    buffer.append(steps_end)
    buffer.append(steps_end)
    buffer.append(steps_end)


# TODO support also extended ascii
def string_(buffer: [str], *args):
    buffer.append(f"case'\"':")
    buffer.append(steps)
    for c in [i for i in range(0x20, 0x80) if i not in (0x5C, 0x22)]:
        buffer.append(f"case {hex(c)}:")
    buffer.append("continue;")
    buffer.append("case'\\\\':" + step)
    buffer.append("case'\\\"':")
    buffer.append("case'\\\\':")
    buffer.append("case'/':")
    buffer.append("case'b':")
    buffer.append("case'f':")
    buffer.append("case'n':")
    buffer.append("case'r':")
    buffer.append("case't':")
    buffer.append("continue;")
    buffer.append("case'u':" + step)
    buffer.append(case_hex + step)
    buffer.append(case_hex + step)
    buffer.append(case_hex + step)
    buffer.append(case_hex + "continue;")
    buffer.append(step_end)
    buffer.append(step_end)
    buffer.append(step_end)
    buffer.append(step_end)
    buffer.append(step_end)
    buffer.append(f"case'\\\"':")
    buffer.append(steps)
    post_(buffer, *args)
    buffer.append(steps_end)
    buffer.append(steps_end)



empty[exponent_] = True
empty[fraction_] = True
