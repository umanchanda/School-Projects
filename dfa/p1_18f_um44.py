# Uday Manchanda
# UCID: um44
# Student ID: 31225402
# Program that checks a given URL to see whether it fits the DFA

import sys
import string

args = sys.argv

print("Project 1 for CS 341")
print("Semester: Fall 2018")
print("Written by: Uday Manchanda, um44")
print("Instructor: Marvin Nakayama, marvin@njit.edu\n")

def start():
    if len(args) == 1:
        choice = input("Would you like to enter a string: ")
        if choice[0] == 'y' and choice[0] != 'n':
            url = input("Please enter a string: ")
            if urlcheck(url):
                print("String is accepted\n")
            else:
                print("String is rejected\n")
        elif choice[0] == 'n':
            print("Goodbye")
        else:
            print("I didn't understand that")
            choice = input("Would you like to enter a string: ")
            while choice[0] != 'y' or choice[0] != 'n':
                choice = input("Would you like to enter a string: ")

    elif len(args) == 2:
        try:
            with open(str(args[1]), 'r') as fread:
                while fread.readline()[0] == 'y':
                    url = fread.readline()
                    if urlcheck(url):
                        print("String is accepted\n")
                    else:
                        print("String is rejected\n")
                fread.close()
        except FileNotFoundError:
            print("File not found")

    else:
        print("Goodbye")
        
def urlcheck(url):
    state = 0
    accept = False
    print("The URL is: " + url)
    print("The current state is q" + str(state))
    for c in url:
        if c == 'w' and state == 0:
            state = 1
            print(printstate(state, c))
        elif c in string.ascii_lowercase and c != 'w' and state == 0:
            state = 6
            print(printstate(state, c))
        elif c == 'w' and state == 1:
            state = 2
            print(printstate(state, c))
        elif c in string.ascii_lowercase and c != 'w' and state == 1:
            state = 6
            print(printstate(state, c))
        elif c == 'w' and state == 2:
            state = 3
            print(printstate(state, c))
        elif c == '.' and state == 3:
            state = 4
            print(printstate(state, c))
        elif c == '.' and state == 6:
            state = 7
            print(printstate(state, c))
        elif c in string.ascii_lowercase and c != '.' and state == 4:
            state = 5
            print(printstate(state, c))
        elif c == '.' and state == 5:
            state = 7
            print(printstate(state, c))
        elif c == 'c' and state == 7:
            state = 8
            print(printstate(state, c))
        elif c == 'o' and state == 8:
            state = 9
            print(printstate(state, c))
        elif c == 'm' and state == 9:
            state = 10
            print(printstate(state, c))
        elif c == '.' and state == 10:
            state = 11
            print(printstate(state, c))
        elif c == 'c' and state == 11:
            state = 12
            print(printstate(state, c))
        elif c == 'o' and state == 12:
            state = 13
            print(printstate(state, c))
        else:
            print(printstate(state, c))
    
    if state == 8 or state == 9 or state == 10 or state == 13:
        accept = True
    
    return accept
        
def printstate(state, c):
    return("The current state is q" + str(state) + " -> " + c)

start()