#include <functional>
#include <iostream>
#include <vector>

struct MqttExampleEvent
{
    int your;
    int mighty;
    int data;
};

// forward declaration needed for callback_t, a downside of two way association(or two way dependency idk what called) :)
class MqttExampleClass;
using callback_t = std::function<void(MqttExampleClass &, MqttExampleEvent)>;

class MqttExampleClass
{
public:
    int field1;

    std::vector<callback_t> listeners;

    void addListener(callback_t listener) { listeners.push_back(listener); }

    void onSomethingTriggered(int your, int mighty, int data)
    {
        std::cout << "Received a data!\n";
        MqttExampleEvent event{your, mighty, data};

        for (callback_t &listener : listeners)
        {
            listener(*this, event);
        }
    }

    void test() { std::cout << "hi there from the class.test()!\n"; }
};

/* Listeners */
void aListener(MqttExampleClass &parentClass, MqttExampleEvent event)
{
    std::cout << "Hi there, " << event.your << ", " << event.mighty << ", "
              << event.data << "\n";
}

void anotherListener(MqttExampleClass &parentClass, MqttExampleEvent event)
{
    std::cout << "WOW, " << event.your << "\n";
    parentClass.test();
}

int main()
{
    MqttExampleClass cls;
    cls.addListener(aListener);
    cls.addListener(anotherListener);
    cls.addListener([](MqttExampleClass &parentClass, MqttExampleEvent event)
                    { std::cout << "maybe another listener\n"; });

    // Somewhere else in the code.
    cls.onSomethingTriggered(1, 2, 3);
    return 0;
}