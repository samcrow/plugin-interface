// Copyright (c) 2014, Sam Crow samcrow@uw.edu
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those
// of the authors and should not be interpreted as representing official policies,
// either expressed or implied, of the FreeBSD Project.

#include <cstring>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <XPLMUtilities.h>
#include "plugin.h"

#ifndef PLUGIN_CLASS_NAME
#error PLUGIN_CLASS_NAME must be defined
#endif
#ifndef PLUGIN_HEADER_NAME
#error PLUGIN_HEADER_NAME must be defined
#endif



#define STR(x) #x

// Make a string literal so that the plugin class can be used in later code
#define PLUGIN_CLASS_NAME_STR STR(PLUGIN_CLASS_NAME)
// Surround the header file name with angle brackets to make it valid after and #include
#define PLUGIN_HEADER_NAME_STR <PLUGIN_HEADER_NAME>

// Include plugin header
#include PLUGIN_HEADER_NAME_STR

namespace {

/**
 * The plugin.
 */
PLUGIN_CLASS_NAME* plugin;

void reportError(const std::string& failureString, const std::string& extraInfo = std::string()) {
    std::stringstream stream;
    if(!extraInfo.empty()) {
        stream << "Plugin " << PLUGIN_CLASS_NAME_STR << ' ' << failureString << ": " << extraInfo << '\n';
    }
    else {
        stream << "Plugin " << PLUGIN_CLASS_NAME_STR << ' ' << failureString << '\n';
    }
    const std::string errorString = stream.str();
    XPLMDebugString(errorString.c_str());
    std::cerr << errorString;
}

}


PLUGIN_API int XPluginStart(
                        char *		outName,
                        char *		outSig,
                        char *		outDesc)
{
    try {
        //Init the plugin
        plugin = new PLUGIN_CLASS_NAME();

        //Get the plugin's information
        std::strcpy(outName, plugin->getName().c_str());
        std::strcpy(outSig, plugin->getSignature().c_str());
        std::strcpy(outDesc, plugin->getDescription().c_str());

        // Indicate success
        return 1;
    }
    catch (std::exception& ex) {
        reportError("failed to start (constructor threw exception)", ex.what());
        return 0;
    }
    catch (...) {
        reportError("failed to start (constructor threw something)");
        return 0;
    }
}

PLUGIN_API void	XPluginStop()
{
    try {
        delete plugin;
    }
    catch (std::exception& ex) {
        reportError("destructor threw exception", ex.what());
    }
    catch (...) {
        reportError("destructor threw something");
    }
}


PLUGIN_API void XPluginDisable()
{
    try {
        plugin->onDisable();
    }
    catch (std::exception& ex) {
        reportError("failed to disable (onDisable threw exception)", ex.what());
    }
    catch (...) {
        reportError("failed to disable (onDisable threw something)");
    }
}

PLUGIN_API void XPluginEnable()
{
    try {
        plugin->onEnable();
    }
    catch (std::exception& ex) {
        reportError("failed to enable (onEnable threw exception)", ex.what());
    }
    catch (...) {
        reportError("failed to enable (onEnable threw something)");
    }
}

PLUGIN_API void XPluginReceiveMessage(
                    XPLMPluginID	inFromWho,
                    long			inMessage,
                    void *			inParam)
{
    try {
        plugin->onMessageReceived(inFromWho, inMessage, inParam);
    }
    catch (std::exception& ex) {
        reportError("onMessageReceived threw exception", ex.what());
    }
    catch (...) {
        reportError("onMessageReceived threw something");
    }
}
