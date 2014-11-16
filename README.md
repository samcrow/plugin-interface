# C++ Plugin Interface #

## Purpose ##

Writing a plugin for X-Plane requires implementing five functions (`XPluginStart`, `XPluginStop`, `XPluginEnable`, `XPluginDisable`, and `XPluginReceiveMessage`) which are usually placed in one source file. Any resources that the plugin uses must be allocated and deallocated from these callbacks.

While this style is acceptable, it has some shortcomings when used with C++:

* RAII classes that allocate resources when they are created can be problematic because they must be either global variables or pointers.
	* Global variables are constructed very early in X-Plane's execution, before the plugin SDK is available. Classes that access the plugin SDK do not work when global variables are created.
	* Pointers work around this issue by allowing objects to be created in `XPluginStart` and deleted in `XPluginStop`. However, this technique is prone to errors because each object that is created must be manually deleted.
* Plugin-specific data is not formally enclosed within a class.
* Exceptions that are thrown from plugin code may propagate out of the plugin and through X-Plane, causing a crash.
* Each plugin requires boilerplate code to provide the plugin's name, signature, and description to X-Plane.
* The `XPluginStart` callback reports failure by returning a special value, which is inconsistent with some styles of C++ programming.
* Each plugin must implement the `XPluginReceiveMessage` function, even if it does not use the messaging functionality.

This interface code provides an alternative paradigm for creating plugins. A class is created that inherits from `Plugin` and implements the methods, `onEnable`, `onDisable`, and optionally `onMessageReceived`. The constructor is called from `XPluginStart` and the destructor is called from `XPluginStop`.

This method has the following advantages:

* All plugin resources, even those that require the plugin SDK functionality, can be declared as non-pointer member variables of the plugin class. They are deleted automatically when the plugin class instance is deleted from `XPluginStop`.
* Exception safety: Any exceptions thrown from the plugin class methods will not cause X-Plane to crash. They will be logged to the log.txt file and the standard error stream.
* The plugin class constructor can throw an exception, instead of returning a special value, to indicate a failure.
* The plugin only needs to implement the `onMessageReceived` method if it uses the messaging features.

## How to use it ##

#### Brief instructions ####

1. Copy the code from this repository into your project.
2. Create a class that extends Plugin, and implement the appropriate virtual methods
3. Define the preprocessor macros `PLUGIN_CLASS_NAME` as the name of your Plugin subclass and `PLUGIN_HEADER_NAME` as the name of the header file that contains your plugin class

#### Example ####

An example project is available at https://github.com/samcrow/plugin-interface-example .

## Licensing ##

This code is licensed under the very permissive Boost license.
