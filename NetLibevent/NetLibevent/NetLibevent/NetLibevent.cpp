/*
 *  NetLibevent.cpp
 *  NetLibevent
 *
 *  Created by 张威 on 18/10/15.
 *  Copyright © 2018年 张威. All rights reserved.
 *
 */

#include <iostream>
#include "NetLibevent.hpp"
#include "NetLibeventPriv.hpp"

void NetLibevent::HelloWorld(const char * s)
{
    NetLibeventPriv *theObj = new NetLibeventPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void NetLibeventPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

