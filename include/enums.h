//
//  enums.h
//  TareaIO
//
//  Created by Matias Barrientos on 09-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef enums_h
#define enums_h

// Metodos de enfriamiento
typedef enum cooling_type cooling_type;

// Tipo de solucion inicial
typedef enum initial_solution initial_solution;

// Tipo de movimiento
typedef enum move_type move_type;

enum move_type {
    SWAP,
    TWO_OPT
};

enum initial_solution {
    DETERMINISTIC,
    GREEDY,
    RANDOM,
    NONE
};

enum cooling_type {
    GEOMETRIC
};


#endif /* enums_h */
