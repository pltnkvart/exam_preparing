#include <iostream>
#include <vector>
#include <algorithm>

class Message {
protected:
    int sender_id;
    std::string time;
    bool isRead;
public:
    virtual void print(std::ostream &os) = 0;

    virtual ~Message() = default;

    bool getIsRead() { return isRead; }
};

class Photo : public Message {
private:
    int file_id;
public:
    Photo(int send_id, std::string _time, int _file_id) {
        sender_id = send_id;
        time = _time;
        isRead = false;
        file_id = _file_id;
    }

    [[nodiscard]] int getFileID() const { return file_id; }

    void print(std::ostream &os) override {
        os << "www.telega.org/img/" << getFileID() << std::endl;
    }
};

class Audio : public Message {
private:
    int file_id;
    std::string text;
public:
    Audio(int send_id, std::string _time, int _file_id, std::string _text) {
        sender_id = send_id;
        time = _time;
        isRead = false;
        file_id = _file_id;
        text = _text;
    }

    [[nodiscard]] int getFileID() const { return file_id; }

    [[nodiscard]] std::string getMsg() const { return text; }

    void print(std::ostream &os) override {
        os << "www.telega.org/audio/" << getFileID() << " Text in audio - " << getMsg() << std::endl;
    }
};

class Text : public Message {
private:
    std::string text;
public:
    Text(int send_id, std::string _time, std::string _text) {
        sender_id = send_id;
        time = _time;
        isRead = false;
        text = _text;
    }

    [[nodiscard]] std::string getMsg() const { return text; }

    void print(std::ostream &os) override {
        os << getMsg() << std::endl;
    }
};

class Contact {
private:
    int id;
    std::string login;
    std::string phoneNumber;
public:
    Contact(int _id, std::string _login, std::string _phoneNumber) {
        id = _id;
        login = _login;
        phoneNumber = _phoneNumber;
    }

    [[nodiscard]] int getID() const { return id; }

    [[nodiscard]] std::string getLogin() const { return login; }
};

class Chat {
private:
    Contact user;
    std::vector<Message *> msgs;

public:
    Chat(const Contact &_contact) : user(_contact) {}

    [[nodiscard]] Contact getContact() const { return user; }

    [[nodiscard]] const std::vector<Message *> &getMsgs() const { return msgs; }

    void addMessage(Message *newMsg) {
        msgs.push_back(newMsg);
    }

    void printUnread(std::ostream &os) {
        for (auto msg: msgs) {
            if (!msg->isRead) {
                msg->print(os);
                msg->isRead = true;
            }
        }
    }

    ~Chat() {
        for (auto msg: msgs) {
            delete msg;
        }
    }
};

class Telega {
private:
    std::vector<Chat *> chats;

public:
    Telega(std::vector<Chat *> &_chats) : chats(_chats) {}

    std::vector<Chat *> &getChats() { return chats; }

    void addChat(Chat *newChat) {
        chats.push_back(newChat);
        std::sort(chats.begin(), chats.end(), [](Chat *a, Chat *b) {
            return a->getContact().getLogin() < b->getContact().getLogin();
        });
    }

    Chat *operator[](int index) {
        return chats[index];
    }

    Chat *operator[](std::string login) {
        for (auto chat: chats) {
            if (chat->getContact().getLogin().compare(login)) {
                return chat;
            }
        }
        return nullptr;
    }

    friend std::ostream &operator<<(std::ostream &os, const Telega &app) {
        for (auto chat: app.chats) {
            os << "Chat with " << chat->getContact().getLogin() << ":" << std::endl;
            for (auto message: chat->getMsgs()) {
                os << message->sender_id << " (" << message->time << "): ";
                message->print(os);
            }
        }
        return os;
    }

    ~Telega() {
        for (auto chat: chats) {
            delete chat;
        }
    }
};

int main() {
    Contact contact1(42, "Artem", "123456789");
    Contact contact2(2123, "Borya", "123456789");
    Chat *chat1 = new Chat(contact1);
    Chat *chat2 = new Chat(contact2);

    Text *textMessage1 = new Text(42, "12:00", "Hello!");
    Photo *photoMessage1 = new Photo(42, "12:05", 1001);
    Audio *audioMessage1 = new Audio(42, "12:10", 2001, "vibe!");


    chat1->addMessage(textMessage1);
    chat1->addMessage(photoMessage1);
    chat1->addMessage(audioMessage1);

    Text *textMessage2 = new Text(2123, "12:10", "Hello snus!");
    Photo *photoMessage2 = new Photo(2123, "12:23", 1001);
    chat2->addMessage(textMessage2);
    chat2->addMessage(photoMessage2);


    std::vector<Chat *> chats;
    chats.push_back(chat1);

    Telega telegram(chats);
    telegram.addChat(chat2);

    std::cout << telegram;

    return 0;
}
