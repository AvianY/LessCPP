#ifndef JOIN_DIALOG_CONTEXT_LESSCPP
#define JOIN_DIALOG_CONTEXT_LESSCPP

#include "contexts/context.h"

#include "const_globals.h"
#include "less.h"

#include "SFMLWidgets/button.h"
#include "SFMLWidgets/text_input.h"
#include "helpers.h"

#include <regex>

class JoinDialogContext : public Context {
private:
    widget::TextInput IPTextInput = widget::TextInput("IP", 15, Medium, "127.0.0.1",
        [](std::string t) { return std::regex_match(t, std::regex("(\\d\\d?\\d?\\.){3}\\d\\d?\\d?")); } );
    widget::Button quitButton = widget::Button("Quit", Medium);

    sf::IpAddress ip_player2 = "127.0.0.1";
    unsigned short tcp_port = 53012;

    Context* p_return_context;

public:

    JoinDialogContext(Context*);

    Context* processEvent(const sf::Event&) override;
    sf::Texture render() override;
};


#endif // JOIN_DIALOG_CONTEXT_LESSCPP
