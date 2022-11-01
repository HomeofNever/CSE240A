//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
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
int bht_mask = 3; // 11
// Gshare
int global_history_register = 0;
int global_history_mask;
int global_history_num;
int *g_history;
// Gshare shared var
int gshare_current_index;

//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//
void init_predictor()
{
    // Gshare
    global_history_num = 1 << ghistoryBits;
    global_history_mask = global_history_num - 1;
    // FIXME: memory clean up
    g_history = calloc(global_history_num, sizeof(int));
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
    case CUSTOM:
    default:
        break;
    }

    // If there is not a compatable bpType then return NOTTAKEN
    return NOTTAKEN;
}

uint8_t make_prediction_gshare(uint8_t pc)
{
    gshare_current_index = global_history_register ^ (pc & global_history_mask);
    if (g_history[gshare_current_index] > 1) // 00, 01, 10, 11
        return TAKEN;

    return NOTTAKEN;
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
    case CUSTOM:
    default:
        break;
    }
}

// gshare
void update_gshare(uint32_t pc, uint8_t outcome)
{
    // update Gshare
    // make use of index in calculation
    int bits = g_history[gshare_current_index];
    bits = bits << 1;
    bits = bits | outcome;
    bits = bits & bht_mask;
    g_history[gshare_current_index] = bits;
    update_global_history_register(outcome);
}

void update_global_history_register(int outcome)
{
    global_history_register = global_history_register << 1;
    global_history_register = global_history_register | outcome;
    global_history_register = global_history_register & global_history_mask;
}
