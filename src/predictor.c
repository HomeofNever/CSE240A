//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include <string.h>
#include "predictor.h"

//
// TODO:Student Information
//
const char *studentName = "NAME";
const char *studentID = "PID";
const char *email = "EMAIL";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = {"Static", "Gshare",
                         "Tournament", "Custom"};

// Configuration
// GShare
int ghistoryBits; // Number of bits used for Global History
// Tournament
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
// Others
int bpType; // Branch Prediction Type
int verbose;

//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//
// BHT
PNT bht_mask = 3; // 11
/// --- Gshare
// register will be reuse for tournament
IDX global_history_register = 0;
IDX global_history_mask;
IDX global_history_num;
PNT *gshare_history; // Branch History Table (BHT)
// Gshare shared var
IDX gshare_current_index;
/// --- Tournament
// Choice mask will use global history mask
PNT *g_history;  // Global history
PNT *lb_history; // BHT for local
IDX *lp_pattern; // Pattern History Table for local, based on pc index
PNT *choice_pattern;
IDX choice_register = 0;
IDX local_branch_history_mask;
IDX local_pattern_history_mask;
// Tournament Shared Var
IDX global_bht_index;
IDX current_local_index;
IDX current_local_pht_index;
PNT current_local_prediction;
PNT current_global_prediction;

//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//
void init_predictor()
{
    global_history_num = 1 << ghistoryBits;
    global_history_mask = global_history_num - 1;
    if (verbose)
        printf("[init]global_history_num (%d)\n", global_history_num);
    switch (bpType)
    {
    case GSHARE:
        gshare_init();
        break;
    case TOURNAMENT:
        tournament_init();
        break;
    case CUSTOM:
        custom_init();
        break;
    default:
        break;
    }
}

void gshare_init()
{
    // FIXME: memory clean up
    gshare_history = malloc(global_history_num * sizeof(PNT));
    memset(gshare_history, WN, global_history_num);
}

// This function assumes gshare_init
void tournament_init()
{
    // Local
    int lb_nums = 1 << lhistoryBits;
    int ph_nums = 1 << pcIndexBits;
    local_branch_history_mask = lb_nums - 1;
    local_pattern_history_mask = ph_nums - 1;
    lb_history = malloc(lb_nums * sizeof(PNT));
    memset(lb_history, WN, lb_nums);
    lp_pattern = malloc(ph_nums * sizeof(IDX));
    memset(lp_pattern, 0, ph_nums);
    g_history = malloc(global_history_num * sizeof(PNT));
    memset(g_history, WN, global_history_num);
    // Choice, same as global
    choice_pattern = malloc(global_history_num * sizeof(PNT));
    memset(choice_pattern, WN, global_history_num);
}

void custom_init()
{
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t
make_prediction(uint32_t pc)
{
    // Make a prediction based on the bpType
    switch (bpType)
    {
    case STATIC:
        return TAKEN;
    case GSHARE:
        return gshare_make_prediction(pc);
    case TOURNAMENT:
        return tournament_make_prediction(pc);
    case CUSTOM:
        return custom_make_prediction(pc);
    default:
        break;
    }

    // If there is not a compatable bpType then return NOTTAKEN
    return NOTTAKEN;
}

PNT gshare_make_prediction(IDX pc)
{
    if (verbose)
    {
        printf("[gshare make prediction] pc(%d)\n", pc);
        printf("[gshare make prediction] global_history_register(%d)\n", global_history_register);
    }
    gshare_current_index = global_history_register ^ (pc & global_history_mask);
    if (verbose)
        printf("[gshare make prediction] gshare_current_index (%d)\n", gshare_current_index);
    return two_bit_predictor(gshare_history[gshare_current_index]);
}

/// --- tournament

PNT tournament_make_prediction(IDX pc)
{
    global_bht_index = global_history_register ^ global_history_mask;
    PNT choice_outcome = choice_pattern[global_bht_index];
    current_global_prediction = tournament_global_prediction(pc);
    current_local_prediction = tournament_local_prediction(pc);

    // Taken: local; not taken: global
    if (choice_outcome > WN)
        return current_local_prediction;
    else
        return current_global_prediction;
}

PNT tournament_global_prediction(IDX pc)
{
    return two_bit_predictor(g_history[global_bht_index]);
}

PNT tournament_local_prediction(IDX pc)
{
    current_local_pht_index = pc & local_pattern_history_mask;
    current_local_index = lp_pattern[current_local_pht_index];
    PNT outcome = lb_history[current_local_index];
    return two_bit_predictor(outcome);
}

/// --- custom

PNT custom_make_prediction(IDX pc)
{
    // @TODO
    return 0;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void train_predictor(uint32_t pc, uint8_t outcome)
{
    switch (bpType)
    {
    case GSHARE:
        update_gshare(pc, outcome);
        break;
    case TOURNAMENT:
        update_tournament(pc, outcome);
        break;
    case CUSTOM:
        update_custom(pc, outcome);
        break;
    default:
        break;
    }

    // Always update history register
    update_global_history_register(outcome);
}

// gshare
void update_gshare(IDX pc, PNT outcome)
{
    // update Gshare
    // make use of index in calculation
    if (verbose)
    {
        printf("[update_gshare] gshare_current_index (%d)\n", gshare_current_index);
        printf("[update_gshare] pc(%d)\n", pc);
        printf("[update_gshare] outcome(%d)\n", outcome);
    }
    gshare_history[gshare_current_index] = update_two_bit_predictor(outcome, gshare_history[gshare_current_index]);
}

// tournament
void update_tournament(IDX pc, PNT outcome)
{
    // update choice pattern
    if (current_global_prediction != current_local_prediction)
    {
        PNT o = current_local_prediction == outcome ? TAKEN : NOTTAKEN;
        choice_pattern[global_bht_index] = update_two_bit_predictor(o, choice_pattern[global_bht_index]);
    }

    // train local predictor
    lb_history[current_local_index] = update_two_bit_predictor(outcome, lb_history[current_local_index]);

    // update PHT
    lp_pattern[current_local_pht_index] <<= 1;
    lp_pattern[current_local_pht_index] &= local_branch_history_mask;
    lp_pattern[current_local_pht_index] |= outcome;

    // train global predictor
    g_history[global_bht_index] = update_two_bit_predictor(outcome, g_history[global_bht_index]);
}

void update_custom(IDX pc, PNT outcome)
{
}

// --- utils

// act as predictor
// 00 0 predict NT, strong not taken
// 01 1 predict NT, weak not taken
// 10 2 predict T, weak taken
// 11 3 predict T, strong taken
PNT two_bit_predictor(PNT history)
{
    if (history > 1) // 00, 01, 10, 11
        return TAKEN;

    return NOTTAKEN;
}

PNT update_two_bit_predictor(PNT outcome, PNT bits)
{
    if (bits > SN && outcome == NOTTAKEN)
        bits--;
    else if (bits < ST && outcome == TAKEN)
        bits++;

    return bits;
}

// this function update global register on train
void update_global_history_register(PNT outcome)
{
    global_history_register <<= 1;
    global_history_register &= global_history_mask;
    global_history_register |= outcome;
}
