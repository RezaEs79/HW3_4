#ifndef MESSAGE_H
#define MESSAGE_H

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

class Message {
public:
    Message(std::string type, std::string sender, std::string receiver);
    Message();

    std::string get_type() const;
    std::string get_sender() const;
    std::string get_receiver() const;
    std::string get_time() const;

    virtual void print(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream& os, const Message& c);

private:
    std::string type; // type of the message ("text", "voice", ...)
    std::string sender; // the username who send this message
    std::string receiver; // the username whom this message is intended for
    std::string time; // creation time of the message
};

class TextMessage : public Message {
public:
    TextMessage(std::string text, std::string sender, std::string receiver);
    std::string get_text() const;
    void print(std::ostream& os) const override;

private:
    std::string text;
};

class VoiceMessage : public Message {
public:
    VoiceMessage(std::string sender, std::string receiver);
    std::vector<unsigned char> get_voice() const;
    void print(std::ostream& os) const override;

private:
    std::vector<unsigned char> voice;
};

#endif // MESSAGE_H