def f():
   x = 3 * 2
   y = 3 - 2
   z = 3 + 2
   l = x
   if y - l:
   # comment fails here
      l = y
   if z > l:
# also fails because of bad indent comment but also '>' comparison fails
      l = z
   return l

print(f())

