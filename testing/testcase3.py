def f():
   print("recursive function")
   return f()

def f1(n):
   if n == 1:
      return 1
   else:
      return n * f1(n - 1)

def f2(a,b):
   return f2(a,b)

f()
f1(5)
f1(-1)
f2(-1,2)
