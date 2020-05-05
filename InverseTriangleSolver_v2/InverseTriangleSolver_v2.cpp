// InverseTriangleSolver_v2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// TriangleSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
Difference from v1: Instead of iterating over possible moves, iterate over board positions. 
Modify move array design such that moves can be accessed based on the middle position of the jump (which must be True for a valid move).
For position i, moves[i] = list of moves passing through i (e.g. moves[4] = [[1,4,8], [2,4,7], [3,4,5]])
*/

#include <iostream>
#include <cstdlib>
#include <array>

const int Max_listed = 15;
const int Side_length = 7;
const int Stones = (Side_length * (Side_length + 1)) / 2;
int Shortest = Stones;
int Long_chain = 0;

int* unique_starts(int side, int* starts, int* p_len_starts);
template <size_t N>
std::array<int[3][3], N> valid_moves(int side, std::array<int[3][3], N>& moves);
template <size_t N>
void print_foreward(int side, int* starts, int len_starts, std::array<int[3][3], N> moves);
template <size_t N>
void play_game(bool board[Stones], std::array<int[3], Stones - 2>& future_moves, int on_move, std::array<std::array<int[3], Stones - 2>, Max_listed>& solutions, int* p_num_sols, std::array<int[3][3], N> moves, int max_listed);
template <size_t N>
void chained_end(bool board[Stones], std::array<int[3], Stones - 2>& future_moves, int on_move, std::array<std::array<int[3], Stones - 2>, Max_listed>& solutions, int* p_num_sols, std::array<int[3][3], N> moves, int max_listed, int chains, int chained_len);
void print_result(std::array<std::array<int[3], Stones - 2>, Max_listed>& solutions, int num_sols, int max_listed);

int main()
{
    int len_starts = 0;
    int arr_starts[(int)(Stones / 3)];
    unique_starts(Side_length, arr_starts, &len_starts);

    std::array<int[3][3], Stones> moves{};
    valid_moves(Side_length, moves);

    print_foreward(Side_length, arr_starts, len_starts, moves);

    std::array<std::array<int[3], Stones - 2>, Max_listed> solutions{};
    int num_solutions = 0;

    for (int i = 0; i < len_starts; i += 1) {
        bool board[Stones];
        for (int st = 0; st < Stones; st += 1) {
            board[st] = true;
        }
        board[arr_starts[i]] = false;
        std::array<int[3], Stones - 2> fut_moves{};
        chained_end(board, fut_moves, 0, solutions, &num_solutions,
            moves, Max_listed, 0, Stones / 3 + 3);
    }

    print_result(solutions, num_solutions, Max_listed);
}


bool* perform_move(bool brd[Stones], bool new_board[Stones], std::array<int[3], Stones - 2>& future_moves, int on_move, int mv_st, int mv_mid, int mv_end) {
    for (int idx = 0; idx < Stones; idx += 1) {
        new_board[idx] = brd[idx];
    }
    new_board[mv_st] = false;
    new_board[mv_mid] = false;
    new_board[mv_end] = true;
    future_moves[on_move][0] = mv_st;
    future_moves[on_move][1] = mv_mid;
    future_moves[on_move][2] = mv_end;

    return new_board;
}

int bsum(bool* lis, int leng) {
    int s = 0;
    for (int i = 0; i < leng; i += 1) {
        s += lis[i];
    }
    return s;
}

void add_solution(std::array<int[3], Stones - 2>& future_moves, std::array<std::array<int[3], Stones - 2>, Max_listed>& solutions, int* p_num_sols, int max_listed) {
    int length = 1;
    int max_chain = 0;
    int chain_len = 1;
    int move_end = future_moves[Stones - 3][2];
    for (int i = Stones - 4; i >= 0; i -= 1) {
        if (!(future_moves[i][0] == move_end)) {
            length += 1;
            chain_len = 1;
        }
        else {
            chain_len += 1;
        }
        move_end = future_moves[i][2];
        if (chain_len > max_chain) {
            max_chain = chain_len;
        }
    }
    if (length < Shortest) {
        std::cout << "New Shortest(" << length << "): ";
        for (int j = Stones - 3; j >= 0; j -= 1) {
            int inv = Stones - 3 - j;
            for (int i = 0; i < max_listed; i += 1) {
                solutions[i][inv][0] = 0;
                solutions[i][inv][1] = 0;
                solutions[i][inv][2] = 0;
            }
            solutions[0][inv][0] = future_moves[j][0];
            solutions[0][inv][1] = future_moves[j][1];
            solutions[0][inv][2] = future_moves[j][2];
            std::cout << future_moves[j][0] << future_moves[j][1] << future_moves[j][2] << "  ";
        }
        *p_num_sols = 1;
        Shortest = length;
        std::cout << "\n";
    }
    else if ((length == Shortest) && (*p_num_sols < max_listed)) {
        // std::cout << "New(" << length << "): ";
        for (int j = Stones - 3; j >= 0; j -= 1) {
            int inv = Stones - 3 - j;
            solutions[*p_num_sols][inv][0] = future_moves[j][0];
            solutions[*p_num_sols][inv][1] = future_moves[j][1];
            solutions[*p_num_sols][inv][2] = future_moves[j][2];
            // std::cout << future_moves[j][0] << future_moves[j][1] << future_moves[j][2] << "  ";
        }
        *p_num_sols += 1;
        // std::cout << "\n";
    }
    else if (length == Shortest && max_chain > Long_chain) {
        std::cout << "Long Chain(" << length << ")[" << max_chain << "]: ";
        for (int j = Stones - 3; j >= 0; j -= 1) {
            int inv = Stones - 3 - j;
            solutions[*p_num_sols - 1][inv][0] = future_moves[j][0];
            solutions[*p_num_sols - 1][inv][1] = future_moves[j][1];
            solutions[*p_num_sols - 1][inv][2] = future_moves[j][2];
            std::cout << future_moves[j][0] << future_moves[j][1] << future_moves[j][2] << "  ";
        }
        std::cout << "\n";
        Long_chain = max_chain;
    }
}

template <size_t N>
void play_game(bool board[Stones], std::array<int[3], Stones - 2>& future_moves, int on_move, std::array<std::array<int[3], Stones - 2>, Max_listed>& solutions, int* p_num_sols, std::array<int[3][3], N> moves, int max_listed) {
    /*
    for (int i = 0; i < Stones; i += 1) {
        // std::cout << board[i];
    }
    // std::cout << "\n"; */
    for (int i = 1; i < Stones - 1; i += 1) {
        if (board[i]) {
            for (int m = 0; m < 3; m += 1) {
                if (board[moves[i][m][0]] && !board[moves[i][m][2]]) {
                    bool new_board[Stones];
                    perform_move(board, new_board, future_moves, on_move, moves[i][m][0], moves[i][m][1], moves[i][m][2]);

                    play_game(new_board, future_moves, on_move + 1, solutions, p_num_sols, moves, max_listed);
                }
                else if (board[moves[i][m][2]] && !board[moves[i][m][0]]) {
                    bool new_board[Stones];
                    perform_move(board, new_board, future_moves, on_move, moves[i][m][2], moves[i][m][1], moves[i][m][0]);

                    play_game(new_board, future_moves, on_move + 1, solutions, p_num_sols, moves, max_listed);
                }
            }
        }
    }

    if (bsum(board, Stones) == 1) {
        add_solution(future_moves, solutions, p_num_sols, max_listed);

    }
}

template <size_t N>
void chained_end(bool board[Stones], std::array<int[3], Stones - 2>& future_moves, int on_move, std::array<std::array<int[3], Stones - 2>, Max_listed>& solutions, int* p_num_sols, std::array<int[3][3], N> moves, int max_listed, int chains, int chained_len) {
    /*
    if (bsum(board, Stones) == Stones - 4) {
        std::cout << future_moves[2][0] << future_moves[2][1] << future_moves[2][2] << "\t";
        std::cout << future_moves[1][0] << future_moves[1][1] << future_moves[1][2] << "\t";
        std::cout << future_moves[0][0] << future_moves[0][1] << future_moves[0][2] << "\n";
    }*/
    
    // can iterate through i = 1; i < Stones - 1. First and last stone can't be the middle of a move.
    for (int i = 1; i < Stones - 1; i += 1) {
        if (board[i]) {
            bool new_board[Stones];
            for (int m = 0; m < 3; m += 1) {
                bool mv = false;
                int chain = 0;
                if (board[moves[i][m][0]] && !board[moves[i][m][2]]) {
                    if (on_move > 0 && future_moves[on_move - 1][0] == moves[i][m][2]) {
                        chain = chains;
                    }
                    else if (chains == 3) {
                        return;
                    }
                    else {
                        chain = chains + 1;
                    }
                    mv = true;
                    perform_move(board, new_board, future_moves, on_move, moves[i][m][0], moves[i][m][1], moves[i][m][2]);
                }
                else if (board[moves[i][m][2]] && !board[moves[i][m][0]]) {
                    if (on_move > 0 && future_moves[on_move - 1][0] == moves[i][m][0]) {
                        chain = chains;
                    }
                    else if (chains == 3) {
                        return;
                    }
                    else {
                        chain = chains + 1;
                    }
                    mv = true;
                    perform_move(board, new_board, future_moves, on_move, moves[i][m][2], moves[i][m][1], moves[i][m][0]);
                }
                if (mv && on_move > chained_len) {
                    play_game(new_board, future_moves, on_move + 1, solutions, p_num_sols, moves, max_listed);
                }
                else if (mv) {
                    chained_end(new_board, future_moves, on_move + 1, solutions, p_num_sols, moves, max_listed, chain, chained_len);
                }
            }
        }
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
std::array<int[3][3], N> valid_moves(int side, std::array<int[3][3], N>& moves) {
    int st_row = 0;
    for (int i = 0; i < side; i += 1) {
        st_row += i;
        for (int j = 0; j < i + 1; j += 1) {
            int mid = st_row + j;
            // "down"
            if (i > 0 && i < side - 1 && j < i) {
                moves[mid][0][1] = mid;
                moves[mid][0][0] = mid - i;
                moves[mid][0][2] = mid + i + 1;
            }
            else {
                moves[mid][0][1] = mid;
                moves[mid][0][0] = mid;
                moves[mid][0][2] = mid;
            }
            // "angled"
            if (i > 0 && i < side - 1 && j > 0) {
                moves[mid][1][1] = mid;
                moves[mid][1][0] = mid - i - 1;
                moves[mid][1][2] = mid + i + 2;
            }
            else {
                moves[mid][1][1] = mid;
                moves[mid][1][0] = mid;
                moves[mid][1][2] = mid;
            }
            // "horizontal"
            if (j > 0 && j < i ) {
                moves[mid][2][1] = mid;
                moves[mid][2][0] = mid - 1;
                moves[mid][2][2] = mid + 1;
            }
            else {
                moves[mid][2][1] = mid;
                moves[mid][2][0] = mid;
                moves[mid][2][2] = mid;
            }
        }
    }    
    return moves;
}

template <size_t N>
void print_foreward(int side, int* starts, int len_starts, std::array<int[3][3], N> moves) {
    std::cout << "Analyzing Board:\n";
    int spacing = (int)log10(Stones - 1) + 1;
    int rst = 0;
    for (int r = 0; r < side; r += 1) {
        rst += r;
        for (int c = 0; c < r + 1; c += 1) {
            if (rst > 0) {
                std::cout << (rst + c);
                for (int sp = 0; sp < spacing - log10(rst + c); sp += 1) {
                    std::cout << " ";
                }
            }
            else {
                std::cout << "0";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    std::cout << "Unique Starting Positions:\n";
    for (int i = 0; i < len_starts; i += 1) {
        std::cout << starts[i] << "  ";
    }
    std::cout << "\n";

    std::cout << "Potentially Valid Moves:\n";
    for (int i = 0; i < Stones; i += 1) {
        for (int j = 0; j < 3; j += 1) {
            if (!(moves[i][j][0] == moves[i][j][2])) {
                std::cout << moves[i][j][0] << "," << moves[i][j][1] << "," << moves[i][j][2] << "   ";
            }
        }
    }
    std::cout << "\n" << "\n";
}

void print_result(std::array<std::array<int[3], Stones - 2>, Max_listed>& solutions, int num_sols, int max_listed) {
    std::cout << "Shortest solution length: " << Shortest << "\n";
    for (int i = 0; i < num_sols; i += 1) {
        std::cout << "[" << solutions[i][0][0] << "-" << solutions[i][0][2];
        int chain_end = solutions[i][0][2];
        for (int j = 1; j < Stones - 2; j += 1) {
            if (solutions[i][j][0] == chain_end) {
                chain_end = solutions[i][j][2];
                std::cout << "-" << chain_end;
            }
            else {
                chain_end = solutions[i][j][2];
                std::cout << ", " << solutions[i][j][0] << "-" << chain_end;
            }
        }
        std::cout << "]\n";
    }
}