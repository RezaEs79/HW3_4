#include "server.h"

Server::Server() { }

std::vector<User> Server::get_users()
{
    return users;
}

std::map<std::string, std::string> Server::get_public_keys()
{
    return public_keys;
}

std::vector<Message*> Server::get_messages()
{
    return messages;
}

User Server::create_user(std::string username)
{
    for (auto u : users) {
        if (username == u.get_username())
            throw std::logic_error("Error");
    }

    std::string public_key {}, private_key {};
    crypto::generate_key(public_key, private_key);
    User newuser { username, private_key, this };
    users.push_back(newuser);
    public_keys.insert({ username, public_key });
    return newuser;
}
char caster_from_unsigned(unsigned char ch)
{
    return static_cast<char>(ch);
}

bool Server::create_message(Message* msg, std::string signature)
{
    bool authentic { false };
    bool flag { false };
    for (auto u : users) {
        if (msg->get_receiver() == u.get_username())
            flag = true;
    }

    if ((msg->get_type()) == "text" && flag) {
        TextMessage* msg2 { dynamic_cast<TextMessage*>(msg) };
        // authentic = crypto::verifySignature(public_keys[msg->get_sender()], "my data", signature);
        authentic = crypto::verifySignature(public_keys[msg->get_sender()], msg2->get_text(), signature);
        if (authentic) {
            messages.push_back(msg);
            TextMessage* textmsg = reinterpret_cast<TextMessage*>(this->get_messages()[0]);
            return true;
        }
    }

    if ((msg->get_type()) == "voice" && flag) {
        VoiceMessage* msg2 { dynamic_cast<VoiceMessage*>(msg) };

        std::string s;
        for (unsigned char c : msg2->get_voice())
            s.append(std::to_string(c));

        authentic = crypto::verifySignature(public_keys[msg->get_sender()], s, signature);
        // authentic = crypto::verifySignature(public_keys[msg->get_sender()], "my data", signature);
        if (authentic) {
            messages.push_back(msg);
            VoiceMessage* voicemsg = reinterpret_cast<VoiceMessage*>(this->get_messages()[0]);
            return true;
        }
    }
    return false;
}
