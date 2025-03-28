#include "letter.h"
#include "kvp.h"
#include "table.h"
#include "parser.h"
#include "ciphertext.h"
#include <bitset>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifndef NO_NCURSES
#include <ncurses.h>
#include <panel.h>
#endif

using namespace std::chrono;
using namespace std;

int main()
{
    table t;
    char lol[] = "abcdefghijklmnopqrstuvwxyz;',.! ";
    for (int x = 0; x < 32; x++){
        bitset<5> in(x+1);
        t.put(lol[x],in);
    }

#ifndef NO_NCURSES
    // Ncurses interface
    initscr();            // Initialize the window
    start_color();        // Start color functionality
    cbreak();             // Line buffering disabled
    noecho();             // Don't echo() while we do getch
    keypad(stdscr, TRUE); // Enable F1, F2 etc..

    // Initialize color pairs
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);

    int height, width;
    getmaxyx(stdscr, height, width); // Get the screen dimensions

    // Menu options
    const char *options[] = {
        "Custom",
        "waltz, nymph, for quick jigs vex bud.",
        "sphinx of black quartz, judge my vow.",
        "pack my box with five dozen liquor jugs.",
        "mr. jock, tv quiz phd, bags few lynx.",
        "Decrypter Mode"
    };
    int n_options = sizeof(options) / sizeof(options[0]);
    int choice = 0;

    // Display menu
    while (true) {
        clear();
        mvprintw(0, 0, "Select a plaintext option:");
        for (int i = 0; i < n_options; i++) {
            if (i == choice) {
                attron(A_REVERSE);
            }
            mvprintw(i + 1, 0, "%s", options[i]);
            if (i == choice) {
                attroff(A_REVERSE);
            }
        }
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                choice = (choice - 1 + n_options) % n_options;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % n_options;
                break;
            case '\n':
                goto selection_made;
        }
    }

selection_made:
    char test[256];
    bool decrypter_mode = false;
    if (choice == 0) {
        // Custom option selected
        clear();
        mvprintw(height / 2, (width - 30) / 2, "Enter plaintext: ");
        echo(); // Enable echoing of characters
        getnstr(test, 255); // Read user input
        noecho(); // Disable echoing of characters
    } else if (choice == 5) {
        // Decrypter mode selected
        decrypter_mode = true;
        srand(time(0));
        int random_index = rand() % 5 + 1; // Select a random predefined message
        strncpy(test, options[random_index], 255);
        test[255] = '\0'; // Ensure null termination
    } else {
        // Predefined option selected
        strncpy(test, options[choice], 255);
        test[255] = '\0'; // Ensure null termination
    }
#else
    // Simple console interface
    cout << "DotsiesCipherGen - Plain text mode" << endl;
    cout << "Available test phrases:" << endl;
    cout << "1. waltz, nymph, for quick jigs vex bud." << endl;
    cout << "2. sphinx of black quartz, judge my vow." << endl;
    cout << "3. pack my box with five dozen liquor jugs." << endl;
    cout << "4. mr. jock, tv quiz phd, bags few lynx." << endl;
    cout << "5. [custom input]" << endl;
    cout << "Enter choice (1-5): ";
    
    int choice;
    cin >> choice;
    cin.ignore(); // clear newline
    
    char test[256];
    bool decrypter_mode = false;
    
    if (choice == 5) {
        cout << "Enter your custom text: ";
        cin.getline(test, 255);
    } else if (choice >= 1 && choice <= 4) {
        const char* phrases[] = {
            "waltz, nymph, for quick jigs vex bud.",
            "sphinx of black quartz, judge my vow.",
            "pack my box with five dozen liquor jugs.",
            "mr. jock, tv quiz phd, bags few lynx."
        };
        strncpy(test, phrases[choice - 1], 255);
        test[255] = '\0';
    } else {
        cout << "Invalid choice, using default phrase" << endl;
        strncpy(test, "waltz, nymph, for quick jigs vex bud.", 255);
        test[255] = '\0';
    }
#endif

    parser ps(t);
    ciphertext out;
    std::size_t sz = strlen(test);
    out = ps.translate(test);

    if (decrypter_mode) {
        // Perform random row_shifts
        int num_shifts = rand() % 10 + 1; // Random number of shifts between 1 and 10
        for (int i = 0; i < num_shifts; i++) {
            int bit_index = rand() % 5; // Random bit index between 0 and 4
            bool direction = rand() % 2; // Random direction (0 for left, 1 for right)
            out.row_shift(bit_index, direction);
        }
    }

    char plaintext[sz + 1]; // +1 for null terminator
    ps.translate(out, plaintext);
    plaintext[sz] = '\0'; // Ensure null termination

#ifndef NO_NCURSES
    clear(); // Clear the screen

    int left_panel_height = height;
    int left_panel_width = width / 3;
    int cipher_panel_height = height;
    int cipher_panel_width = width / 3;
    int right_panel_height = height / 2;
    int right_panel_width = width / 3;
    int manip_panel_height = height / 2;
    int manip_panel_width = width / 3;
    int bitset_panel_height = 3;
    int bitset_panel_width = width;
    int start_y = 0;

    // Create left panel
    WINDOW *left_win = newwin(left_panel_height, left_panel_width, start_y, 0);
    PANEL *left_panel = new_panel(left_win);
    box(left_win, 0, 0); // Draw a box around the window

    // Print the label for the left panel
    attron(COLOR_PAIR(1));
    mvwprintw(left_win, 0, 2, " Cipher Key ");
    attroff(COLOR_PAIR(1));

    // Print the arrow indicator for the bitset index
    int bitset_index = 0;
    mvwprintw(left_win, 1, 10 + bitset_index * 2, "^");

    // Print all the letters in lol vertically in the left panel
    for (int i = 0; i < 32; i++) {
        mvwprintw(left_win, i + 2, 1, "%c - %d", lol[i], i + 1);
        // Print the bitsets of the cipher key next to the number
        auto result = t.get(lol[i]);
        bool found = result.first;
        std::bitset<5> b = result.second;
        if (found) {
            for (std::size_t j = 0; j < 5; j++) {
                if (b[j]) {
                    mvwaddch(left_win, i + 2, 10 + j * 2, ACS_CKBOARD);
                } else {
                    mvwaddch(left_win, i + 2, 10 + j * 2, ' ');
                }
            }
        }
    }

    // Create cipher panel
    WINDOW *cipher_win = newwin(cipher_panel_height, cipher_panel_width, start_y, left_panel_width);
    PANEL *cipher_panel = new_panel(cipher_win);
    box(cipher_win, 0, 0); // Draw a box around the window

    // Print the label for the cipher panel
    attron(COLOR_PAIR(1));
    mvwprintw(cipher_win, 0, 2, " Cipher ");
    attroff(COLOR_PAIR(1));

    // Print the arrow indicator for the bitset index in the cipher panel
    mvwprintw(cipher_win, 1, 10 + bitset_index * 2, "^");

    // Print the bitsets of the ciphertext in the cipher panel
    for (std::size_t i = 0; i < out.size(); i++) {
        for (std::size_t j = 0; j < 5; j++) {
            if (out.getBit(i, j)) {
                mvwaddch(cipher_win, i + 2, 10 + j * 2, ACS_CKBOARD);
            } else {
                mvwaddch(cipher_win, i + 2, 10 + j * 2, ' ');
            }
        }
    }

    // Create right panel
    WINDOW *right_win = newwin(right_panel_height, right_panel_width, start_y, left_panel_width + cipher_panel_width);
    PANEL *right_panel = new_panel(right_win);
    box(right_win, 0, 0); // Draw a box around the window

    // Print the label for the right panel
    attron(COLOR_PAIR(2));
    mvwprintw(right_win, 0, 2, " Plaintext ");
    attroff(COLOR_PAIR(2));

    // Print the plaintext in the right panel
    mvwprintw(right_win, 1, 1, "%s", plaintext);

    // Create manipulations panel
    WINDOW *manip_win = newwin(manip_panel_height, manip_panel_width, right_panel_height, left_panel_width + cipher_panel_width);
    PANEL *manip_panel = new_panel(manip_win);
    box(manip_win, 0, 0); // Draw a box around the window

    // Print the label for the manipulations panel
    attron(COLOR_PAIR(3));
    mvwprintw(manip_win, 0, 2, " Manipulations ");
    attroff(COLOR_PAIR(3));

    // Create bitset index indicator panel
    WINDOW *bitset_win = newwin(bitset_panel_height, bitset_panel_width, height - bitset_panel_height, 0);
    PANEL *bitset_panel = new_panel(bitset_win);
    box(bitset_win, 0, 0); // Draw a box around the window

    // Print the label for the bitset index indicator panel
    attron(COLOR_PAIR(4));
    mvwprintw(bitset_win, 0, 2, " Bitset Index Indicator ");
    attroff(COLOR_PAIR(4));

    int row_shift_counts[5] = {0}; // Initialize counters for row_shift manipulations

    mvwprintw(bitset_win, 1, 1, "Index: %d", bitset_index);
    for (int i = 0; i < 5; i++) {
        attron(COLOR_PAIR(5));
        mvwprintw(bitset_win, 2, 1 + i * 12, "Bit %d: %d", i, row_shift_counts[i]);
        attroff(COLOR_PAIR(5));
    }

    update_panels();
    doupdate();

    int ch;
    int manip_row = 1; // Start row for manipulations panel
    int manip_col = 1; // Start column for manipulations panel
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case KEY_UP:
                bitset_index = (bitset_index - 1 + 5) % 5;
                break;
            case KEY_DOWN:
                bitset_index = (bitset_index + 1) % 5;
                break;
            case KEY_RIGHT:
                // Print right arrow in manipulations panel with the bitset index
                mvwprintw(manip_win, manip_row, manip_col, "->%d", bitset_index);
                manip_col += 5; // Move to the next position
                if (manip_col >= manip_panel_width - 5) {
                    manip_col = 1;
                    manip_row++;
                }
                if (manip_row >= manip_panel_height - 1) {
                    manip_row = 1;
                    manip_col = 1;
                    werase(manip_win); // Clear the manipulations panel
                    box(manip_win, 0, 0);
                    attron(COLOR_PAIR(3));
                    mvwprintw(manip_win, 0, 2, " Manipulations ");
                    attroff(COLOR_PAIR(3));
                }
                // Perform row shift to the right at the bitset index
                out.row_shift(bitset_index, true);
                row_shift_counts[bitset_index]++; // Increment the counter for the bitset index
                // Regenerate and update the plaintext
                ps.translate(out, plaintext);
                plaintext[sz] = '\0'; // Ensure null termination
                werase(right_win);
                box(right_win, 0, 0);
                attron(COLOR_PAIR(2));
                mvwprintw(right_win, 0, 2, " Plaintext ");
                attroff(COLOR_PAIR(2));
                mvwprintw(right_win, 1, 1, "%s", plaintext);
                // Update the bitsets in the cipher panel
                werase(cipher_win);
                box(cipher_win, 0, 0);
                attron(COLOR_PAIR(1));
                mvwprintw(cipher_win, 0, 2, " Cipher ");
                attroff(COLOR_PAIR(1));
                mvwprintw(cipher_win, 1, 10 + bitset_index * 2, "^");
                for (std::size_t i = 0; i < out.size(); i++) {
                    for (std::size_t j = 0; j < 5; j++) {
                        if (out.getBit(i, j)) {
                            mvwaddch(cipher_win, i + 2, 10 + j * 2, ACS_CKBOARD);
                        } else {
                            mvwaddch(cipher_win, i + 2, 10 + j * 2, ' ');
                        }
                    }
                }
                break;
            case KEY_LEFT:
                // Print left arrow in manipulations panel with the bitset index
                mvwprintw(manip_win, manip_row, manip_col, "<-%d", bitset_index);
                manip_col += 5; // Move to the next position
                if (manip_col >= manip_panel_width - 5) {
                    manip_col = 1;
                    manip_row++;
                }
                if (manip_row >= manip_panel_height - 1) {
                    manip_row = 1;
                    manip_col = 1;
                    werase(manip_win); // Clear the manipulations panel
                    box(manip_win, 0, 0);
                    attron(COLOR_PAIR(3));
                    mvwprintw(manip_win, 0, 2, " Manipulations ");
                    attroff(COLOR_PAIR(3));
                }
                // Perform row shift to the left at the bitset index
                out.row_shift(bitset_index, false);
                row_shift_counts[bitset_index]--; // Decrement the counter for the bitset index
                // Regenerate and update the plaintext
                ps.translate(out, plaintext);
                plaintext[sz] = '\0'; // Ensure null termination
                werase(right_win);
                box(right_win, 0, 0);
                attron(COLOR_PAIR(2));
                mvwprintw(right_win, 0, 2, " Plaintext ");
                attroff(COLOR_PAIR(2));
                mvwprintw(right_win, 1, 1, "%s", plaintext);
                // Update the bitsets in the cipher panel
                werase(cipher_win);
                box(cipher_win, 0, 0);
                attron(COLOR_PAIR(1));
                mvwprintw(cipher_win, 0, 2, " Cipher ");
                attroff(COLOR_PAIR(1));
                mvwprintw(cipher_win, 1, 10 + bitset_index * 2, "^");
                for (std::size_t i = 0; i < out.size(); i++) {
                    for (std::size_t j = 0; j < 5; j++) {
                        if (out.getBit(i, j)) {
                            mvwaddch(cipher_win, i + 2, 10 + j * 2, ACS_CKBOARD);
                        } else {
                            mvwaddch(cipher_win, i + 2, 10 + j * 2, ' ');
                        }
                    }
                }
                break;
        }

        // Update bitset index indicator and counters
        werase(bitset_win);
        box(bitset_win, 0, 0);
        attron(COLOR_PAIR(4));
        mvwprintw(bitset_win, 0, 2, " Bitset Index Indicator ");
        attroff(COLOR_PAIR(4));
        mvwprintw(bitset_win, 1, 1, "Index: %d", bitset_index);
        for (int i = 0; i < 5; i++) {
            attron(COLOR_PAIR(5));
            mvwprintw(bitset_win, 2, 1 + i * 12, "Bit %d: %d", i, row_shift_counts[i]);
            attroff(COLOR_PAIR(5));
        }

        // Update the arrow indicator for the bitset index in the left panel
        werase(left_win);
        box(left_win, 0, 0);
        attron(COLOR_PAIR(1));
        mvwprintw(left_win, 0, 2, " Cipher Key ");
        attroff(COLOR_PAIR(1));
        mvwprintw(left_win, 1, 10 + bitset_index * 2, "^");
        for (int i = 0; i < 32; i++) {
            mvwprintw(left_win, i + 2, 1, "%c - %d", lol[i], i + 1);
            auto result = t.get(lol[i]);
            bool found = result.first;
            std::bitset<5> b = result.second;
            if (found) {
                for (std::size_t j = 0; j < 5; j++) {
                    if (b[j]) {
                        mvwaddch(left_win, i + 2, 10 + j * 2, ACS_CKBOARD);
                    } else {
                        mvwaddch(left_win, i + 2, 10 + j * 2, ' ');
                    }
                }
            }
        }

        // Update the arrow indicator for the bitset index in the cipher panel
        werase(cipher_win);
        box(cipher_win, 0, 0);
        attron(COLOR_PAIR(1));
        mvwprintw(cipher_win, 0, 2, " Cipher ");
        attroff(COLOR_PAIR(1));
        mvwprintw(cipher_win, 1, 10 + bitset_index * 2, "^");
        for (std::size_t i = 0; i < out.size(); i++) {
            for (std::size_t j = 0; j < 5; j++) {
                if (out.getBit(i, j)) {
                    mvwaddch(cipher_win, i + 2, 10 + j * 2, ACS_CKBOARD);
                } else {
                    mvwaddch(cipher_win, i + 2, 10 + j * 2, ' ');
                }
            }
        }

        update_panels();
        doupdate();
    }

    del_panel(left_panel);
    delwin(left_win);
    del_panel(cipher_panel);
    delwin(cipher_win);
    del_panel(right_panel);
    delwin(right_win);
    del_panel(manip_panel);
    delwin(manip_win);
    del_panel(bitset_panel);
    delwin(bitset_win);
    endwin(); // End curses mode
#else
    // Simple console output
    cout << "\nOriginal plaintext: " << test << endl;
    cout << "Encrypted ciphertext:" << endl;
    
    // Display ciphertext as a grid of bits
    for (std::size_t i = 0; i < out.size(); i++) {
        cout << "Letter " << i << ": ";
        for (std::size_t j = 0; j < 5; j++) {
            cout << (out.getBit(i, j) ? "1" : "0");
        }
        cout << endl;
    }
    
    cout << "\nDecrypted plaintext: " << plaintext << endl;
    
    // Interactive bit manipulation
    cout << "\nSimple row shift manipulation:" << endl;
    cout << "Enter bit row to manipulate (0-4) or -1 to quit: ";
    int bitset_index;
    cin >> bitset_index;
    
    while (bitset_index >= 0 && bitset_index < 5) {
        cout << "Direction (0=left, 1=right): ";
        bool direction;
        cin >> direction;
        
        // Perform row shift
        out.row_shift(bitset_index, direction);
        
        // Regenerate and update the plaintext
        ps.translate(out, plaintext);
        plaintext[sz] = '\0';
        
        cout << "Updated plaintext: " << plaintext << endl;
        cout << "Enter bit row to manipulate (0-4) or -1 to quit: ";
        cin >> bitset_index;
    }
#endif

    return 0;
}