#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <sstream>

#include <unistd.h>
#include <ncurses.h>

#include <thread>
#include <future>
#include <mutex>

using namespace std;
using Handler = function<void(void)>;
using Contract = tuple<thread *, bool, bool>;

int queue = -1;
mutex g_lock;
map<thread::id, bool> container;

void menu() {
    cout << "Thread sync:" << endl;
    cout << "\tpress '+' to create new thread" << endl;
    cout << "\tpress '-' to delete thread" << endl;
    cout << "\tpress 'q' to close program" << endl << endl;
}

void worker(int current_id) {
    while (true) {
        g_lock.lock();

        if (container.find(this_thread::get_id()) == container.end()) {
            queue = -1;
            g_lock.unlock();
            return;
        }
        
        if (queue != current_id - 1) {
            g_lock.unlock();
            this_thread::sleep_for(chrono::milliseconds(1));
            continue;
        }
        
        if (current_id == container.size() - 1) {
            queue= -1;
        } else queue = current_id;

        if (current_id == 0) {
            this_thread::sleep_for(chrono::milliseconds(100));
            system("clear");
            menu();
        }

        stringstream stream;

        stream << "Worker message: " << gettid() << endl;
        for (char symbol : stream.str()) {
            cout << symbol;
        }
        if (container.size() == 0) {
            system("clear");
            menu();
        }
        g_lock.unlock();
    }
}

int main() {
    map<char, Handler> library = {
        { '+', [&](){
            thread *tr = new thread(worker, container.size());
            container.insert({ tr->get_id(), true });
            tr->detach();
        } },
        { '-', [&](){
            container.erase(container.begin()->first);
            if (container.size() == 0) {
                system("clear");
                menu();
            }
        } },
        { 'q', [&](){
            container.erase(container.begin(), container.end());
            exit(0);
        } },
    };

    menu();
    while (true) {
        char choice = getchar();
        auto userChoice = library.find(choice);
        if (userChoice != library.end()) {
            Handler result = library[choice];
            result();
        }
    }

    return 0;
}