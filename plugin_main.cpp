
//          Copyright Sam Crow 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstring>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <memory>
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
std::unique_ptr<PLUGIN_CLASS_NAME> plugin;

namespace {

/**
 * Reports a plugin error
 * @param failureString A string with information on what failed
 * @param extraInfo An optional string with more information on the error
 */
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

}


PLUGIN_API int XPluginStart(
                        char *		outName,
                        char *		outSig,
                        char *		outDesc)
{
    try {
        //Init the plugin
		plugin.reset(new PLUGIN_CLASS_NAME());

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
		reportError("failed to start (constructor threw an unrecognized type)");
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
		reportError("destructor threw an unrecognized type");
    }
}


PLUGIN_API void XPluginDisable()
{
    try {
		plugin->disable();
    }
    catch (std::exception& ex) {
        reportError("failed to disable (onDisable threw exception)", ex.what());
    }
    catch (...) {
		reportError("failed to disable (onDisable threw an unrecognized type)");
    }
}

PLUGIN_API void XPluginEnable()
{
    try {
		plugin->enable();
    }
    catch (std::exception& ex) {
		reportError("failed to enable (enable threw exception)", ex.what());
    }
    catch (...) {
		reportError("failed to enable (ennable threw an unrecognized type)");
    }
}

PLUGIN_API void XPluginReceiveMessage(
                    XPLMPluginID	inFromWho,
                    long			inMessage,
                    void *			inParam)
{
    try {
		plugin->messageReceived(inFromWho, inMessage, inParam);
    }
    catch (std::exception& ex) {
        reportError("onMessageReceived threw exception", ex.what());
    }
    catch (...) {
		reportError("onMessageReceived threw an unrecognized type");
    }
}
