# Uday Manchanda
# UCID: um44
# Student ID: 31225402
# Program that checks a given expression to see whether it fits the PDA

import sys
import string

args = sys.argv

print("Project 2 for CS 341")
print("Semester: Fall 2018")
print("Written by: Uday Manchanda, um44")
print("Instructor: Marvin Nakayama, marvin@njit.edu\n")

def start():
    if len(args) == 1:
        choice = input("Would you like to enter a expression: ")
        if choice[0] == 'y' and choice[0] != 'n':
            expression = input("Please enter a expression: ")
            if expressioncheck(expression):
                print("Expression is accepted\n")
            else:
                print("Expression is rejected\n")
        elif choice[0] == 'n':
            print("Goodbye")
        else:
            print("I didn't understand that")
            choice = input("Would you like to enter a expression: ")
            while choice[0] != 'y' or choice[0] != 'n':
                choice = input("Would you like to enter a expression: ")
                expression = input("Please enter a expression: ")
                if expressioncheck(expression):
                    print("Expression is accepted\n")
                else:
                    print("Expression is rejected\n")

    elif len(args) == 2:
        try:
            with open(str(args[1]), 'r') as fread:
                while fread.readline()[0] == 'y':
                    expression = fread.readline()
                    if expressioncheck(expression):
                        print("Expression is accepted\n")
                    else:
                        print("Expression is rejected\n")
                fread.close()
        except FileNotFoundError:
            print("File not found")

    else:
        print("Goodbye")

def expressioncheck(expression):
    state = 0
    print("The expression is: " + expression)
    print("The current state is q" + str(state))

    stack = []
    numbers = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
    symbols = ['+', '-', '/', '%']

    for c in expression:

        if c == '$' and state == 0:
            state = 1
            stack.append('$')
            print(printstate(state, c))
            print(printpush(c))
            print(stack)
        
        elif c == '(' and state == 1:
            state = 1
            stack.pop()
            print(printpop('$'))
            stack.append('(')
            print(printpush(c))
            print(printstate(state, c))
            print(stack)

        elif c in numbers and state == 1:
            state = 3
            print(printstate(state, c))
            print(printpush('\u03B5'))
            print(printpop('\u03B5'))
            print(stack)
        
        elif c in numbers and state == 3:
            state = 3
            print(printstate(state, c))
            print(printpush('\u03B5'))
            print(printpop('\u03B5'))
            print(stack)
        
        elif c in symbols and state == 1:
            state = 2
            print(printstate(state, c))
            print(printpush('\u03B5'))
            print(printpop('\u03B5'))
            print(stack)
        
        elif c == ')' and state == 3:
            state = 4
            print(printstate(state, c))
            stack.pop()
            print(printpop('('))
            print(printpush('\u03B5'))
            print(stack)
        
        elif c == ')' and state == 2:
            state = 4
            print(printstate(state, c))
            stack.pop()
            print(printpop('('))
            print(printpush('\u03B5'))
            print(stack)
        
        elif c == '$' and state == 4:
            state = 5
            print(printstate(state, c))
            try:
                stack.pop()
            except:
                return False
            print(printpop('$'))
            print(printpush('\u03B5'))
            print(stack)

    return state == 5


def printstate(state, c):
    return("The current state is q" + str(state) + " -> " + c)

def printpush(c):
    return(c + " was pushed onto the stack")

def printpop(c):
    return(c + " was popped off the stack")

if __name__ == "__main__":
    start()