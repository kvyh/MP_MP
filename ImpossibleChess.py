# -*- coding: utf-8 -*-
"""
Created on Wed Jul  8 19:26:10 2020

@author: karlv
"""
from copy import copy
import math

# generate boards for "impossible chessboard"

def valuate(board):
    return sum(2**i * x for i, x in enumerate(board))


def two_switch_unequal(disallowed, board, value):
    total = sum(board)
    for n in range(len(board)):
        if board not in disallowed[total][n]:
            disallowed[total][n].append(board)
    
    for first in range(len(board)):
        
        for second in range(len(board)):
            if second == first:
                continue
            
            if board[first] == False and board[second] == False:
                two_switch = copy(board)
                two_switch[first] = True
                two_switch[second] = True
                
                if two_switch not in disallowed[total + 2][value]:                
                    disallowed[total + 2][value].append(two_switch)
            
            elif board[first] == False and board[second] == True:
                two_switch = copy(board)
                two_switch[first] = True
                two_switch[second] = False
                
                if two_switch not in disallowed[total][value]:                
                    disallowed[total][value].append(two_switch)
            
            elif board[first] == True and board[second] == False:
                two_switch = copy(board)
                two_switch[first] = False
                two_switch[second] = True
                
                if two_switch not in disallowed[total][value]:                
                    disallowed[total][value].append(two_switch)
            
            
def adjacent_needed(board, known):
    base = set(x for x in range(len(board)))
    for x in range(len(board)):
        if board[x]:
            val = valuate(board) - 2**x
            
        else:
            val = valuate(board) + 2**x
        
        if val in known:
            base.remove(known[val])
    return base

def generate_dict(side_len):
    """
    process: 
    000... = 0
    100... = 0 
    0100.. = 1 ...
    1. check border for previously defined boards
    2. for each board iterate through False spaces
    3. create a new board with a True instead of the False if there is a valid value
    4. when a new board is defined, no board 2 steps away from it can be the same value
     
    
    """ 
    
    squares = side_len ** 2
    basic = [False for x in range(squares)]
    func = {valuate(basic): 0}
    
    disallowed = {}
    for total in range(squares + 1):
        disallowed[total] = {}
        for digit in range(squares):
            disallowed[total][digit] = []
    two_switch_unequal(disallowed, basic, 0)
    
    border = [(basic, 0)]
    
    for board_tot in range(squares):
        new_border = []
        
        for board, value in border:
            
            
            new_tot = sum(board) + 1
            for digit in range(squares):
                if board[digit] == False:
                    newb = copy(board)
                    newb[digit] = True
                    for val in range(squares):
                        if val in adjacent_needed(board, func) and newb not in disallowed[new_tot][val]:
                            new_border.append((newb, val))
                            two_switch_unequal(disallowed, newb, val)
                            func[valuate(newb)] = val
        border  = new_border
        # Remove disallowances that will no longer be relevant
        disallowed[board_tot] = {} 
        
    return func
    
    
def nice_out(func):
    keys = sorted(func.keys())
    blocks = int(math.log(len(keys), 2))
    
    failed = 0
    for block in range(2 ** blocks):
        if block not in keys:
            failed += 1
    
    for key in keys:
        print(key)
        for digit in range(blocks)[::-1]:
            if digit == 0:
                print(key % 2, end = "")
            else:
                print((key % (2 ** (digit + 1))) // 2 ** (digit), end = "")
        print(":", func[key])
    
    print("At least ", failed, "configurations without a valid value.")
    
