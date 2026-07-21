// Modify this file
// You may add more files if you want
// USE THE PROVIDED FUNCTIONS TO RECEIVE AND SEND MOVES (OTHERWISE YOU MAY TIME OUT)
#include "chess.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//struct for chess moves and board tracking
typedef struct 
{
    int sr; //start row
    int sc; //start column
    int er; //end row
    int ec; //end column
    char piece; //moving piece
    char capture; //pieces captured
} Move;

#define BOARD_SIZE 8

char board[BOARD_SIZE][BOARD_SIZE];

//bounds for chess board
//int r = row, int c = column
static bool in_bounds(int r,int c)
{ 
    return ((r >= 0 && r < 8) && (c >= 0 && c < 8)); 
}

//default test values (don't change)
const move_t moves[] = {"e2e4x", "e7e5x", "f1c4x", "b8c6x", "d1h5x", "g8f6x", "h5f7x", "a7a5x", "a2a4x", "b7b5x"};

//default function (don't change)
void white()
{
    send_move(moves[0]);
    int idx = 2;
    int time, opp_time;
    move_t opp_move;
    do
    {
        if (get_move(opp_move, &time, &opp_time) < 0)
            return;
        send_move(moves[idx]);
        idx += 2;
    } while (true);
}

//default function (don't change)
void black()
{
    int idx = 1;
    int time, opp_time;
    move_t opp_move;
    do
    {
        if (get_move(opp_move, &time, &opp_time) < 0)
            return;
        send_move(moves[idx]);
        idx += 2;
    } while (true);
}

//initalize board
void init_board() 
{
    const char *init[8] = 
    {
        //rook, knight, bishop, queen, king, bishop, knight, rook
        //8 pawns
        "rnbqkbnr", //0
        "pppppppp", //1
        "........", //2
        "........", //3
        "........", //4
        "........", //5
        "PPPPPPPP", //6
        "RNBQKBNR"  //7
    };

    //8 by 8 board placement nested loop
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            board[i][j] = init[i][j]; //place pieces on board
        }
    }
}


//check if pieces are blocking path of another piece
//int sr, er = start, end row, int sc, ec = start, end column
bool check_piece_in_way(int sr, int sc, int er, int ec)
{
    int dr = (er > sr) - (er < sr); //vertical direction
    int dc = (ec > sc) - (ec < sc); //horizontal direction

    for(sr += dr,sc += dc; sr != er || sc != ec; sr += dr,sc += dc)
    {
        //if no empty space --> piece in way
        if(board[sr][sc] != '.')
        {
            return (false);
        }
    }

    return (true);
}

//make move on board
//Move *m is add of Move
void make_move(Move *m) {
    board[m-> er][m-> ec] = m-> piece;
    board[m-> sr][m-> sc] = '.';
}

//undo move (if illegal --. go back)
void undo_move(Move *m) {
    board[m-> sr][m-> sc] = m-> piece;
    board[m-> er][m-> ec] = m-> capture;
}

//check if king in check
// int isWhite --> 1 (white), 0 (black)
bool IsKingInCheck(int isWhite)
{
    int kr = -1;
    int kc = -1;

    char king = isWhite ? 'K':'k';

    //find location of king on 8x8 board
    for(int r = 0; r < 8; r++)
    {
        for(int c = 0; c < 8; c++)
        {
            if(board[r][c] == king)
            {
                kr = r; 
                kc = c; 
                r = 8;
                break;
            }
        }
    }

    //king not found
    if(kr == -1) 
    {
        return (false);
    }

    //
    for(int r = 0; r < 8; r++)
    {
        for(int c = 0; c < 8; c++)
        {
            char p = board[r][c];

            //skip empty
            if(p == '.') 
            {
                continue;
            }

            //skip white (friendly)
            if(isWhite && (p >= 'a')) 
            {
                continue;
            }

            //skip black (friendly)
            if(!isWhite && (p <= 'Z')) 
            {
                continue;
            }

            //position relative to other pieces
            int dr = kr-r;
            int dc = kc-c;
            int adx = (dr > 0 ? dr:-dr);
            int ady = (dc > 0 ? dc:-dc);

            if(p =='r'||p =='R'||p =='q'||p =='Q')
            {
                if(dr == 0 || dc == 0) //same r,c
                {
                    if(check_piece_in_way(r, c, kr, kc)) //no piece in way
                    { 
                        return (true); //in check
                    }
                }
            }

            if(p == 'b'||p == 'B'||p == 'Q'||p == 'q')
            { 
                if(adx == ady) //same diagnol
                {
                    if(check_piece_in_way(r, c, kr, kc))
                    {
                        return (true); //in check
                    }
                } 
            }

            if(p == 'n'||p == 'N') //knight
            { 
                if(((adx == 2) && (ady == 1))||((adx == 1) && (ady == 2)))
                {
                    return (true); //in check
                }
            }

            if(p == 'p') //pawn
            { 
                if(dr == 1 && adx == 1)
                {
                    return (true); //in check
                }
            }

            if(p == 'P')
            { 
                if(dr == -1 && adx == 1) 
                {
                    return (true); 
                }
            }

            if(p == 'k'||p == 'K') //king
            { 
                if(adx <= 1 && ady <= 1) 
                {
                    return (true); //in check
                }
            }
        }
    }

    return (false); //not in check
}

//moves in designated direction for rook, bishop, queen
static void generate_directional_moves(int r, int c, int dr, int dc, int isWhite, Move moves_list[], int *count)
{
    //start one square from piece
    int er = r + dr;
    int ec = c + dc;

    //track edge of board
    while(in_bounds(er, ec))
    {
        char target=board[er][ec];
        if (target != '.')
        {
            //if blocking piece is same color
            if ((isWhite && target >= 'A' && target <= 'Z') || (!isWhite && target >= 'a' && target <= 'z'))
            {
                break;
            }
        }

        //updates moves + captures
        moves_list[*count].sr = r; 
        moves_list[*count].sc = c;
        moves_list[*count].er = er;
        moves_list[*count].ec = ec;
        moves_list[*count].piece = board[r][c]; 
        moves_list[*count].capture = target;

        (*count)++;

        //hit piece --> stop
        if(target != '.') 
        {
            break;
        }

        //move one more
        er += dr; 
        ec += dc;
    }
}

//moves for rook
static void generate_rook_moves(int r, int c, int isWhite, Move moves_list[], int *count)
{
    generate_directional_moves(r, c, 1, 0, isWhite,moves_list, count); //x
    generate_directional_moves(r, c, -1, 0, isWhite, moves_list, count); //-x
    generate_directional_moves(r, c, 0, 1, isWhite, moves_list, count); //y
    generate_directional_moves(r, c, 0, -1, isWhite, moves_list, count); //-y
}

//moves for bishop
static void generate_bishop_moves(int r,int c,int isWhite,Move moves_list[],int *count)
{
    generate_directional_moves(r, c, 1, 1, isWhite, moves_list, count); //r top
    generate_directional_moves(r, c, 1, -1, isWhite, moves_list, count); //r bottom
    generate_directional_moves(r, c, -1, 1, isWhite, moves_list, count); //l top
    generate_directional_moves(r, c, -1, -1, isWhite, moves_list, count); //l bottom
}

//moves for queen
static void generate_queen_moves(int r, int c, int isWhite, Move moves_list[], int *count)
{
    generate_rook_moves(r, c, isWhite, moves_list, count); //up, down
    generate_bishop_moves(r, c, isWhite, moves_list, count); //diagnol
}

//moves for knight
static void generate_knight_moves(int r, int c, int isWhite, Move moves_list[], int *count)
{
    int drs[8]={-2, -1, 1, 2, 2, 1, -1, -2}; //x of L jump (index)
    int dcs[8]={1, 2, 2, 1, -1, -2, -2, -1}; //y of L jump (index)

    for(int i = 0; i < 8; i++) //L combos
    {
        int er = r + drs[i];
        int ec = c + dcs[i];

        if(!in_bounds(er, ec)) //no move if out bounds
        {
            continue;
        }

        char target = board[er][ec];

        //same color --> friendly piece (skip)
        if ((isWhite && target >= 'A' && target <= 'Z') || (!isWhite && target >= 'a' && target <= 'z'))
        {
            continue;
        }

        //updates moves + captures
        moves_list[*count].sr = r;
        moves_list[*count].sc = c;
        moves_list[*count].er = er;
        moves_list[*count].ec = ec;
        moves_list[*count].piece = board[r][c];
        moves_list[*count].capture = target;

        (*count)++;
    }
}

//moves for king
static void generate_king_moves(int r, int c, int isWhite, Move moves_list[], int *count)
{
    //3x3 for king
    for(int dr = -1; dr <= 1; dr++)
    {
        for(int dc =- 1; dc <= 1; dc++)
        {
            if(dr == 0 && dc == 0) //current --> skip
            {
                continue;
            }

            //moving calc
            int er = r + dr;
            int ec = c + dc;

            //check in bounds
            if(!in_bounds(er, ec))
            {
                continue;
            }

            char target = board[er][ec];

            //same color --> friendly piece (skip)
            if((isWhite && (target >= 'A')) || (!isWhite && (target <= 'Z') && (target != '.'))) 
            {
                continue;
            }

            //updates moves + captures
            moves_list[*count].sr = r;
            moves_list[*count].sc = c;
            moves_list[*count].er = er;
            moves_list[*count].ec = ec;
            moves_list[*count].piece = board[r][c];
            moves_list[*count].capture = target;

            (*count)++;
        }
    }
}

//moves for pawn
static void generate_pawn_moves(int r, int c, int isWhite, Move moves_list[], int *count)
{
    int dr = isWhite ? -1:1; //decide move up (white), move down (black)
    int startRow = isWhite ? 6:1;
    int er = r + dr;
    
    //move forward if empty
    if(in_bounds(er, c) && board[er][c] == '.')
    {
        moves_list[*count].sr = r;
        moves_list[*count].sc = c;
        moves_list[*count].er = er;
        moves_list[*count].ec = c;
        moves_list[*count].piece = board[r][c];
        moves_list[*count].capture = '.';

        (*count)++;

        //special move --> move 2 squares for pawn in start (if no piece there)
        if(r == startRow && in_bounds(r + 2 * dr, c) && board[r + 2 * dr][c] == '.')
        {
            moves_list[*count].sr = r;
            moves_list[*count].sc = c;
            moves_list[*count].er = r + 2 * dr;
            moves_list[*count].ec = c;
            moves_list[*count].piece = board[r][c];
            moves_list[*count].capture = '.';

            (*count)++;
        }
    }

    //special move --> diagnol capture
    for(int dc = -1; dc <= 1; dc += 2)
    {
        int ec = c + dc;

        //in bound
        if(!in_bounds(er, ec)) 
        {
            continue;
        }

        char target = board[er][ec];

        //if enemy piece --> capture
        if ((isWhite && target >= 'a' && target <= 'z') || (!isWhite && target >= 'A' && target <= 'Z'))
        {
            //updates moves + captures
            moves_list[*count].sr = r;
            moves_list[*count].sc = c;
            moves_list[*count].er = er;
            moves_list[*count].ec = ec;
            moves_list[*count].piece = board[r][c];
            moves_list[*count].capture = target;

            (*count)++;
        }
    }
}

//make legal move (don't leave king in check)
void generate_legal_moves(int isWhite, Move moves_list[], int *count)
{
    //reset
    *count = 0;
    Move temp[256];

    //iterate over whole board
    for(int r = 0; r < 8; r++)
    {
        for(int c = 0; c < 8; c++)
        {
            char p = board[r][c];

            if(p == '.') //empty
            {
                continue;
            }

            if(isWhite && p < 'A') //own white piece
            {
                continue;
            }

            if(!isWhite && p > 'Z') //own black piece
            {
                continue;
            }
            
            //temp count
            int tc = 0;

            //go through all cases and make legal move based on piece funcs
            switch(p)
            {
                case 'P': case 'p': generate_pawn_moves(r, c, isWhite, temp, &tc); 
                                    break;
                case 'R': case 'r': generate_rook_moves(r, c, isWhite, temp, &tc); 
                                    break;
                case 'N': case 'n': generate_knight_moves(r, c, isWhite, temp, &tc); 
                                    break;
                case 'B': case 'b': generate_bishop_moves(r, c, isWhite, temp, &tc); 
                                    break;
                case 'Q': case 'q': generate_queen_moves(r, c, isWhite, temp, &tc); 
                                    break;
                case 'K': case 'k': generate_king_moves(r, c, isWhite, temp, &tc); 
                                    break;
            }

            //check if legal, and loop till make_move --> legal
            for(int i = 0; i < tc; i++)
            {
                make_move(&temp[i]);
                if(!IsKingInCheck(isWhite))
                {
                    moves_list[*count] = temp[i];
                    (*count)++;
                }

                undo_move(&temp[i]); //undo if illegal
            }
        }
    }
}

//update board with opponent move
void apply_opponent_move(move_t m)
{
    //string to board terms
    int sc = m[0] - 'a';
    int sr = 8 - (m[1] - '0');
    int ec = m[2] - 'a';
    int er = 8 - (m[3] - '0');

    //move struct --> recreate move
    Move mv;
    mv.sr = sr;
    mv.sc = sc;
    mv.er = er;
    mv.ec = ec;
    mv.piece = board[sr][sc]; //moved
    mv.capture = board[er][ec]; //captured

    //make own move
    make_move(&mv);
}

//decision making
void play_turn(int isWhite)
{
    //intialize
    Move moves_list[256];
    int count;

    generate_legal_moves(isWhite,moves_list, &count);

    //no move
    if(count==0)
    { 
        if(IsKingInCheck(isWhite))
        {}
        else 
        {}
        return; 
    }

    make_move(&moves_list[0]);

    //protocol --> turn into string
    move_t mstr;
    mstr[0] = moves_list[0].sc + 'a';
    mstr[1] = '8' - moves_list[0].sr;
    mstr[2] = moves_list[0].ec + 'a';
    mstr[3] = '8' - moves_list[0].er;
    mstr[4] = 'x'; //default

    //special move
    //pawn promotion
    if (moves_list[0].piece == 'P' && moves_list[0].er == 0)
    {
        mstr[4] = 'q'; //queen
    }
    else if (moves_list[0].piece == 'p' && moves_list[0].er == 7)
    {
        mstr[4] = 'q'; //queen
    }

    send_move(mstr);
}


//initalize function for main call
//int color = assigned color
void chess_engine(int color)
{
    init_board();
    while(1)
    {
        if(color == 1) //white
        {
            play_turn(1);
            move_t opp; //first turn --> move
            int t1, t2;
            if(get_move(opp, &t1, &t2) < 0) 
            {
                break;
            }
            apply_opponent_move(opp);
        } 
        else if(color == 2) //black
        {
            move_t opp;
            int t1, t2;
            if(get_move(opp, &t1, &t2) < 0) //second turn --> recieve
            {
                break;
            }

            apply_opponent_move(opp);
            play_turn(0);
        }
    }
}
