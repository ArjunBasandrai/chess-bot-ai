#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uci.h"

#include "board_constants.h"
#include "board.h"
#include "moves_list.h"
#include "movegen.h"
#include "search.h"
#include "fen.h"
#include "transposition_table.h"
#include "gettime.h"
#include "polyglot/polykeys.h"

void reset_time_control() {
    quit = 0;
    movestogo = 30;
    movetime = -1;
    m_time = -1;
    inc = 0;
    starttime = 0;
    stoptime = 0;
    timeset = 0;
    stopped = 0;
}

int parse_move(char *move_string) {
    moves move_list[1];
    generate_moves(move_list);

    int source_square = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;
    int target_square = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;

    for (int move_count = 0; move_count < move_list->count; move_count++) {
        int move = move_list->moves[move_count];

        if (source_square == get_move_source(move) && target_square == get_move_target(move)) {
            int promoted_piece = get_move_promoted(move);

            if (promoted_piece) {
                if ((promoted_piece == Q || promoted_piece == q) && move_string[4] == 'q') {
                    return move;
                }

                else if ((promoted_piece == R || promoted_piece == r) && move_string[4] == 'r') {
                    return move;
                }

                else if ((promoted_piece == B || promoted_piece == b) && move_string[4] == 'b') {
                    return move;
                }

                else if ((promoted_piece == N || promoted_piece == n) && move_string[4] == 'n') {
                    return move;
                }

                continue;
            }
            
            return move;  
        }
    }

    return 0;

    // printf("source square: %s\n",sqaure_to_coordinate[target_square]);
}


void parse_position(char *command) {
    command += 9;
    char *current_char = command;

    // parse UCI startpos command
    if (strncmp(command, "startpos", 8) == 0) {
        parse_fen(start_position);
    } 
    
    // parse UCI fen command
    else {
        current_char = strstr(command, "fen");

        if (current_char == NULL) {
            parse_fen(start_position);
        }

        else {
            current_char += 4;

            parse_fen(current_char);
        }
    }

    current_char = strstr(command,"moves");

    if (current_char != NULL) {

        current_char += 6;
        int make_line = 1;
        while(*current_char) {
            int move = parse_move(current_char);

            if (!move) {
                break;
            }

            repetition_index++;
            repetitions_table[repetition_index] = hash_key;
            make_move(move, all_moves);

            while (*current_char && *current_char != ' ') current_char++;
            current_char++;
        }
    }
}

void parse_go(char *command)
{
    reset_time_control();

    // init parameters
    int depth = -1;

    // init argument
    char *argument = NULL;

    // infinite search
    if ((argument = strstr(command,"infinite"))) {}

    if ((argument = strstr(command,"binc")) && side == black) {
        // parse black time increment
        inc = atoi(argument + 5);
    }

    if ((argument = strstr(command,"winc")) && side == white) {
        // parse white time increment
        inc = atoi(argument + 5);
    }

    if ((argument = strstr(command,"wtime")) && side == white) {
        // parse white time limit
        m_time = atoi(argument + 6);
    }

    if ((argument = strstr(command,"btime")) && side == black) {
        // parse black time limit
        m_time = atoi(argument + 6);
    }

    if ((argument = strstr(command,"movestogo"))) {
        // parse number of moves to go
        movestogo = atoi(argument + 10);
    }

    if ((argument = strstr(command,"movetime"))) {
        // parse amount of time allowed to spend to make a move
        movetime = atoi(argument + 9);
    }

    if ((argument = strstr(command,"depth"))) {
        depth = atoi(argument + 6);
    }

    if (movetime != -1) {
        m_time = movetime;

        movestogo = 1;
    }

    starttime = get_time_ms();

    depth = depth;

    // if time control is available
    if(m_time != -1)
    {
        timeset = 1;

        m_time /= movestogo;
        if (m_time > 1500) m_time -= 50;
        stoptime = starttime + m_time + inc;
        if (m_time < 1500 && inc && depth == 64) stoptime = starttime + inc - 50;
    }

    if(depth == -1)
        depth = 64;

        search_position(depth);
}

void uci_loop() {
    int max_hash = 1024;
    int mb = 64;
    
    setbuf(stdout, NULL);

    char input[2000];

    // main loop
    while (1) {
        memset(input, 0, sizeof(input));
        fflush(stdout);

        if (!fgets(input, 2000, stdin)) { continue; }

        if (input[0] == '\n') { continue; }

        if (strncmp(input, "isready", 7) == 0) { 
            printf("readyok\n"); 
            continue;
        }

        else if (strncmp(input, "position", 8) == 0) { 
            parse_position(input);
            clear_transposition_table();
        }

        else if (strncmp(input, "ucinewgame", 10) == 0) { 
            parse_position("position startpos"); 
            clear_transposition_table();
        }

        else if (strncmp(input, "go", 2) == 0) { 
            parse_go(input); 
        }

        else if (strncmp(input, "quit", 4) == 0) { 
            break;
        }

        else if (strncmp(input, "polykey", 7) == 0) { 
            get_book_move();
        }

        else if (strncmp(input, "uci", 3) == 0) { 
            printf("id name %s %s\n",engine_name, version);
            printf("id author Arjun Basandrai\n");
            printf("option name Hash type spin default 64 min 4 max %d\n",max_hash);
            printf("option name OwnBook type check default true\n");
            printf("uciok\n");
        }

        else if (!strncmp(input, "setoption name Hash value ", 26)) {
            sscanf(input,"%*s %*s %*s %*s %d",&mb);

            if (mb < 4) mb = 4;
            if (mb > max_hash) mb = max_hash;

            init_transposition_table(mb);
        }

        else if (!strncmp(input, "setoption name Book value ", 26)) {
            char *ptr = NULL;
            ptr = strstr(input, "true");
            if (ptr != NULL) {
                engine_options->use_book = 1;
            } else {
                engine_options->use_book = 0;
            }
        }
    }
}