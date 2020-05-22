//
// Created by fenix on 4/18/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_CODE_CONTROL_H
#define COUNT_NUMBER_OF_ALL_WORDS_CODE_CONTROL_H

//#define DEBUG_INFO

/* ////////  Work for parallel program /////////
 * txt - for loaded files                     //
 * '*' - file to be unarchived                //
 * 'e' - archive entry to be processed        //
 * 'b' - publish a archive entry (recursive a)//
 * '.' - one data entry is indexed            //
 * '+' - two maps to be merged                //
 * 'U' - one unachive thread is finished      //
 * 'C' - one indexing thread is finished      //
 * 'M' - one merge thread is finished         //
 */////////////////////////////////////////////

//#define VISUAL_PIPELINE

#define QUEUE_CAPACITY 16
#define MAX_TEXT_FILE_SIZE 100000000

#endif //COUNT_NUMBER_OF_ALL_WORDS_CODE_CONTROL_H
