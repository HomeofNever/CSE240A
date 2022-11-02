//========================================================//
//  predictor.h                                           //
//  Header file for the Branch Predictor                  //
//                                                        //
//  Includes function prototypes and global predictor     //
//  variables and defines                                 //
//========================================================//

#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <stdint.h>
#include <stdlib.h>

//
// Student Information
//
extern const char *studentName;
extern const char *studentID;
extern const char *email;

//------------------------------------//
//      Global Predictor Defines      //
//------------------------------------//
#define NOTTAKEN 0
#define TAKEN 1

// The Different Predictor Types
#define STATIC 0
#define GSHARE 1
#define TOURNAMENT 2
#define CUSTOM 3
extern const char *bpName[];

// Definitions for 2-bit counters
#define SN 0 // Strong not taken, predict NT, strong not taken
#define WN 1 // Weak not taken, predict NT, weak not taken
#define WT 2 // Weak taken, predict T, weak taken
#define ST 3 // Strong taken, predict T, strong taken

typedef uint32_t IDX;
typedef uint8_t PNT;

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//
extern int ghistoryBits; // Number of bits used for Global History
extern int lhistoryBits; // Number of bits used for Local History
extern int pcIndexBits;  // Number of bits used for PC index
extern int bpType;       // Branch Prediction Type
extern int verbose;

//------------------------------------//
//    Predictor Function Prototypes   //
//------------------------------------//

// Initialize the predictor
//
void init_predictor();
void gshare_init();
void tournament_init();
void custom_init();

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t make_prediction(uint32_t pc);
PNT gshare_make_prediction(IDX pc);
PNT tournament_make_prediction(IDX pc);
PNT tournament_global_prediction(IDX pc);
PNT tournament_local_prediction(IDX pc);
PNT custom_make_prediction(IDX pc);

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void train_predictor(uint32_t pc, uint8_t outcome);
void update_gshare(IDX pc, PNT outcome);
void update_tournament(IDX pc, PNT outcome);
void update_custom(IDX pc, PNT outcome);

// Utils
PNT two_bit_predictor(PNT history);
PNT update_two_bit_predictor(PNT outcome, PNT bits);
void update_global_history_register(PNT outcome);

#endif
