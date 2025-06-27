#pragma once

#include <eeprom.hpp>
#include <timer.hpp>

#include "board.hpp"



class App {
public:
    App() {
        app_instance = this;
    }

    void setup() {
        setup_gpio();
        params.init();
        statistics.init();
        const auto& p = params.get();
        timer.set_init_count(p.count * p.frequency);
        sleep_time_ms = 1000U / p.frequency;
        serial_cli.begin(115200);
    }


    void loop() {
        if (was_button_pressed) {
            was_button_pressed = false;
            statistics.bump_reset_button_count();
            timer.reset();
        }

        set_relay(!timer.tick());
        led_blink();
        CLI();
        delay(sleep_time_ms);
    }

    void CLI() {
        if (serial_cli.available() <= 0) {
            return;
        }         

        String input = serial_cli.readStringUntil('\n');
        input.trim();

        auto print_help = [&]() {
            serial_cli.println("Usage:");
            serial_cli.println("  help - Show this help message");
            serial_cli.println("  status - Show current status");
            serial_cli.println("  reset - Reset the statistics");
            serial_cli.println("  <count> - Set the timer count (in seconds)");
        };

        if (input == "help") {
            print_help();

        } else if (input == "status") {
            const auto& s = statistics.get();
            serial_cli.printf("Timer count:        %u\n", timer.get_count());
            serial_cli.printf("Initial count:      %u\n", timer.get_init_count());
            serial_cli.printf("Boot count:         %llu\n", s.boot_count);
            serial_cli.printf("Reset button count: %llu\n", s.reset_button_count);

        } else if (input == "reset") {
            statistics.reset();
            serial_cli.println("Statistics reset.");

        } else if (input.toInt() > 10) {
            unsigned int new_count = input.toInt();
            unsigned int old_count = params.get().count;
            params.get().count = new_count;
            params.save();
            serial_cli.printf("Count changed: %u -> %u.\n", old_count, new_count);

        } else {
            serial_cli.println("Unknown command.");
            print_help();
        }
    }

    void led_blink() {
        constexpr auto start_end_time = 5U;

        const auto& p = params.get();
        const auto count = timer.get_count();

        if (count < p.frequency * start_end_time) {
            set_led(true);
        
        } else if (timer.get_init_count() -  count < p.frequency * start_end_time) {
            set_led(false);
        }
        else {
            set_led(count % p.frequency == 0);
        }
    }

    static App* app_instance;

    static App* instance() {
            return app_instance;
    }

    static void IRAM_ATTR interrupt_handler() {
        instance()->was_button_pressed = true;
    }

private:
    Parameter params;
    Statistic statistics;
    Timer timer;

    HardwareSerial serial_cli = HardwareSerial(0);
    unsigned int sleep_time_ms = 0;
    bool was_button_pressed = false;

    void setup_gpio() {
        pinMode(GPIO_RELAY_1, OUTPUT);
        pinMode(GPIO_RELAY_2, OUTPUT);
        pinMode(GPIO_LED, OUTPUT);

        // Keep the power on.
        digitalWrite(GPIO_RELAY_1, HIGH);
        // Turn on device.
        digitalWrite(GPIO_RELAY_2, HIGH);

        pinMode(GPIO_BUTTON, INPUT_PULLUP);

        attachInterrupt(digitalPinToInterrupt(GPIO_BUTTON), interrupt_handler, FALLING);
    }

    void set_relay(bool on) {
        digitalWrite(GPIO_RELAY_1, on ? HIGH : LOW);
        digitalWrite(GPIO_RELAY_2, on ? HIGH : LOW);
    }

    void set_led(bool on) {
        digitalWrite(GPIO_LED, on ? LOW : HIGH);
    }
};

App* App::app_instance = nullptr;
