#include "user.h"

User::User(std::string username, std::string private_key, Server* server)
    : server(server)
{
    this->username = username;
    this->private_key = private_key;
}

std::string User::get_username()
{
    return username;
}

bool User::send_text_message(std::string text, std::string receiver)
{
    bool out { false };
    if (this->get_username() == receiver) // This command prevents to sent msg to yourself
        return out;
    TextMessage* textmsg { new TextMessage(text, this->get_username(), receiver) };
    // std::string signature = crypto::signMessage(private_key, "my data");
    std::string signature = crypto::signMessage(private_key, textmsg->get_text());
    out = server->create_message(textmsg, signature);
    return out;
}

bool User::send_voice_message(std::string receiver)
{
    bool out { false };
    if (this->get_username() == receiver) // This command prevents to sent msg to yourself
        return out;
    VoiceMessage* voicemsg { new VoiceMessage(this->get_username(), receiver) };
    std::string s;
    for (unsigned char c : voicemsg->get_voice())
        s.append(std::to_string(c));
    std::string signature = crypto::signMessage(private_key, s);
    // std::string signature = crypto::signMessage(private_key, "my data");
    out = server->create_message(voicemsg, signature);
    return out;
}