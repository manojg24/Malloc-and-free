print("enter the number")
x=input("Num: ")
x=int(x)
print("Enter Mul/Div for Multiplication/Division by 2")

s =(input("Operation: \n"))
if((s) == 'Mul'):
    x = x * 2
elif (s == 'Div'):
    x = x / 2
else:
    print("opration not supported")

print("Result is" + str(x))

