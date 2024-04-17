#!/usr/bin/python2.7
import os, sys, re, textwrap


EMBED_DIR  = "../src/embed"
EXTENSIONS = (".lua")
PATTERN    = "const unsigned char %s[] = {\n%s\n};"


def listdir(path):
  return [os.path.join(dp, f) for dp, dn, fn in os.walk(path) for f in fn if f.endswith(tuple(EXTENSIONS))]


def make_c_include(name, data):
  res = ""
  for c in data:
    res += str("0x{:02x}".format(ord(c))) + ", "
  res = res.rstrip(", ")

  name = re.sub("[^a-z0-9]", "_", name.lower())
  text = textwrap.fill(res, width=79, initial_indent='  ', subsequent_indent='  ')
  return PATTERN % (name, text)


def main():
  os.chdir(sys.path[0])

  for filename in listdir(EMBED_DIR):
    print(filename)
    name = os.path.basename(filename)
    text = make_c_include(name, open(filename).read())
    open("%s/%s.h" % (EMBED_DIR, name), "wb").write(text.encode())


if __name__ == "__main__":
  main()
