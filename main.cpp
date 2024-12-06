#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <sstream>

class Subject {
public:
    std::string
    f3(const std::map<std::string, std::string> &args) {    // Обобщённый метод f3, который выполняет разные операции над текстом
        std::string operation = args.at("operation");
        std::string text = args.at("text");

        if (operation == "replace_word") {
            return replace_word(text, args.at("target"), args.at("replacement"));
        } else if (operation == "find_phrase") {
            return find_phrase(text, args.at("phrase"));
        } else if (operation == "capitalize_words") {
            return capitalize_words(text);
        } else if (operation == "capitalize_sentences") {
            return capitalize_sentences(text);
        } else if (operation == "number_sentences") {
            return number_sentences(text);
        } else {
            return "Unknown operation";
        }
    }

private:
    static std::string
    replace_word(const std::string &text, const std::string &target, const std::string &replacement) {
        std::string result = text;
        size_t pos = 0;
        while ((pos = result.find(target, pos)) != std::string::npos) {
            result.replace(pos, target.length(), replacement);
            pos += replacement.length();
        }
        return result;
    }

    static std::string find_phrase(const std::string &text, const std::string &phrase) {
        return (text.find(phrase) != std::string::npos) ? "Found" : "Not Found";
    }

    static std::string capitalize_words(const std::string &text) {
        std::string result = text;
        bool capitalize = true;
        for (char &c: result) {
            if (std::isspace(c)) {
                capitalize = true;
            } else if (capitalize) {
                c = std::toupper(c);
                capitalize = false;
            }
        }
        return result;
    }

    static std::string capitalize_sentences(const std::string &text) {
        std::string result = text;
        bool capitalize = true;
        for (char &c: result) {
            if (capitalize && std::isalpha(c)) {
                c = std::toupper(c);
                capitalize = false;
            }
            if (c == '.' || c == '!' || c == '?') {
                capitalize = true;
            }
        }
        return result;
    }

    static std::string number_sentences(const std::string &text) {
        std::stringstream ss(text);
        std::string sentence;
        std::string result;
        int number = 1;

        while (std::getline(ss, sentence, '.')) {
            if (!sentence.empty()) {
                result += std::to_string(number++) + ". " + sentence + ".\n";
            }
        }
        return result;
    }
};

class Wrapper {
public:
    Wrapper(Subject *subj, std::function<std::string(const std::map<std::string, std::string> &)> func)
            : subject_(subj), func_(func) {}

    std::string execute(const std::map<std::string, std::string> &args) {
        return func_(args);
    }

private:
    Subject *subject_;
    std::function<std::string(const std::map<std::string, std::string> &)> func_;
};

class Engine {
public:
    void register_command(const std::string &name, Wrapper *wrapper) {
        commands_[name] = wrapper;
    }

    std::string execute(const std::string &command, const std::map<std::string, std::string> &args) {
        if (commands_.find(command) != commands_.end()) {
            return commands_[command]->execute(args);
        } else {
            return "Command not found!";
        }
    }

private:
    std::map<std::string, Wrapper *> commands_;
};

int main() {
    Subject subj;

    // Обёртка для вызова универсального метода f3
    Wrapper wrapper(&subj, [&](const std::map<std::string, std::string> &args) {
        return subj.f3(args);
    });

    Engine engine;
    engine.register_command("command1", &wrapper);

    // Выполнение замены слов через f3
    std::cout << engine.execute("command1", {{"operation",   "replace_word"},
                                             {"text",        "hello world"},
                                             {"target",      "world"},
                                             {"replacement", "everyone"}}) << std::endl;

    // Выполнение поиска фразы
    std::cout << engine.execute("command1", {{"operation", "find_phrase"},
                                             {"text",      "hello world"},
                                             {"phrase",    "hello"}}) << std::endl;

    return 0;
}