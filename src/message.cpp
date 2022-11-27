#include "message.h"

Message::Message(std::string type, std::string sender, std::string receiver)
{
    this->type = type;
    this->sender = sender;
    this->receiver = receiver;

    std::time_t now = std::time(0);
    std::string tm = std::ctime(&now);
    this->time = tm.erase(tm.find_last_not_of(" \n\r\t") + 1);
}

Message::Message()
    : Message("", "", "") {};

std::string Message::get_type() const
{
    return type;
}

std::string Message::get_sender() const
{
    return sender;
}

std::string Message::get_receiver() const
{
    return receiver;
}

std::string Message::get_time() const
{
    return time;
}

void Message::print(std::ostream& os) const
{
    os << "*************************" << std::endl;
    os << get_sender() << " ->  " << get_receiver() << std::endl;
    os << "message type: " << get_type() << std::endl;
    os << "message time: " << get_time() << std::endl;
    os << "*************************" << std::endl;
}
std::ostream& operator<<(std::ostream& os, const Message& c)
{

    c.print(os);
    return os;
}

// TextMessage Class

TextMessage::TextMessage(std::string text, std::string sender, std::string receiver)
    : Message("text", sender, receiver)
{
    this->text = text;
}

std::string TextMessage::get_text() const
{
    return text;
}

void TextMessage::print(std::ostream& os) const
{

    os << "*************************" << std::endl;
    os << get_sender() << " ->  " << get_receiver() << std::endl;
    os << "message type: " << get_type() << std::endl;
    os << "message time: " << get_time() << std::endl;
    os << "text: " << get_text() << std::endl;
    os << "*************************" << std::endl;
}

// VoiceMessage Class

VoiceMessage::VoiceMessage(std::string sender, std::string receiver)
    : Message("voice", sender, receiver)
{
    for (size_t i = 0; i < 5; i++) {
        unsigned char temp = rand() % 256;
        voice.push_back(temp);
    }
}

std::vector<unsigned char> VoiceMessage::get_voice() const
{
    return voice;
}

void VoiceMessage::print(std::ostream& os) const
{
    std::string str;
    for (size_t i = 0; i < 5; i++) {
        str.append(std::to_string(static_cast<int>(get_voice()[i])));
        str.append(" ");
    }
    os << "*************************" << std::endl;
    os << get_sender() << " ->  " << get_receiver() << std::endl;
    os << "message type: " << get_type() << std::endl;
    os << "message time: " << get_time() << std::endl;
    os << "voice: " << str << std::endl;
    os << "*************************" << std::endl;
}
