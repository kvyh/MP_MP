# -*- coding: utf-8 -*-
"""
Created on Wed May 27 09:36:31 2020

@author: karlv
"""
import copy
import numpy as np
import time

def first_place_pieces(side_len, placed = [], used = []):
    
    for place in possible_spots(side_len, placed, used):
        placed.append(place)
        new_used = copy.copy(used)
        for dist in get_distances(place, placed):
            new_used.append(dist)
        first_place_pieces(side_len, placed, new_used)
        
        # If a solution is found, return without popping. 
        if len(placed) == side_len:
            #print(placed)
            return
    if len(placed) == side_len:
            #print(placed)
            return
    placed.pop(-1)


def place_pieces(side_len, placed = [], used = []):
    pieces = side_len
    if len(placed) == pieces:
        #print(placed)
        return
    
    for place in possible_spots(side_len, placed, used):
        new_pl = copy.copy(placed)
        new_pl.append(place)
        new_used = copy.copy(used)
        for dist in get_distances(place, placed):
            new_used.append(dist)
        place_pieces(side_len, new_pl, new_used)
        
        
def possible_spots(side_len, placed, used):
    possible = []
    
    for i in range(side_len):
        for j in range(side_len):
            if ((i,j) in placed):
                continue
            new_dist = get_distances((i,j), placed)
            if len(new_dist) == len(set(new_dist)) and not any((dist in used) for dist in new_dist):
                possible.append((i,j))
    return possible
            

def get_distances(new_piece, pieces):
    distances = []
    for piece in pieces:
        distances.append(np.sqrt((piece[0]-new_piece[0])**2 + (piece[1]-new_piece[1])**2))
    return distances