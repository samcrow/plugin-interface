
//          Copyright Sam Crow 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "plugin.h"

Plugin::Plugin(const std::string& name, const std::string& signature, const std::string& description, action_type preSubclassAction) :
    name(name),
    signature(signature),
    description(description)
{
    if(preSubclassAction) {
        preSubclassAction();
    }
}

void Plugin::debug(std::string message) {
    message.append("\n");
    XPLMDebugString(message.c_str());
}


void Plugin::onMessageReceived(XPLMPluginID /*sender*/, long /*message*/, void * /*data*/) {

}
