from ctypes import *
import uuid


class Command(object):

    def __init__(self):
        ll = cdll.LoadLibrary
        self.lib = ll("./FileJNI.so")

    def mount(self):
        self.lib.mount()

    def mkdir(self, father):
        name = bytes(str(uuid.uuid1()), "utf-8")
        self.lib.mkdir1(name, father)

    def touch(self, father):
        name = bytes(str(uuid.uuid1()), "utf-8")
        self.lib.touch(name, father)

    def rm(self, no, father):
        self.lib.rm(no, father)

    def ls(self, no):
        print("_______")
        print(no)
        ls = self.lib.ls
        ls.restype = c_char_p
        result = ls(no)
        if result is not None:
            s = bytes.decode(ls(no))
            return [pair.split(":") for pair in s.split("|")][0:-1]
        return None

    def mv_in(self):
        self.lib.mv_in()

    def mv_out(self):
        self.lib.mv_out()

