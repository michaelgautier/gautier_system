#include "keyboardtr.hxx"

using cls = ::visualfunc::formulation::keyboardtr;

extern const char chars[];
extern const char alt_chars[];

bool cls::check_keyboard(interactionstate* interaction_ctx, const float wait_time) {
    const bool has_keyboard_event = al_wait_for_event_timed(keyboard_evt_queue, &_keyboard_event, wait_time);

    if(has_keyboard_event) {
        ALLEGRO_EVENT_TYPE keyboard_event_type = _keyboard_event.type;

        const int keycode = _keyboard_event.keyboard.keycode;
        const unsigned keymodifiers = _keyboard_event.keyboard.modifiers;

        interaction_ctx->IsKeyAvailable = true;
        interaction_ctx->KeyboardKeyCode = keycode;
        interaction_ctx->KeyModifiers = keymodifiers;
        interaction_ctx->IsKeyDown = false;
        interaction_ctx->IsKeyUp = false;

        switch (keyboard_event_type) {
        case ALLEGRO_EVENT_KEY_DOWN:
            interaction_ctx->IsKeyDown = true;
            //cout << "key down\n";
            break;
        case ALLEGRO_EVENT_KEY_UP:
            interaction_ctx->IsKeyUp = true;
            //cout << "key up\n";
            break;
        case ALLEGRO_EVENT_KEY_CHAR:
            cout << "ALLEGRO_EVENT_KEY_CHAR, event\n";
            break;
        }

        switch(keycode) {
        case ALLEGRO_KEY_LSHIFT:
        case ALLEGRO_KEY_RSHIFT:
            interaction_ctx->IsShiftDown = interaction_ctx->IsKeyDown;
            if(interaction_ctx->IsKeyUp) {
                interaction_ctx->IsShiftDown = false;
            }
            break;
        case ALLEGRO_KEY_CAPSLOCK:
            interaction_ctx->IsCapsLockOn = interaction_ctx->IsKeyDown;
            break;
        }
        /*cout << "keyboard\n";
        cout << "\t IsKeyAvailable " << interaction_ctx->IsKeyAvailable << "\n";
        cout << "\t KeyboardKeyCode " << interaction_ctx->KeyboardKeyCode << "\n";
        cout << "\t IsKeyDown " << interaction_ctx->IsKeyDown << "\n";
        cout << "\t IsKeyUp " << interaction_ctx->IsKeyUp << "\n";*/
    }

    return has_keyboard_event;
}

int cls::process_keyboard(interactionstate* interaction_ctx, interactionstate* interaction_ctx_previous) {
    int key_update_count = 0;

    int
    keycode = interaction_ctx->KeyboardKeyCode;
    unsigned
    keymodifiers = interaction_ctx->KeyModifiers;

    const bool
    is_keyboard_caps_on = (interaction_ctx->IsShiftDown || (!interaction_ctx->IsShiftDown && interaction_ctx->IsCapsLockOn));

    const bool
    is_keyboard_key_available = interaction_ctx->IsKeyAvailable;

    const bool
    is_keyboard_key_pressed = (interaction_ctx->IsKeyUp && interaction_ctx_previous->IsKeyDown);

    const bool
    is_last_key_same = (interaction_ctx->KeyboardKeyCode == interaction_ctx_previous->KeyboardKeyCode);

    /*if(_inter_sts_lst->IsKeyDown == true) {
        cout << "1 last state keydown\n";

        if(interaction_ctx->IsKeyUp == true) {
            cout << "1 state is key up\n";
        }
    }

    if(interaction_ctx->IsKeyUp == true) {
        cout << "0 state is key up\n";
    }

    if(is_keyboard_key_pressed == true) {
        cout << "is_keyboard_key_pressed\n";
    }

    if(keycode > 0 && is_last_key_same == true) {
        cout << "is_last_key_same\n";
    }*/

    if(text_buffer_index != -1 && is_keyboard_key_available && is_keyboard_key_pressed) {
        key_update_count++;
        //cout << "keycode " << keycode << " = " << al_keycode_to_name(keycode) << "\n";
        string* const text = (&(&texts[text_buffer_index])->text);

        /*Ignore these keys for now.*/
        switch (keycode) {
        case ALLEGRO_KEY_BACKSPACE:
            break;
        case ALLEGRO_KEY_DELETE:
        case ALLEGRO_KEY_PAD_DELETE:
            break;
        case ALLEGRO_KEY_HOME:
            break;
        case ALLEGRO_KEY_END:
            break;
        case ALLEGRO_KEY_RIGHT:
            break;
        case ALLEGRO_KEY_LEFT:
            break;
        //case ALLEGRO_KEY_SPACE:
        //_text_buffer_feed_entry->append(" ");
        //break;
        default: {
            char d = get_al_char_from_keycode(keycode, is_keyboard_caps_on);

            //auto& n = std::use_facet<std::ctype<wchar_t>>(std::locale());;
            //char c = n.narrow(d, 0);

            //cout << "keycode: " << keycode << " keymodifiers: " << keymodifiers << " encoded data: " << d << "\n";

            //_text_buffer_feed_entry->append(al_keycode_to_name(keycode));
            if(d > 0) {
                text->push_back(d);
            }
            //cout << "_text_buffer_feed_entry \t " << *_text_buffer_feed_entry << "\n";
        }
        break;
        }
    }

    return key_update_count;
}

const char cls::get_al_char_from_keycode(int keycode, bool is_keyboard_caps_on) {
    char d = 0;

    bool keysel1 = (keycode >= 1 && keycode <= 46);
    bool keysel2 = (keycode >= 60 && keycode <= 62);
    bool keysel3 = (keycode >= 65 && keycode <= 66);
    bool keysel4 = (keycode >= 68 && keycode <= 70);
    bool keysel5 = (keycode >= 72 && keycode <= 75);
    bool keysel6 = (keycode >= 86 && keycode <= 89);
    bool keysel7 = (keycode >= 101 && keycode <= 104);

    if(keysel1 || keysel2 || keysel3 || keysel4 || keysel5 || keysel6 || keysel7) {
        if(keycode > -1 && keycode < 27) {
            d = chars[keycode];

            if(is_keyboard_caps_on) {
                d = std::toupper(d);
            } else {
                d = std::tolower(d);
            }
        } else {
            if(is_keyboard_caps_on) {
                d = alt_chars[keycode];
            } else {
                d = chars[keycode];
            }
        }
    } else {
        d = -1;
    }

    return d;
}

const
char chars[] = {
    ' ',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z',

    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',

    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',

    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',

    ' ',
    '`',
    '-',
    '=',
    ' ',
    '\t',
    '[',
    ']',
    ' ',
    ';',
    '\'',
    '\\',
    ' ',
    ',',
    '.',
    '/',
    ' ',//space

    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',

    '/',
    '*',
    '-',
    '+',
    ' ',
    ' ',

    ' ',
    ' ',

    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ':',
    '~',

    '=',/* MacOS X */
    '`' /* MacOS X */
};

const
char alt_chars[] = {
    ' ',
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',

    ')',
    '!',
    '@',
    '#',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',

    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',

    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',

    ' ',
    '~',
    '_',
    '+',
    ' ',
    '\t',
    '{',
    '}',
    ' ',
    ':',
    '"',
    '|',
    ' ',
    '<',
    '>',
    '?',
    ' ',//space

    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',

    '/',
    '*',
    '-',
    '+',
    ' ',
    ' ',

    ' ',
    ' ',

    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ':',
    '~',

    '=',/* MacOS X */
    '`' /* MacOS X */
};
