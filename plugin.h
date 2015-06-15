
//          Copyright Sam Crow 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

    /**
     * Destructor: called when the plugin system is about to terminate this plugin.
     * Plugins should use this to deallocate their resources.
     */
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
     * onDisable event: Called when the plugin system is about to disable this plugin
     */
	virtual void disable() = 0;

    /**
     * onEnable event: Called when the plugin system wants to enable this plugin.
     * This will be called once after the plugin is created and later if the user manually
     * disables and then enables the plugin.
     *
     * @throws Implementation should throw exceptions if the plugin cannot be enabled.
     * Exceptions will be caught and reported.
     */
	virtual void enable() = 0;

    /**
     * onMessageReceived event: Called when this plugin receives a message from another
     * plugin. See the documentation on the XPluginReceiveMessage function for more information.
     */
	virtual void messageReceived(XPLMPluginID sender, long message, void * data);

protected:


    /**
     * Writes a string to X-Plane's log.txt file
     * @param message The text to write. A newline will automatically be appended.
     */
    void debug(std::string message);

private:

    /**
	  The name of this plugin. This is sent to X-plane to display in the plugin admin.
      */
    std::string name;

    /**
	  The signature of this plugin (like org.samcrow.plugin_name). This is sent to X-plane to display in the plugin admin.
      */
    std::string signature;

    /**
	  The description of this plugin. This is sent to X-plane to display in the plugin admin.
      */
    std::string description;

};

#endif // PLUGIN_H
