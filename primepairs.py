# -*- coding: utf-8 -*-
"""
Created on Thu Oct  1 15:39:34 2020

@author: karlv
"""

import numpy as np
from sympy import isprime
from copy import copy

def create_dict(n):
   connections = {}
   for x in range(1,n+1):
       connections[x] = set()
       for y in range(1, n+1):
           if x == y:
               continue
           if isprime(x+y):
               connections[x].add(y)
   return connections

# find the Hamiltonian path through the graph (finding if it exists is NP-complete)
    
class constructor:
    def __init__(self, b, s, start):
        self.brackets = b
        self.size = s
        self.start = start
        
    def str(self):
        return self.start + "-start, " + str(self.brackets) + " brackets of size: " + str(self.size)
        

def get_constructors(m : int, constructors : list, start = "low"):        
    # m must be even; start is "low" or "high" or "both"
    # bracket size and bracket number are both minimum 2
    i = 2
    
    if start.lower() == "both":
        while i < np.sqrt(m):
            if m % i == 0:
                # s must be even
                for b, s in [(i, m // i), (m // i, i)]:
                    if s % 2 == 0:
                        if (isprime(s + 3) and isprime(b * s + 5) 
                            and isprime((b + 1) * s + 5)):
                            constructors.append(constructor(b, s, "Low"))
                            
                        if (isprime((b - 1) * s + 1) and isprime(b * s + 1) 
                            and isprime((2 * b - 1) * s + 3)):
                            constructors.append(constructor(b, s, "High"))
            i += 1
            
    elif start.lower() == "low":
        while i < np.sqrt(m):
            if m % i == 0:
                for b, s in [(i, m // i), (m // i, i)]:
                    if s % 2 == 0:
                        if (isprime(s + 3) and isprime(b * s + 5) 
                            and isprime((b + 1) * s + 5)):
                            constructors.append(constructor(b, s, "Low"))
            i += 1
       
    elif start.lower() == "high":
        while i < np.sqrt(m):
            if m % i == 0:
                for b, s in [(i, m // i), (m // i, i)]:
                    if s % 2 == 0:
                        if (isprime((b - 1) * s + 1) and isprime(b * s + 1) 
                            and isprime((2 * b - 1) * s + 3)):
                            constructors.append(constructor(b, s, "High"))
            i += 1
    
    

def find_b_S(n):
    constructors = []
    # find any low-start
    for m in [n - 4, n - 3, n - 2]:
        if not m % 2 == 0:
           continue
        
        get_constructors(m, constructors, "low")
        
    # find any high-start
    for m in [n - 2, n - 1, n]:
        if not m % 2 == 0:
           continue
        
        get_constructors(m, constructors, "high")
        
    for con in constructors:
        print(con.start + "-start", "b =", con.brackets, "s =", con.size)
    return constructors

def constructors_up_to(n, verbose = True):
    up_to = []
    for m in range(0, n+1, 2):
        get_constructors(m, up_to, "both")
        
    values = {}
    for con in up_to:
        if con.start.lower() == "low":
            for n in range(con.brackets * con.size + 2, con.brackets * con.size + 5):
                if n in values.keys():
                    values[n].append(con)
                else:
                    values[n] = [con]
        if con.start.lower() == "high":
            for n in range(con.brackets * con.size, con.brackets * con.size + 3):
                if n in values.keys():
                    values[n].append(con)
                else:
                        values[n] = [con]
    if verbose:
        for val in values:
            print(val, end=": ")
            for con in values[val]:
                print(con.str(), end=";  ")
            print()
        
    return values 
