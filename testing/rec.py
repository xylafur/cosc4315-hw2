def f(n):
   return n

def f1(n):
   if(n == 0):
      return n
   else:
      return f1(n + 1)

def f2(n):
   if(n == 0):
      return f2(n + 1)
   else:
      return 1

f(1)
f1(1)  # no
f1(-5-2) # no
f1(-5) # no
f2(1)  # yes
f2(0)  # yes
