#pragma once

#include <param.hpp>
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
        const auto& p = params.get();
        timer.set(p.count * p.frequency);
        sleep_time_ms = 1000U / p.frequency;
        serial_cli.begin(115200);
    }


    void loop() {
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
            serial_cli.println("  reset - Reset the timer");
            serial_cli.println("  <count> - Set the timer count (in seconds)");
        };

        if (input == "help") {
            print_help();

        } else if (input == "status") {
            serial_cli.printf("Timer count: %u\n", timer.get_count());

        } else if (input == "reset") {
            timer.reset();
            serial_cli.println("Timer reset.");

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
        const auto count = timer.get_count();

        if (count % frequency == 0) {
            digitalWrite(GPIO_LED, LOW);
        } else {
            digitalWrite(GPIO_LED, HIGH);
        }
    }
        
    void reset_timer() {
        timer.reset();
    }

    static App* app_instance;

    static App* instance() {
            return app_instance;
    }

    static void IRAM_ATTR interrupt_handler() {
        instance()->reset_timer();
    }

private:
    Parameter params;
    Timer timer;
    unsigned int sleep_time_ms = 0;

    HardwareSerial serial_cli = HardwareSerial(0);

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

    static constexpr unsigned int frequency = 10U; // Hz

};

App* App::app_instance = nullptr;
