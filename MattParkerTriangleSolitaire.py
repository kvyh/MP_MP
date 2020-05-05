# -*- coding: utf-8 -*-
"""


Created on Wed Apr 29 10:46:59 2020

@author: kvyh

Thinking about the board as if it is:
0
1 2
3 4 5
6 7 8 9
...
"""
import copy
import math
import argparse

import cProfile, pstats, io
from pstats import SortKey

Shortest = 0

def find_unique_starts(side_length):
    starts = []
    # nested triangles as layers, the top half (rounded up) of the side is unique
    layer_peak = 0
    for layer in range(math.ceil(side_length/3)):
        layer_peak += layer * 4
        layer_side = side_length - layer * 3
        for row in range(math.ceil(layer_side/2)):
            starts.append(layer_peak + row * (layer * 2)
                                 + sum([x for x in range(row + 1)]))
    return starts

# I'm treating 1-3-6 and 6-3-1 as the same, handling the duality later
def get_moves(side_length):
    possible_moves = []
    start_of_row = 0
    # row of move's start
    for n in range(side_length):
        start_of_row += n
        # column of move's start
        for m in range(n+1):
            start = start_of_row + m
            # 'down' and 'angled'
            if side_length >= 1 + (n + 2):
                possible_moves.append((start, start + n + 1, start + 2*n + 3))
                possible_moves.append((start, start + n + 2, start + 2*n + 5))                
            # horizontal
            if n >= m + 2:
                possible_moves.append((start, start + 1, start + 2))
    return possible_moves
    
# loop: get board state, list possible jumps, iterate through them
def play_game(board_state, current_dict, previous_moves, success_list):
    for move in board_state.valid_moves:
        current_dict[move] = {}
        cur_stones = copy.copy(board_state.stones)
        cur_stones[move[0]] = False
        cur_stones[move[1]] = False
        cur_stones[move[2]] = True
        moves_list = copy.copy(previous_moves)
        moves_list.append(move)
        play_game(board(cur_stones), current_dict[move], moves_list, success_list)
    # if the game is successfully finished, add the move sequence to our list
    if board_state.valid_moves == [] and sum(board_state.stones) == 1:
        concat_moves = [[previous_moves[0]]]
        for move in previous_moves[1:]:
            if move[0] == concat_moves[-1][-1]:
                concat_moves[-1].append(move[2])
            else:
                concat_moves.append([move[0], move[2]])
        success_list.append(concat_moves)
  
def play_game_limited(board_state, previous_moves, success_list, max_listed):
    for move in board_state.valid_moves:
        cur_stones = copy.copy(board_state.stones)
        cur_stones[move[0]] = False
        cur_stones[move[1]] = False
        cur_stones[move[2]] = True
        moves_list = copy.copy(previous_moves)
        moves_list.append(move)
        play_game_limited(board(cur_stones), moves_list, success_list, max_listed)
    
    if board_state.valid_moves == [] and sum(board_state.stones) == 1:
        global Shortest
        concat_moves = [[previous_moves[0]]]
        for move in previous_moves[1:]:
            if move[0] == concat_moves[-1][-1]:
                concat_moves[-1].append(move[2])
            else:
                concat_moves.append([move[0], move[2]])
        # Only have the shortest solutions in success_list
        if len(concat_moves) < Shortest:
            #print("New shortest: ", end = "")
            #print(concat_moves)
            success_list.clear()
            success_list.append(concat_moves)
            Shortest = len(concat_moves)
        elif len(concat_moves) == Shortest and len(success_list) < max_listed:
            success_list.append(concat_moves)
            #print("Equal length: ", end = "")
            #print(concat_moves)
        #elif len(concat_moves) == Shortest:
            #print("Equal length: ", end = "")
            #print(concat_moves)
    
    
class board:
    def __init__(self, stones : list):
        self.stones = stones
        self.valid_moves = self.moves()
    
    def moves(self):
        valid = []
        for move in Possible_moves:
            if self.stones[move[1]]:
                if self.stones[move[0]] and not self.stones[move[2]]:
                    valid.append(move)
                elif self.stones[move[2]] and not self.stones[move[0]]:
                    valid.append((move[2], move[1], move[0]))
        return valid
    
      
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description = "solitaire solver")
    
    parser.add_argument("side_len", type = int, default = 4)
    parser.add_argument("-m","--max_listed", type = int, default = 10,
                        help = 'How many solutions the program will print.')
    parser.add_argument("-s","--memory_limited", type = bool, default = False)
    args = parser.parse_args()
    
    side_len = args.side_len
        
    print("Analyzing Board:")
    if side_len <= 4:
        spacing = 1
    elif side_len < 14:
        spacing = 2
    else:
        spacing = 3
        
    row_start = 0
    for row in range(side_len):
        row_start += row        
        for colu in range(row + 1):
            if row_start > 0:
                print(row_start + colu, end = " " * (spacing - int(math.log(row_start + colu, 10))))
            else:
                print(0, end = "")
        print()
    print()
    
    stones = sum([n+1 for n in range(side_len)])
    Shortest = stones
    
    unique_starts = find_unique_starts(side_len)
    
    print("Unique Starting Positions:")
    print(unique_starts)
    print()
    
    Possible_moves = get_moves(side_len)
    print("All Potentially Valid Moves:")
    print(Possible_moves)
    
    complete_games = []
    
    pr = cProfile.Profile()
    pr.enable()
    if not args.memory_limited:
        Moves_dict = {}
        for x in range(stones):
            Moves_dict[x] = {}
        
        # start with only the unique positions
        for start in unique_starts:
            starting_stones = [True for s in range(stones)]
            starting_stones[start] = False
            
            play_game(board(starting_stones), Moves_dict[start], [start], complete_games)
            
        min_length = len(starting_stones)
        for lis in complete_games:
            if len(lis) < min_length:
                min_length = len(lis)
        print()
        print("Minimum Solution Length: " + str(min_length))
        no_printed = 0
        for lis in complete_games:
            if no_printed >= args.max_listed:
                print("...")
                break
            if len(lis) == min_length:
                print(lis)
                no_printed += 1
    else:
        for start in unique_starts:
            starting_stones = [True for s in range(stones)]
            starting_stones[start] = False
            
            play_game_limited(board(starting_stones), [start], complete_games, args.max_listed)
        
        print()
        print("Minimum Solution Length: " + str(Shortest))
        for lis in complete_games:
            print(lis)
    pr.disable()
    s = io.StringIO()
    sortby = SortKey.CUMULATIVE
    ps = pstats.Stats(pr, stream=s).sort_stats(sortby)
    ps.print_stats()
    print(s.getvalue()[:2000])
