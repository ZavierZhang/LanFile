/*
 *  LANFileTransfer.cpp
 *  LANFileTransfer
 *
 *  Created by 张威 on 18/10/15.
 *  Copyright © 2018年 张威. All rights reserved.
 *
 */

#include <iostream>
#include "LANFileTransfer.hpp"
#include "LANFileTransferPriv.hpp"

void LANFileTransfer::HelloWorld(const char * s)
{
    LANFileTransferPriv *theObj = new LANFileTransferPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void LANFileTransferPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

