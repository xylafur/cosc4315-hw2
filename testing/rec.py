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

def f3(a,b,c):
   if a+b==0:
      return 1
   else:
      return f3(a-1,b-1,c)

f(1)
f1(1)  # no
f1(0)  # no
f2(1)  # yes
f2(0)  # yes
f3(2,1,2,3,4,5,6,7,8,9,10)
