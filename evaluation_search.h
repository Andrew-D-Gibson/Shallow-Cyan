//
//  evaluation_search.h
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 4/8/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#ifndef __Shallow_Cyan__Evaluation__
#define __Shallow_Cyan__Evaluation__

#include "legal_move_gen.h"

cMove topNegamaxAB(cBoard board, int depth, time_t &endTime, cMove topMove);

#endif /* defined(__Shallow_Cyan__Evaluation__) */
