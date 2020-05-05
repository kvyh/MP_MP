// TriangleSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <array>

const int Max_listed = 15;
const int Side_length = 6;
const int Stones = (Side_length * (Side_length + 1)) / 2;
int Shortest = Stones;

int* unique_starts(int side, int* starts, int* p_len_starts);
template <size_t N>
std::array<int[3], N> valid_moves(int side, std::array<int[3], N> &moves, int* p_st_count);
template <size_t N>
void print_foreward(int side, int* starts, int len_starts, std::array<int[3], N> moves, int n_moves);
template <size_t N>
void play_game(bool board[Stones], std::array<int[3], Stones - 2>& previous_moves, int on_move, std::array<std::array<int[3], Stones-2>, Max_listed>& solutions, int* p_num_sols, std::array<int[3], N> moves, int num_moves, int max_listed);
void print_result(std::array<std::array<int[3], Stones - 2>, Max_listed>& solutions, int num_sols, int max_listed);

int main()
{    
    int len_starts = 0;
    int arr_starts[(int) (Stones / 3)];
    unique_starts(Side_length, arr_starts, &len_starts);
    
    int num_moves = 0;
    std::array<int[3], Stones * 3> moves{};
    valid_moves(Side_length, moves, &num_moves);

    print_foreward(Side_length, arr_starts, len_starts, moves, num_moves);

    std::array<std::array<int[3], Stones - 2>, Max_listed> solutions{};
    int num_solutions = 0;
    len_starts = 1;
    for (int i = 0; i < len_starts; i += 1) {
        bool board[Stones];
        for (int st = 0; st < Stones; st += 1) {
            board[st] = true;
        }
        board[arr_starts[i]] = false;
        std::array<int[3], Stones - 2> prev_moves{};
        play_game(board, prev_moves, 0, solutions, &num_solutions, 
            moves, num_moves, Max_listed);
    }

    print_result(solutions, num_solutions, Max_listed);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
bool* perform_move(bool brd[Stones], bool* new_board, std::array<int[3], Stones - 2>& previous_moves, int on_move, int mv_st, int mv_mid, int mv_end) {
    for (int idx = 0; idx < Stones; idx += 1) {
        new_board[idx] = brd[idx];
    }
    new_board[mv_st] = false;
    new_board[mv_mid] = false;
    new_board[mv_end] = true;
    previous_moves[on_move][0] = mv_st;
    previous_moves[on_move][1] = mv_mid;
    previous_moves[on_move][2] = mv_end;
    
    return new_board;
}

int bsum(bool* lis, int leng) {
    int s = 0;
    for (int i = 0; i < leng; i += 1) {
        s += lis[i];
    }
    return s;
}

void add_solution(std::array<int[3], Stones - 2>& previous_moves, std::array<std::array<int[3], Stones - 2>, Max_listed>& solutions, int* p_num_sols, int max_listed) {
    int length = 1;
    int move_end = previous_moves[0][2];
    for (int i = 1; i < Stones - 2; i += 1) {
        if (!(previous_moves[i][0] == move_end)) {
            length += 1;
        }
        move_end = previous_moves[i][2];
    }
    if (length < Shortest) {
        // std::cout << "New Shortest(" << length << "): ";
        for (int j = 0; j < Stones - 2; j += 1) {
            for (int i = 0; i < max_listed; i += 1) {
                solutions[i][j][0] = 0;
                solutions[i][j][1] = 0;
                solutions[i][j][2] = 0;
            }
            solutions[0][j][0] = previous_moves[j][0];
            solutions[0][j][1] = previous_moves[j][1];
            solutions[0][j][2] = previous_moves[j][2];
            // std::cout << previous_moves[j][0] << previous_moves[j][1] << previous_moves[j][2] << "  ";
        }
        *p_num_sols = 1;
        Shortest = length;
        // std::cout << "\n";
    }
    else if ((length == Shortest) && (*p_num_sols < max_listed)) {
        // std::cout << "New(" << length << "): ";
        for (int j = 0; j < Stones - 2; j += 1) {
            solutions[*p_num_sols][j][0] = previous_moves[j][0];
            solutions[*p_num_sols][j][1] = previous_moves[j][1];
            solutions[*p_num_sols][j][2] = previous_moves[j][2];
            // std::cout << previous_moves[j][0] << previous_moves[j][1] << previous_moves[j][2] << "  ";
        }
        *p_num_sols += 1;
        // std::cout << "\n";
    }
    if (length == Shortest) {
        std::cout << "New(" << length << "): ";
        for (int j = 0; j < Stones - 2; j += 1) {
            std::cout << previous_moves[j][0] << previous_moves[j][1] << previous_moves[j][2] << "  ";
        }
        std::cout << "\n";
    }
}

template <size_t N>
void play_game(bool board[Stones], std::array<int[3], Stones - 2>& previous_moves, int on_move, std::array<std::array<int[3], Stones - 2>, Max_listed> &solutions, int* p_num_sols, std::array<int[3], N> moves, int num_moves, int max_listed) {
    /*
    for (int i = 0; i < Stones; i += 1) {
        // std::cout << board[i];
    }
    // std::cout << "\n"; */
    
    for (int i = 0; i < num_moves; i += 1) {
        if (board[moves[i][1]]) {
            if (board[moves[i][0]] && !board[moves[i][2]]) {
                bool new_board[Stones];
                perform_move(board, new_board, previous_moves, on_move, moves[i][0], moves[i][1], moves[i][2]);
                
                play_game(new_board, previous_moves, on_move + 1, solutions, p_num_sols, moves, num_moves, max_listed);
            }
            else if (board[moves[i][2]] && !board[moves[i][0]]) {
                bool new_board[Stones];
                perform_move(board, new_board, previous_moves, on_move, moves[i][2], moves[i][1], moves[i][0]);
                
                play_game(new_board, previous_moves, on_move + 1, solutions, p_num_sols, moves, num_moves, max_listed);
            }
        }
    }

    if (bsum(board, Stones) == 1) {
        add_solution(previous_moves, solutions, p_num_sols, max_listed);
        
    }
}

int* unique_starts(int side, int* starts, int* p_len_starts) {
    int idx = 0;
    int layer_peak = 0;
    // // std::cout << side / 3. << "   " << ceil(side / 3.);
    for (int i = 0; i < ceil(side / 3.); i += 1) {
        layer_peak += i * 4;
        int row_range = ceil((side - i * 3) / 2.);
        for (int row = 0; row < row_range; row += 1) {
            starts[*p_len_starts] = layer_peak + row * (i * 2) + (row * (row + 1)) / 2;
            *p_len_starts += 1;  
        }
    }
    return starts;
}

template <size_t N>
std::array<int[3], N> valid_moves(int side, std::array<int[3], N> &p_moves, int* p_st_count) {
    int st_row = 0;
    for (int i = 0; i < side; i += 1) {
        st_row += i;
        for (int j = 0; j < i + 1; j += 1) {
            int st = st_row + j;
            if (side >= 1 + (i + 2)) {
                p_moves[*p_st_count][0] = st;
                p_moves[*p_st_count][1] = st + i + 1;
                p_moves[*p_st_count][2] = st + 2 * i + 3;
                *p_st_count += 1;
                p_moves[*p_st_count][0] = st;
                p_moves[*p_st_count][1] = st + i + 2;
                p_moves[*p_st_count][2] = st + 2 * i + 5;
                *p_st_count += 1;
            }
            if (i >= j + 2) {
                p_moves[*p_st_count][0] = st;
                p_moves[*p_st_count][1] = st + 1;
                p_moves[*p_st_count][2] = st + 2;
                *p_st_count += 1;
            }
        }
    }
    return p_moves;
}

template <size_t N>
void print_foreward(int side, int* starts, int len_starts, std::array<int[3], N> moves, int n_moves) {
    // std::cout << "Analyzing Board:\n";
    int spacing = (int)log10(Stones - 1) + 1;
    int rst = 0;
    for (int r = 0; r < side; r += 1) {
        rst += r;
        for (int c = 0; c < r + 1; c += 1) {
            if (rst > 0) {
                // std::cout << (rst + c);
                for (int sp = 0; sp < spacing - log10(rst + c); sp += 1) {
                    // std::cout << " ";
                }
            }
            else {
                // std::cout << "0";
            } 
        }
        // std::cout << "\n";
    }
    // std::cout << "\n";

    // std::cout << "Unique Starting Positions:\n";
    for (int i = 0; i < len_starts; i += 1) {
        // std::cout << starts[i] << "  ";
    }
    // std::cout << "\n";

    // std::cout << "Potentially Valid Moves:\n";
    for (int i = 0; i < n_moves; i += 1) {
        // std::cout << moves[i][0] << "," << moves[i][1] << "," << moves[i][2] << "\t";
    }
    // std::cout << "\n" << "\n";
}

void print_result(std::array<std::array<int[3], Stones - 2>, Max_listed>& solutions, int num_sols, int max_listed) {
    // std::cout << "Shortest solution length: " << Shortest << "\n";
    for (int i = 0; i < num_sols; i += 1) {
        // std::cout << "[" << solutions[i][0][0] << "-" << solutions[i][0][2];
        int chain_end = solutions[i][0][2];
        for (int j = 0; j < Stones - 2; j += 1) {
            if (solutions[i][j][0] == chain_end) {
                chain_end = solutions[i][j][2];
                // std::cout << "-" << chain_end;
            }
            else {
                chain_end = solutions[i][j][2];
                // std::cout << ", " << solutions[i][j][0] << "-" << chain_end;
            }
        }
        // std::cout << "]\n";
    }
}