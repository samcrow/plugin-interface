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

#ifndef PLUGIN_H
#define PLUGIN_H

#include <XPLMUtilities.h>
#include <string>


/**
 *@brief An abstract base class for an X-plane plugin
 *
 * A plugin should inherit from this class
  */
class Plugin
{
public:

    /**
     * Constructor. Provides plugin information to X-Plane.
     *
     * The constructor will be called when the plugin is started, from the
     * XPluginStart callback
     *
     * @param name The name of the plugin
     * @param signature The plugin signature
     * @param description A description of the plugin
     */
    Plugin(const std::string& name, const std::string& signature, const std::string& description);

    /// Destructor
    virtual ~Plugin() = default;

    /**
     * @return the plugin's name
     */
    std::string getName() {
        return name;
    }

    /**
     * @return the plugin's signature
     */
    std::string getSignature() {
        return signature;
    }

    /**
     * @return this plugin's description
     */
    std::string getDescription() {
        return description;
    }

    /*
     * Events that a plugin can receive.
     * These are all virtual.
     * A plugin that wants to use an event should override its corresponding method.
     */

    /**
     * onStop event: Called when the plugin system is about to terminate this plugin.
     * Plugins should use this to deallocate their resources. The plugin will be
     * deleted after this event completes.
     */
    virtual void onStop() = 0;

    /**
     * onDisable event: Called when the plugin system is about to disable this plugin
     */
    virtual void onDisable() = 0;

    /**
     * onEnable event: Called when the plugin system wants to enable this plugin.
     * This will be called once after the plugin is created and later if the user manually
     * disables and then enables the plugin.
     *
     * @throws Implementation should throw exceptions if the plugin cannot be enabled.
     * Exceptions will be caught and reported.
     */
    virtual void onEnable() = 0;

    /**
     * onMessageReceived event: Called when this plugin receives a message from another
     * plugin. See the documentation on the XPluginReceiveMessage function for more information.
     */
    virtual void onMessageReceived(XPLMPluginID sender, long message, void * data);

protected:


    /**
     * Writes a string to X-Plane's log.txt file
     * @param message The text to write. A newline will automatically be appended.
     */
    void debug(std::string message);

private:

    /**
      The name of this plugin. This is sent to X-plane to display in the plugin admin.

      Individual plugins should set this value in their constructors.
      */
    std::string name;

    /**
      The signature of this plugin (like org.samcrow.plugin_name). This is sent to X-plane to display in the plugin admin.

      Individual plugins should set this value in their constructors.
      */
    std::string signature;

    /**
      The description of this plugin. This is sent to X-plane to display in the plugin admin.

      Individual plugins should set this value in their constructors.
      */
    std::string description;

};

#endif // PLUGIN_H
