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

std::vector<Message*> Server::get_all_messages_from(std::string username)
{

    std::vector<Message*> msgs_from(messages.size());
    std::vector<Message*>::iterator temp = std::copy_if(messages.begin(), messages.end(), msgs_from.begin(),
        [username](Message* msg) { return msg->get_sender() == username; });
    msgs_from.erase(temp, msgs_from.end());
    return msgs_from;
}

std::vector<Message*> Server::get_all_messages_to(std::string username)
{
    std::vector<Message*> msgs_to(messages.size());
    std::vector<Message*>::iterator temp = std::copy_if(messages.begin(), messages.end(), msgs_to.begin(),
        [username](Message* msg) { return msg->get_receiver() == username; });
    msgs_to.erase(temp, msgs_to.end());
    return msgs_to;
}

std::vector<Message*> Server::get_chat(std::string user1, std::string user2)
{
    std::vector<Message*> msg_chat(messages.size());
    std::vector<Message*>::iterator temp = std::copy_if(messages.begin(), messages.end(), msg_chat.begin(),
        [user1, user2](Message* msg) { return (msg->get_receiver() == user1 && msg->get_sender() == user2 || msg->get_receiver() == user2 && msg->get_sender() == user1); });
    msg_chat.erase(temp, msg_chat.end());
    return msg_chat;
}

void Server::sort_msgs(std::vector<Message*>& msgs)
{
    sort(msgs.begin(), msgs.end(), [](Message* time1, Message* time2) {
        std::string stime1 = time1->get_time();
        std::string stime2 = time2->get_time();
        struct tm timeDate1;
        struct tm timeDate2;
        strptime(stime1.c_str(), "%a %b %e %H:%M:%S %Y", &timeDate1);
        strptime(stime2.c_str(), "%a %b %e %H:%M:%S %Y", &timeDate2);
        // time_t t1 = std::mktime(&timeDate1); time_t t2 = std::mktime(&timeDate2);  difftime(t1,t2) > 0.0 ? 1 : -1; //sometimes this works. easier!
        // we should use: tm_year,tm_yday,tm_hour,tm_min,tm_sec
        if (timeDate1.tm_year != timeDate2.tm_year)
            return timeDate1.tm_year < timeDate2.tm_year;
        if (timeDate1.tm_yday != timeDate2.tm_yday)
            return timeDate1.tm_yday < timeDate2.tm_yday;
        if (timeDate1.tm_hour != timeDate2.tm_hour)
            return timeDate1.tm_hour < timeDate2.tm_hour;
        if (timeDate1.tm_min != timeDate2.tm_min)
            return timeDate1.tm_min < timeDate2.tm_min;
        if (timeDate1.tm_sec != timeDate2.tm_sec)
            return timeDate1.tm_sec < timeDate2.tm_sec;
        return false;
    });
}
