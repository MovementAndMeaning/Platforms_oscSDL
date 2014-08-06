//--------------------------------------------------------------------------------------------------
//
//  File:       oscReader.cpp
//
//  Project:    oscSDL
//
//  Contains:   simple thread class for osc Reader
//              largely based of ofxOscReader
//      https://github.com/openframeworks/openFrameworks/blob/master/addons/ofxOsc/
//
//  Written by: Johnty Wang
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and/or
//                  other materials provided with the distribution.
//                * Neither the name of the copyright holders nor the names of its contributors may
//                  be used to endorse or promote products derived from this software without
//                  specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//              EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//              OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//              SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//              INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//              TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//              BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//              ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//              DAMAGE.
//
//  Created:    2014-08-03
//
//--------------------------------------------------------------------------------------------------

#include "oscReader.h"

void oscReader::setup(int port) {
    if (mySocket)
        shutdown();
    
    
    socketHasShutdown = false;
    mySocket = new UdpListeningReceiveSocket(IpEndpointName(IpEndpointName::ANY_ADDRESS, port), this);
    
    pthread_create(&thread, NULL, &oscReader::startThread, (void*)this);
    
}

oscReader::~oscReader() {
    shutdown();
}

void oscReader::ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& ep)
{
    printf("message received!\n");
}


int oscReader::startThread(void *readerInstance) {
    printf("oscReader::starting osc reader thread!\n");
    oscReader* instance = (oscReader*) readerInstance;
    
    instance->mySocket->Run();
    
    
    printf("oscReader::shutting down\n");
    
    return 0;
}

void oscReader::shutdown()
{
    if (mySocket) {
        mySocket->AsynchronousBreak();
    }
    
    delete  mySocket;
    mySocket = NULL;
}
