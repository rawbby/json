from json_trivial import *


def colon_(buffer: [str], *args):
    buffer.append("case':':" + steps)
    post_(buffer, *args)
    buffer.append(steps_end)


empty[colon_] = False


def object_(buffer: [str], *args):
    def call_object_inner_(buffer: [str], *args):
        assert len(args) == 0
        buffer.append("case',':")
        buffer.append("if(1==object_inner(is))FAIL();")
        buffer.append("case'}':return 0;")

    empty[call_object_inner_] = False

    if not hasattr(object_, "object_inner"):
        setattr(object_, "object_inner", 1)
        fb: [str] = []
        fb.append(steps)
        fb.append("case'}':return 0;")
        fb.append("case',':")
        post_(fb, ws_, string_, ws_, colon_, ws_, value_, ws_, call_object_inner_)
        fb.append(steps_end)

        register_name(''.join(fb), "object_inner")

    if not hasattr(object_, "object_outer"):
        setattr(object_, "object_outer", 1)
        fb: [str] = []
        fb.append(steps)
        fb.append("case'}':return 0;")
        post_(fb, ws_, string_, ws_, colon_, ws_, value_, ws_, call_object_inner_)
        fb.append(steps_end)

        register_name(''.join(fb), "object_outer")

    buffer.append("case'{':if(1==object_outer(is))FAIL();")
    buffer.append(steps)
    post_(buffer, *args)
    buffer.append(steps_end)


def array_(buffer: [str], *args):
    def call_array_inner_(buffer: [str], *args):
        assert len(args) == 0
        buffer.append("case',':")
        buffer.append("if(1==array_inner(is))FAIL();")
        buffer.append("case']':return 0;")

    empty[call_array_inner_] = False

    if not hasattr(array_, "array_inner"):
        setattr(array_, "array_inner", 1)
        fb: [str] = []
        fb.append(steps)
        fb.append("case']':return 0;")
        fb.append("case',':")
        post_(fb, ws_, value_, ws_, call_array_inner_)
        fb.append(steps_end)

        register_name(''.join(fb), "array_inner")

    if not hasattr(array_, "array_outer"):
        setattr(array_, "array_outer", 1)
        fb: [str] = []
        fb.append(steps)
        fb.append("case']':return 0;")
        post_(fb, ws_, value_, ws_, call_array_inner_)
        fb.append(steps_end)

        register_name(''.join(fb), "array_outer")

    buffer.append("case'[':if(1==array_outer(is))FAIL();")
    buffer.append(steps)
    post_(buffer, *args)
    buffer.append(steps_end)


def value_(buffer: [str], *args):
    array_(buffer, *args)
    object_(buffer, *args)
    string_(buffer, *args)
    number_(buffer, *args)
    true_(buffer, *args)
    false_(buffer, *args)
    null_(buffer, *args)


empty[array_] = False
empty[value_] = False
