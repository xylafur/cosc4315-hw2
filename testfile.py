
def f():
   def p():
      pass
   x=1
   y=2
   if (x==1):
      t= x+y
   return t

x=1+3*100/2+f()
y=x*100+10*3.1416
print("x=", x)
print("y=", y)
