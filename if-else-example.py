
a = 1
b = 1
a = 2
b = 1

def foo():
   a = 1
   b = 2
   a = 1
   b = 2

def bar(c):
   a = c
   c = 1

if True:
   print("Fuck")

if False:
   if True:
      print("yey")
      if False:
         if True:
            print("yey")
         else:
            if False:
               if True:
                  print("yey")
               else:
                  print('n0o')
            print('n0o')
   else:
      print('n0o')

if False:
   if True:
      print("yey")
   else:
      print('n0o')


