#pragma once

#define version "1.6.0"

void reset_time_control();
int parse_move(char *move_string);
void parse_position(char *command);
void parse_go(char *command);
void uci_loop();