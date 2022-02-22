
/*
 * DataModule2.h
 *
 *  Created on: 5 Mar 2020
 *      Author: student
 */

#define MESS_Size 30

struct DataModule

   {

   int glucose;
   int pressure;
   int voltage

   char message[MESS_Size];

   };

#define DATA_MODULE_SIZE sizeof(struct DataModule)
