#pragma execution_character_set("utf-8")
#include <iostream>
#include <Windows.h>
#include <thread>
#include <atomic>
#include <map>
#include <chrono>

class translator
{
private:
    std::atomic<bool> running;
    std::map<int, int> keyTranslate;
    std::thread detection;
    std::map<int, bool> state;
public:
    translator() : running(false)
    {
        for (const auto& translating : keyTranslate)
        {
            state[translating.first] = false;
        }
    }

    ~translator()
    {
        stop();
    }

    void Translating(int fromVK, int toVK)
    {
        keyTranslate[fromVK] = toVK;
        state[fromVK] = false;
    }

    void CharM(char fromChar, char toChar)
    {
        int fromVk = VkKeyScanA(fromChar) & 0xFF;
        int toVk = VkKeyScanA(toChar) & 0xFF;
        Translating(fromVk, toVk);
    }

    void StartTranslationLoop()
    {
        if (running) return;

        running = true;
        detection = std::thread(&translator::listenForKeyPresses, this);

        std::cout << "\n";
        std::cout << "    [+] Running!\n";
        std::cout << "    [*] Press F12 to stop\n";
    }

    void stop() {
        running = false;
        if (detection.joinable()) {
            detection.join();
        }
    }

    void clearTranslations() {
        keyTranslate.clear();
        state.clear();
    }
private:
    void listenForKeyPresses() {
        while (running) {
            for (const auto& translating : keyTranslate) {
                int vkCode = translating.first;

                if (GetAsyncKeyState(vkCode) & 0x8000) {
                    if (!state[vkCode]) {
                        state[vkCode] = true;
                        translateKey(vkCode, translating.second);
                    }
                }
                else {
                    state[vkCode] = false;
                }
            }

            if (GetAsyncKeyState(VK_F12) & 0x8000) {
                std::cout << "    [*] Stopping...\n";
                running = false;
                break;
            }

            Sleep(10);
        }
    }

    void translateKey(int originalVK, int newVK) {
        SHORT newKeyScan = VkKeyScanA(newVK);
        bool needShift = (newKeyScan & 0xFF00) != 0;

        simulateKeyPress(VK_BACK);
        Sleep(15);

        if (needShift) {
            simulateKeyPress(VK_SHIFT, false);
        }

        simulateKeyPress(newVK);

        if (needShift) {
            simulateKeyPress(VK_SHIFT, true);
        }
        Sleep(50);
    }

    void simulateKeyPress(int vkCode, bool release = true) {
        keybd_event(vkCode, 0, 0, 0);
        if (release) {
            keybd_event(vkCode, 0, KEYEVENTF_KEYUP, 0);
        }
    }
};

void showMenu() {
    system("cls");
    std::cout << "\n";
    std::cout << R"(
    ██╗      █████╗ ██╗   ██╗ ██████╗ ██╗   ██╗████████╗   
    ██║     ██╔══██╗╚██╗ ██╔╝██╔═══██╗██║   ██║╚══██╔══╝
    ██║     ███████║ ╚████╔╝ ██║   ██║██║   ██║   ██║      [*] Change your keyboard layout without admin permissions.
    ██║     ██╔══██║  ╚██╔╝  ██║   ██║██║   ██║   ██║      [*] https://github.com/zanokulol
    ███████╗██║  ██║   ██║   ╚██████╔╝╚██████╔╝   ██║      [*] https://zanoku.lol
    ╚══════╝╚═╝  ╚═╝   ╚═╝    ╚═════╝  ╚═════╝    ╚═╝  
)" << "\n";

    std::cout << "    ============================================================================================\n";
    std::cout << "\n";

    std::cout << "    [1] Start Translation\n";
    std::cout << "    [2] Change Layout\n";
    std::cout << "    [3] Create Custom Layout\n";
    std::cout << "    [4] Exit\n";
}

void applyColemakLayout(translator& remap) {
    remap.clearTranslations();

    remap.CharM('a', 'a');
    remap.CharM('s', 'r');
    remap.CharM('d', 's'); 
    remap.CharM('f', 't');
    remap.CharM('g', 'd');
    remap.CharM('h', 'h');
    remap.CharM('j', 'n');
    remap.CharM('k', 'e');
    remap.CharM('l', 'i');
    remap.CharM(';', 'o');
    remap.CharM(0xE4, 0x27);

    remap.CharM('q', 'q');
    remap.CharM('w', 'w');
    remap.CharM('e', 'f');
    remap.CharM('r', 'p');
    remap.CharM('t', 'g');
    remap.CharM('z', 'y');
    remap.CharM('u', 'l');
    remap.CharM('i', 'u');
    remap.CharM('o', 'y');
    remap.CharM('p', ';');
    remap.CharM(0xFC, '[');
    remap.CharM('+', ']');

    remap.CharM('y', 'z');
    remap.CharM('x', 'x');
    remap.CharM('c', 'c');
    remap.CharM('v', 'v');
    remap.CharM('b', 'b');
    remap.CharM('n', 'k');
    remap.CharM('m', 'm');
    remap.CharM(',', ',');
    remap.CharM('.', '.');
    remap.CharM('-', '-');

    remap.CharM('"', '"');
    remap.CharM(0xA7, '#');
    remap.CharM('&', '&');
    remap.CharM('/', '/');
    remap.CharM('(', '(');
    remap.CharM(')', ')');
    remap.CharM('=', '=');
    remap.CharM('?', '\\');
    remap.CharM(0xB0, '`');
    remap.CharM('^', '~');

    remap.CharM('>', '>');
    remap.CharM('<', '<');
    remap.CharM(':', ':');
    remap.CharM('_', '_');
    remap.CharM('*', '*');
    remap.CharM(0xC4, '{');
    remap.CharM(0xD6, '}');
    remap.CharM(0xDC, '|');
}

int main() {
    translator translate;
    int currentLayout = 0;

    SetConsoleOutputCP(65001);
    SetConsoleTitleA("Key Translator");

    while (true) {
        showMenu();

        std::cout << "    [*] Your option: ";
        int menuChoice;
        std::cin >> menuChoice;

        switch (menuChoice) {
        case 1:
            if (currentLayout == 0) {
                std::cout << "    [-] No layout selected! Please select a layout first.\n";
                std::cout << "    [*] Press any key to continue...\n";
                std::cin.ignore();
                std::cin.get();
            }
            else {
                std::cout << "    [+] Starting translation with layout " << currentLayout << "...\n";
                translate.StartTranslationLoop();

                while (true) {
                    if (GetAsyncKeyState(VK_F12) & 0x8000) {
                        translate.stop();
                        std::cout << "    [*] Translation stopped. Returning to menu...\n";
                        Sleep(1000);
                        break;
                    }
                    Sleep(100);
                }
            }
            break;

        case 2:
        {
            system("cls");
            std::cout << "\n    === Layout Selection ===\n";
            std::cout << "    [1] Colemak\n";
            std::cout << "    [2] Default\n";
            std::cout << "    [3] Back to main menu\n";
            std::cout << "    [*] Select layout: ";

            int layoutChoice;
            std::cin >> layoutChoice;

            switch (layoutChoice) {
            case 1:
                applyColemakLayout(translate);
                currentLayout = 1;
                std::cout << "    [+] Colemak layout applied!\n";
                break;
            case 2:
                translate.clearTranslations();
                currentLayout = 2;
                std::cout << "    [+] Default layout applied!\n";
                break;
            case 3:
                break;
            default:
                std::cout << "    [-] Invalid layout selection!\n";
                break;
            }

            if (layoutChoice != 3) {
                std::cout << "    [*] Press any key to continue...";
                std::cin.ignore();
                std::cin.get();
            }
        }
        break;

        case 3:
            std::cout << "    [+] Custom layout feature coming soon!\n";
            std::cout << "    [*] Press any key to continue...";
            std::cin.ignore();
            std::cin.get();
            break;

        case 4:
            std::cout << "    [*] Goodbye!\n";
            return 0;

        default:
            std::cout << "    [-] Invalid selection! Please choose 1-4.\n";
            std::cout << "    [*] Press any key to continue...";
            std::cin.ignore();
            std::cin.get();
            break;
        }
    }

    return 0;
}