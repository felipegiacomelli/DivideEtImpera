import sys, os, subprocess

if len(sys.argv) == 2:
    name = sys.argv[1]
else:
    name = "a"

y_top = 800
y_max = 760
y_min = 5

x_min = 90
x_max = 1280

lenx = x_max - x_min
leny = y_max - y_min

command = "convert -crop %ix%i+%i+%i %s.png %s1.png" % (lenx, leny, x_min, y_top-y_max, name, name)
subprocess.check_call(["sh", "-c", "%s" % command])
print("\n%s\n" % command)

command = "convert %s1.png -resize 400 %s1.png" % (name, name)
subprocess.check_call(["sh", "-c", "%s" % command])
print("\n%s\n" % command)
